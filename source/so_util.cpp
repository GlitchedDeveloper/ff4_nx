/* so_util.cpp -- utils to load and hook .so modules
 *
 * Copyright (C) 2026 GlitchedDeveloper, givethesourceplox, Andy Nguyen, fgsfds
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "so_util.h"

#include <assert.h>
#define DA_NOSTRUCT
#include <disarm64.h>
#include <elf.h>
#include <malloc.h>
#include <switch.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "error.h"
#include "util.h"

namespace so {
void *text_base, *text_virtbase;
size_t text_size;

void *data_base, *data_virtbase;
size_t data_size;

void *trampoline_base, *trampoline_virtbase;
size_t trampoline_size = 1024 * 1024; // 1 MB
uintptr_t trampoline_cursor;

static void *load_base, *load_virtbase;
static size_t load_size;
static VirtmemReservation* load_memrv;

static void* so_base;

static Elf64_Ehdr* elf_hdr;
static Elf64_Phdr* prog_hdr;
static Elf64_Shdr* sec_hdr;
static Elf64_Sym* syms;
static int num_syms;

static char* shstrtab;
static char* dynstrtab;

static const char* importNameNover(const char* name, char* buf, size_t buf_size) {
    strncpy(buf, name, buf_size - 1);
    buf[buf_size - 1] = '\0';

    char* at = strchr(buf, '@');
    if (at)
        *at = '\0';

    return buf;
}

static int lookupImportSymbol(DynLibFunction* funcs, int num_funcs, const char* name, uintptr_t* out_addr, int* used_cpplib) {
    char clean_name[256];
    const char* lookup_name = importNameNover(name, clean_name, sizeof(clean_name));

    for (int k = 0; k < num_funcs; k++) {
        if (strcmp(lookup_name, funcs[k].symbol) == 0) {
            *out_addr = funcs[k].func;
            if (used_cpplib)
                *used_cpplib = 0;
            return 1;
        }
    }

    return 0;
}

void CodeBuf::w32(uint32_t v) {
    ((uint32_t*)rw)[count] = v;
    this->count += 1;
}

void CodeBuf::w64(uint64_t v) {
    uint32_t* ptr  = (uint32_t*)rw;
    ptr[count]     = (uint32_t)v;
    ptr[count + 1] = (uint32_t)(v >> 32);
    this->count += 2;
}

void CodeBuf::wb(int64_t target) {
    int64_t offset = (int64_t)target - (int64_t)(rx + (count * 4));
    if (offset < -0x08000000 || offset > 0x07ffffff) {
        fatal_error("Error: function outside 128MB range (offset: %d)", offset);
    }
    this->w32(de64_B(offset / 4));
}

void CodeBuf::wjump(uint64_t target) {
    this->w32(de64_LDRx_pcrel(DA_GP(17), 2));
    this->w32(de64_BR(DA_GP(17)));
    this->w64(target);
}

void CodeBuf::copy(uint32_t instruction, uint64_t orig_rx) {
    uint64_t new_rx = this->rx + (uint64_t)(this->count * 4);

    struct Da64Inst ddi;
    da64_decode(instruction, &ddi);

    switch (ddi.mnem) {
        case DA64I_B:
        case DA64I_BL: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            int64_t off    = target - (int64_t)new_rx;
            uint32_t enc   = (ddi.mnem == DA64I_B)
                ? de64_B(off / 4)
                : de64_BL(off / 4);
            if (!enc)
                fatal_error("Error: B/BL relocation out of range in trampoline copy (offset: %lld)", (long long)off);
            this->w32(enc);
            return;
        }
        case DA64I_BCOND:
        case DA64I_BCCOND: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            int64_t off    = target - (int64_t)new_rx;
            Da64Cond cond  = (Da64Cond)ddi.ops[0].cond;
            uint32_t enc   = (ddi.mnem == DA64I_BCOND)
                ? de64_BCOND(cond, off / 4)
                : de64_BCCOND(cond, off / 4);
            if (!enc)
                fatal_error("Error: B.cond relocation out of range in trampoline copy");
            this->w32(enc);
            return;
        }
        case DA64I_CBZ:
        case DA64I_CBNZ: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            int64_t off    = target - (int64_t)new_rx;
            int sf         = ddi.ops[0].reggp.sf;
            DA_GReg Rt     = DA_GP(ddi.ops[0].reg);

            uint32_t enc = (ddi.mnem == DA64I_CBZ)
                ? (sf ? de64_CBZx(Rt, off / 4) : de64_CBZw(Rt, off / 4))
                : (sf ? de64_CBNZx(Rt, off / 4) : de64_CBNZw(Rt, off / 4));
            if (enc) {
                this->w32(enc);
                return;
            }

            int64_t jump_pc  = (int64_t)rx + (int64_t)(this->count * 4) + 4;
            int64_t jump_off = target - jump_pc;

            if (jump_off >= -0x08000000 && jump_off <= 0x07FFFFFC) {
                uint32_t inv = (ddi.mnem == DA64I_CBZ)
                    ? (sf ? de64_CBNZx(Rt, 2) : de64_CBNZw(Rt, 2))
                    : (sf ? de64_CBZx(Rt, 2) : de64_CBZw(Rt, 2));
                this->w32(inv);
                this->wb(target);
            } else {
                uint32_t inv = (ddi.mnem == DA64I_CBZ)
                    ? (sf ? de64_CBNZx(Rt, 5) : de64_CBNZw(Rt, 5))
                    : (sf ? de64_CBZx(Rt, 5) : de64_CBZw(Rt, 5));
                this->w32(inv);
                this->wjump(target);
            }
            return;
        }
        case DA64I_TBZ:
        case DA64I_TBNZ: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            int64_t off    = target - (int64_t)new_rx;
            unsigned bit   = ddi.ops[1].uimm16;
            DA_GReg Rt     = DA_GP(ddi.ops[0].reg);

            uint32_t enc = (ddi.mnem == DA64I_TBZ)
                ? de64_TBZ(Rt, bit, off / 4)
                : de64_TBNZ(Rt, bit, off / 4);
            if (enc) {
                this->w32(enc);
                return;
            }

            int64_t jump_pc  = (int64_t)rx + (int64_t)(this->count * 4) + 4;
            int64_t jump_off = target - jump_pc;

            if (jump_off >= -0x08000000 && jump_off <= 0x07ffffff) {
                uint32_t inv = (ddi.mnem == DA64I_TBZ)
                    ? de64_TBNZ(Rt, bit, 2)
                    : de64_TBZ(Rt, bit, 2);
                this->w32(inv);
                this->wb(target);
            } else {
                uint32_t inv = (ddi.mnem == DA64I_TBZ)
                    ? de64_TBNZ(Rt, bit, 5)
                    : de64_TBZ(Rt, bit, 5);
                this->w32(inv);
                this->wjump(target);
            }
            return;
        }
        case DA64I_LDRW_LIT: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            int64_t off    = target - (int64_t)new_rx;
            uint32_t enc   = de64_LDRw_pcrel(DA_GP(ddi.ops[0].reg), off / 4);
            if (!enc)
                fatal_error("Error: LDR literal relocation out of range in trampoline copy");
            this->w32(enc);
            return;
        }
        case DA64I_LDR_LIT: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            int64_t off    = target - (int64_t)new_rx;
            uint32_t enc   = de64_LDRx_pcrel(DA_GP(ddi.ops[0].reg), off / 4);
            if (!enc)
                fatal_error("Error: LDR literal relocation out of range in trampoline copy");
            this->w32(enc);
            return;
        }
        case DA64I_LDRSW_LIT: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            int64_t off    = target - (int64_t)new_rx;
            uint32_t enc   = de64_LDRSWx_pcrel(DA_GP(ddi.ops[0].reg), off / 4);
            if (!enc)
                fatal_error("Error: LDR literal relocation out of range in trampoline copy");
            this->w32(enc);
            return;
        }
        case DA64I_PRFM_LIT: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            int64_t off    = target - (int64_t)new_rx;
            uint32_t enc   = de64_PRFM_pcrel((Da64PrfOp)ddi.ops[0].prfop, off / 4);
            if (!enc)
                fatal_error("Error: PRFM literal relocation out of range in trampoline copy");
            this->w32(enc);
            return;
        }
        case DA64I_LDR_LIT_FP: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            int64_t off    = target - (int64_t)new_rx;
            DA_VReg Rt     = DA_V(ddi.ops[0].reg);
            uint32_t enc   = 0;
            switch (ddi.ops[0].regfp.size) {
                case 2:
                    enc = de64_LDRs_pcrel(Rt, off / 4);
                    break;
                case 3:
                    enc = de64_LDRd_pcrel(Rt, off / 4);
                    break;
                case 4:
                    enc = de64_LDRq_pcrel(Rt, off / 4);
                    break;
            }
            if (!enc)
                fatal_error("Error: LDR FP literal relocation out of range in trampoline copy");
            this->w32(enc);
            return;
        }
        case DA64I_ADR: {
            int64_t target = (int64_t)orig_rx + (int64_t)ddi.imm64;
            uint32_t enc   = de64_ADR(DA_GP(ddi.ops[0].reg), new_rx, (uintptr_t)target);
            if (!enc)
                fatal_error("Error: ADR relocation out of range in trampoline copy");
            this->w32(enc);
            return;
        }
        case DA64I_ADRP: {
            int64_t target_page = ((int64_t)orig_rx & ~0xFFFLL) + (int64_t)ddi.imm64;
            uint32_t enc        = de64_ADRP(DA_GP(ddi.ops[0].reg), new_rx, (uintptr_t)target_page);
            if (!enc)
                fatal_error("Error: ADRP relocation out of range in trampoline copy");
            this->w32(enc);
            return;
        }
        default:
            this->w32(instruction);
            return;
    }
}

bool addr_is_in_rx(uintptr_t addr) {
    return (addr >= (uintptr_t)load_virtbase && addr < (uintptr_t)load_virtbase + load_size);
}

uintptr_t addr_rx_to_rw(uintptr_t addr) {
    if (!addr_is_in_rx(addr))
        return 0;
    return (uintptr_t)load_base + (addr - (uintptr_t)load_virtbase);
}

bool addr_is_in_rw(uintptr_t addr) {
    return (addr >= (uintptr_t)load_base && addr < (uintptr_t)load_base + load_size);
}

uintptr_t addr_rw_to_rx(uintptr_t addr) {
    if (!addr_is_in_rw(addr))
        return 0;
    return (uintptr_t)load_virtbase + (addr - (uintptr_t)load_base);
}

void hook(uintptr_t addr, uintptr_t dst, uintptr_t* trampoline_out) {
    if (addr == 0)
        return;

    uintptr_t addr_rw;
    uintptr_t addr_rx;
    if (addr_is_in_rx(addr)) {
        addr_rx = addr;
        addr_rw = addr_rx_to_rw(addr);
    } else if (addr_is_in_rw(addr)) {
        addr_rx = addr_rw_to_rx(addr);
        addr_rw = addr;
    } else {
        fatal_error("Error: addr outsize so memory");
    }

    uintptr_t stub_rw = ((uintptr_t)trampoline_base + trampoline_cursor);
    uintptr_t stub_rx = ((uintptr_t)trampoline_virtbase + trampoline_cursor);
    CodeBuf stub { stub_rw, stub_rx };
    stub.wjump(dst);
    trampoline_cursor += stub.count * 4;

    if (trampoline_out != nullptr) {
        uintptr_t trampoline_rw = ((uintptr_t)trampoline_base + trampoline_cursor);
        uintptr_t trampoline_rx = ((uintptr_t)trampoline_virtbase + trampoline_cursor);
        CodeBuf trampoline { trampoline_rw, trampoline_rx };
        trampoline.copy(((uint32_t*)addr_rw)[0], addr_rx);
        trampoline.wb(addr_rx + 4);
        trampoline_cursor += trampoline.count * 4;
        *trampoline_out = trampoline_rx;
    }

    CodeBuf orig { addr_rw, addr_rx };
    orig.wb(stub_rx);
}

void flushCaches(void) {
    armDCacheFlush(load_virtbase, load_size);
    armICacheInvalidate(load_virtbase, load_size);
}

void freeTemp(void) {
    free(so_base);
    so_base = nullptr;
}

void finalize(void) {
    Result rc = 0;

    // map the entire thing as code memory
    rc = svcMapProcessCodeMemory(envGetOwnProcessHandle(), (u64)load_virtbase, (u64)load_base, load_size);
    if (R_FAILED(rc))
        fatal_error("Error: svcMapProcessCodeMemory failed:\n%08x", rc);

    // map text sections as R+X
    const u64 text_asize = ALIGN_MEM(text_size, 0x1000); // align to page
    rc                   = svcSetProcessMemoryPermission(envGetOwnProcessHandle(), (u64)text_virtbase, text_asize, Perm_Rx);
    if (R_FAILED(rc))
        fatal_error("Error: could not map %u bytes of RX memory at %p:\n%08x", text_asize, text_virtbase, rc);

    // map data sections as R+W
    const u64 data_asize          = ALIGN_MEM(load_size - text_asize - trampoline_size, 0x1000);
    const uintptr_t data_virtbase = (uintptr_t)text_virtbase + text_asize;
    rc                            = svcSetProcessMemoryPermission(envGetOwnProcessHandle(), data_virtbase, data_asize, Perm_Rw);
    if (R_FAILED(rc))
        fatal_error("Error: could not map %u bytes of RW memory at %p (%p) (2):\n%08x", data_asize, data_virtbase, data_virtbase, rc);

    // map trampoline sections as R+X
    const u64 trampoline_asize = load_size - text_asize - data_asize;
    rc                         = svcSetProcessMemoryPermission(envGetOwnProcessHandle(), (u64)trampoline_virtbase, trampoline_asize, Perm_Rx);
    if (R_FAILED(rc))
        fatal_error("Error: could not map %u bytes of RX memory at %p:\n%08x", trampoline_asize, trampoline_virtbase, rc);
}

int load(const char* filename, void* base, size_t max_size) {
    int res        = 0;
    size_t so_size = 0;
    int text_segno = -1;
    int data_segno = -1;

    FILE* fd = fopen(filename, "rb");
    if (fd == nullptr)
        return -1;

    fseek(fd, 0, SEEK_END);
    so_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    so_base = malloc(so_size);
    if (!so_base) {
        fclose(fd);
        return -2;
    }

    fread(so_base, so_size, 1, fd);
    fclose(fd);

    if (memcmp(so_base, ELFMAG, SELFMAG) != 0) {
        res = -1;
        goto err_free_so;
    }

    elf_hdr  = (Elf64_Ehdr*)so_base;
    prog_hdr = (Elf64_Phdr*)((uintptr_t)so_base + elf_hdr->e_phoff);
    sec_hdr  = (Elf64_Shdr*)((uintptr_t)so_base + elf_hdr->e_shoff);
    shstrtab = (char*)((uintptr_t)so_base + sec_hdr[elf_hdr->e_shstrndx].sh_offset);

    // calculate total size of the LOAD segments
    for (int i = 0; i < elf_hdr->e_phnum; i++) {
        if (prog_hdr[i].p_type == PT_LOAD) {
            const size_t prog_size = ALIGN_MEM(prog_hdr[i].p_memsz, prog_hdr[i].p_align);
            // get the segment numbers of text and data segments
            if ((prog_hdr[i].p_flags & PF_X) == PF_X) {
                text_segno = i;
            } else {
                // assume data has to be after text
                if (text_segno < 0)
                    goto err_free_so;
                if (data_segno < 0)
                    data_segno = i;
            }

            if (load_size < prog_hdr[i].p_vaddr + prog_size)
                load_size = prog_hdr[i].p_vaddr + prog_size;
        }
    }

    // align total size to page size
    load_size = ALIGN_MEM(load_size, 0x1000);
    load_size += trampoline_size;
    load_size = ALIGN_MEM(load_size, 0x1000);
    if (load_size > max_size) {
        res = -3;
        goto err_free_so;
    }

    // allocate space for all load segments (align to page size)
    // TODO: find out a way to allocate memory that doesn't fuck with the heap
    load_base = base;
    if (!load_base)
        goto err_free_so;
    memset(load_base, 0, load_size);

    // reserve virtual memory space for the entire LOAD zone while we're fucking with the ELF
    virtmemLock();
    load_virtbase = virtmemFindCodeMemory(load_size, 0x1000);
    load_memrv    = virtmemAddReservation(load_virtbase, load_size);
    virtmemUnlock();

    trampoline_base     = (void*)((uintptr_t)load_base + (load_size - trampoline_size));
    trampoline_virtbase = (void*)((uintptr_t)load_virtbase + (load_size - trampoline_size));

    debugPrintf("load base = %p\n", load_virtbase);

    // copy all PT_LOAD segments to where they belong
    for (int i = 0; i < elf_hdr->e_phnum; i++) {
        if (prog_hdr[i].p_type != PT_LOAD)
            continue;

        void* seg_virtbase = (void*)(prog_hdr[i].p_vaddr + (Elf64_Addr)load_virtbase);
        void* seg_base     = (void*)(prog_hdr[i].p_vaddr + (Elf64_Addr)load_base);

        if ((prog_hdr[i].p_flags & PF_X) == PF_X) {
            text_size     = prog_hdr[i].p_memsz;
            text_virtbase = seg_virtbase;
            text_base     = seg_base;
        } else if (data_segno == i) {
            data_size     = prog_hdr[i].p_memsz;
            data_virtbase = seg_virtbase;
            data_base     = seg_base;
        }

        prog_hdr[i].p_vaddr = (Elf64_Addr)seg_virtbase;
        memcpy(seg_base, (void*)((uintptr_t)so_base + prog_hdr[i].p_offset), prog_hdr[i].p_filesz);
    }

    syms      = nullptr;
    dynstrtab = nullptr;

    for (int i = 0; i < elf_hdr->e_shnum; i++) {
        char* sh_name = shstrtab + sec_hdr[i].sh_name;
        if (strcmp(sh_name, ".dynsym") == 0) {
            syms     = (Elf64_Sym*)((uintptr_t)load_base + sec_hdr[i].sh_addr);
            num_syms = sec_hdr[i].sh_size / sizeof(Elf64_Sym);
        } else if (strcmp(sh_name, ".dynstr") == 0) {
            dynstrtab = (char*)((uintptr_t)load_base + sec_hdr[i].sh_addr);
        }
    }

    if (syms == nullptr || dynstrtab == nullptr) {
        res = -2;
        goto err_free_load;
    }

    return 0;

err_free_load:
    virtmemLock();
    virtmemRemoveReservation(load_memrv);
    virtmemUnlock();
    free(load_base);
err_free_so:
    free(so_base);

    return res;
}

int relocate(void) {
    for (int i = 0; i < elf_hdr->e_shnum; i++) {
        char* sh_name = shstrtab + sec_hdr[i].sh_name;
        if (strcmp(sh_name, ".rela.dyn") == 0 || strcmp(sh_name, ".rela.plt") == 0) {
            Elf64_Rela* rels = (Elf64_Rela*)((uintptr_t)load_base + sec_hdr[i].sh_addr);
            for (int j = 0; j < sec_hdr[i].sh_size / sizeof(Elf64_Rela); j++) {
                uintptr_t* ptr = (uintptr_t*)((uintptr_t)load_base + rels[j].r_offset);
                Elf64_Sym* sym = &syms[ELF64_R_SYM(rels[j].r_info)];

                int type = ELF64_R_TYPE(rels[j].r_info);
                switch (type) {
                    case R_AARCH64_ABS64:
                        if (sym->st_shndx != SHN_UNDEF) {
                            *ptr = (uintptr_t)load_virtbase + sym->st_value + rels[j].r_addend;
                        } else {
                            // Undefined ABS64 imports are resolved later via the static
                            // import table / libc++ loader. Zero them here so file contents
                            // cannot leave stale garbage pointers in vtables.
                            *ptr = 0;
                        }
                        break;

                    case R_AARCH64_RELATIVE:
                        // sometimes the value of r_addend is also at *ptr
                        *ptr = (uintptr_t)load_virtbase + rels[j].r_addend;
                        break;

                    case R_AARCH64_GLOB_DAT:
                    case R_AARCH64_JUMP_SLOT: {
                        if (sym->st_shndx != SHN_UNDEF)
                            *ptr = (uintptr_t)load_virtbase + sym->st_value + rels[j].r_addend;
                        break;
                    }

                    default:
                        fatal_error("Error: unknown relocation type:\n%x\n", type);
                        break;
                }
            }
        }
    }

    return 0;
}

int resolve(DynLibFunction* funcs, int num_funcs, int taint_missing_imports) {
    int cpplib_resolved = 0;
    int cpplib_missed   = 0;

    for (int i = 0; i < elf_hdr->e_shnum; i++) {
        char* sh_name = shstrtab + sec_hdr[i].sh_name;
        if (strcmp(sh_name, ".rela.dyn") == 0 || strcmp(sh_name, ".rela.plt") == 0) {
            Elf64_Rela* rels = (Elf64_Rela*)((uintptr_t)load_base + sec_hdr[i].sh_addr);
            for (int j = 0; j < sec_hdr[i].sh_size / sizeof(Elf64_Rela); j++) {
                uintptr_t* ptr = (uintptr_t*)((uintptr_t)load_base + rels[j].r_offset);
                Elf64_Sym* sym = &syms[ELF64_R_SYM(rels[j].r_info)];

                int type = ELF64_R_TYPE(rels[j].r_info);
                switch (type) {
                    case R_AARCH64_ABS64:
                    case R_AARCH64_GLOB_DAT:
                    case R_AARCH64_JUMP_SLOT: {
                        if (sym->st_shndx == SHN_UNDEF) {
                            char* name              = dynstrtab + sym->st_name;
                            uintptr_t resolved_addr = 0;
                            int used_cpplib         = 0;

                            if (lookupImportSymbol(funcs, num_funcs, name, &resolved_addr, &used_cpplib)) {
                                *ptr = resolved_addr + rels[j].r_addend;
                                if (used_cpplib)
                                    cpplib_resolved++;
                            } else {
                                cpplib_missed++;
                                if (taint_missing_imports)
                                    *ptr = rels[j].r_offset;
                                debugPrintf("UNRESOLVED: %s (type=%d)\n", name, type);
                            }
                        }

                        break;
                    }

                    default:
                        break;
                }
            }
        }
    }

    debugPrintf("so_resolve: cpplib resolved %d, missed %d\n", cpplib_resolved, cpplib_missed);
    return 0;
}

void executeInitArray(void) {
    for (int i = 0; i < elf_hdr->e_shnum; i++) {
        char* sh_name = shstrtab + sec_hdr[i].sh_name;
        if (strcmp(sh_name, ".init_array") == 0) {
            int (**init_array)() = (int (**)())((uintptr_t)load_virtbase + sec_hdr[i].sh_addr);
            for (int j = 0; j < sec_hdr[i].sh_size / 8; j++) {
                if (init_array[j] != 0)
                    init_array[j]();
            }
        }
    }
}

uintptr_t findAddr_rw(const char* symbol) {
    debugPrintf("so_find_addr %s\n", symbol);
    for (int i = 0; i < num_syms; i++) {
        char* name = dynstrtab + syms[i].st_name;
        if (strcmp(name, symbol) == 0)
            return (uintptr_t)load_base + syms[i].st_value;
    }

    fatal_error("Error: could not find symbol:\n%s\n", symbol);
    return 0;
}

uintptr_t findRelAddr(const char* symbol) {
    for (int i = 0; i < elf_hdr->e_shnum; i++) {
        char* sh_name = shstrtab + sec_hdr[i].sh_name;
        if (strcmp(sh_name, ".rela.dyn") == 0 || strcmp(sh_name, ".rela.plt") == 0) {
            Elf64_Rela* rels = (Elf64_Rela*)((uintptr_t)load_base + sec_hdr[i].sh_addr);
            for (int j = 0; j < sec_hdr[i].sh_size / sizeof(Elf64_Rela); j++) {
                Elf64_Sym* sym = &syms[ELF64_R_SYM(rels[j].r_info)];

                int type = ELF64_R_TYPE(rels[j].r_info);
                if (type == R_AARCH64_GLOB_DAT || type == R_AARCH64_JUMP_SLOT) {
                    char* name = dynstrtab + sym->st_name;
                    if (strcmp(name, symbol) == 0)
                        return (uintptr_t)load_base + rels[j].r_offset;
                }
            }
        }
    }

    fatal_error("Error: could not find symbol:\n%s\n", symbol);
    return 0;
}

uintptr_t findAddr_rx(const char* symbol) {
    for (int i = 0; i < num_syms; i++) {
        char* name = dynstrtab + syms[i].st_name;
        if (strcmp(name, symbol) == 0)
            return (uintptr_t)load_virtbase + syms[i].st_value;
    }

    fatal_error("Error: could not find symbol:\n%s\n", symbol);
    return 0;
}

DynLibFunction* findImport(DynLibFunction* funcs, int num_funcs, const char* name) {
    for (int i = 0; i < num_funcs; ++i)
        if (!strcmp(funcs[i].symbol, name))
            return &funcs[i];
    return nullptr;
}

int unload(void) {
    if (load_base == nullptr)
        return -1;

    if (so_base) {
        // someone forgot to free the temp data
        freeTemp();
    }

    // remap text as RW
    const u64 text_asize = ALIGN_MEM(text_size, 0x1000); // align to page
    svcSetProcessMemoryPermission(envGetOwnProcessHandle(), (u64)text_virtbase, text_asize, Perm_Rw);
    // unmap everything
    svcUnmapProcessCodeMemory(envGetOwnProcessHandle(), (u64)load_virtbase, (u64)load_base, load_size);

    // release virtual address range
    virtmemLock();
    virtmemRemoveReservation(load_memrv);
    virtmemUnlock();

    return 0;
}
}