#pragma once

#include "so_util.h"
#include "types.h"
#include <GLES/gl.h>

#define HOOK_FUNCTION_BASE(name, dest) so::hook((uintptr_t)name, (uintptr_t)dest)
#define HOOK_FUNCTION_T_BASE(name, dest, trampoline) so::hook((uintptr_t)name, (uintptr_t)dest, (uintptr_t*)&trampoline)
#define HOOK_FUNCTION(name) HOOK_FUNCTION_BASE(babil::name, name)
#define HOOK_FUNCTION_T(name) \
    debugPrintf("HOOK_FUNCTION_T(" #name ")\n"); \
    HOOK_FUNCTION_T_BASE(babil::name, name, name##_t)
#define DECLARE_TRAMPOLINE(name, trampoline) static decltype(name) trampoline = nullptr

namespace babil {

using fx32 = s32;
using NNSG2dChar = c16;

struct NNSG3dResTex;
struct NNSG3dResFileHeader;

namespace AchievementCheckFuncs {
struct cls;
}

namespace AchievementReporter {
struct cls;
}

namespace AchievementResource {
struct cls;
}

namespace common::EfficacyDataConvection::HashNode {
struct cls;
}

namespace pl::Player {
struct cls;
}

namespace title::TitleContents {
struct cls;
}

namespace ui::Widget {
struct cls;
}

namespace world::WorldStateScheduler {
struct cls;
}

enum class enAchievementCheckTimings : u32 {
    Always = 0,
    EventStart = BIT(0),
    EventFinish = BIT(1),
    PlayNewGame = BIT(2),
    FreeMove = BIT(3),
    FreeVehicle = BIT(4),
    BattleEnd = BIT(5),
    Reported = BIT(6),
    UseAbilityItem = BIT(7),
    ShopSell = BIT(8),
};

enum class enAchievementCheckTypes : u32 {
    Flag = 0,
    FlagRange = 1,
    HardModeBeat = 2,
    BeatCount = 3,
    GetItem = 4,
    GetDecant = 5,
    GetDecantCnt = 6,
    LearnSummon = 7,
    LearnSummonCnt = 8,
    CompletedMap = 9,
    OverPlayerLevel = 10,
    Gill = 11,
    MonsterKillCount = 12,
    MonsterDictionary = 13,
    MonsterPartyKill = 14,
    NoCheck = 15,
    Field = 16,
    AchievementComp = 17,
};

// Per-monster bestiary entry (2 bytes). kill() caps m_KillCount at 999 and clears
// bits 14..15 via `& 0x3fff`.
struct __attribute__((packed)) MonsterMania {
    bool m_Encountered : 1;
    bool m_EncounteredAlt : 1;
    bool m_InLibrary : 1;
    bool m_3 : 1;
    u16 m_KillCount : 10;
    u16 _pad0 : 2;
};
enum class MonsterManiaMask : u16 {
    Encountered = BIT(0),
    EncounteredAlt = BIT(1),
    InLibrary = BIT(2),
    unk = BIT(3),
    KillCount = 0x3ff0,
};

struct __attribute__((packed)) VecFx32 {
    fx32 x; // 0x0
    fx32 y; // 0x4
    fx32 z; // 0x8
};
static_assert(sizeof(VecFx32) == 0xc);
static_assert(offsetof(VecFx32, x) == 0x0);
static_assert(offsetof(VecFx32, y) == 0x4);
static_assert(offsetof(VecFx32, z) == 0x8);

struct __attribute__((packed)) NNSG3dResMdl {
};

struct __attribute__((packed)) NNSG3dResMdlSet {
};

struct __attribute__((packed)) NNSG3dResTex {
};

struct __attribute__((packed)) NNSG3dResFileHeader {
};

struct __attribute__((packed)) NNSG2dCellAnimation {
};

struct __attribute__((packed)) AchievementFlagParam {
    s32 m_Bank; // 0x0
    s32 m_Offset; // 0x4
    bool m_Value; // 0x8
    u8 _pad0[3]; // 0x9
};
static_assert(sizeof(AchievementFlagParam) == 0xc);
static_assert(offsetof(AchievementFlagParam, m_Bank) == 0x0);
static_assert(offsetof(AchievementFlagParam, m_Offset) == 0x4);
static_assert(offsetof(AchievementFlagParam, m_Value) == 0x8);

struct __attribute__((packed)) AchievementFlagRangeParam {
    s32 m_Bank; // 0x0
    s32 m_Start; // 0x4
    s32 m_End; // 0x8
    bool m_Value; // 0xc
    u8 _pad0[3]; // 0xd
};
static_assert(sizeof(AchievementFlagRangeParam) == 0x10);
static_assert(offsetof(AchievementFlagRangeParam, m_Bank) == 0x0);
static_assert(offsetof(AchievementFlagRangeParam, m_Start) == 0x4);
static_assert(offsetof(AchievementFlagRangeParam, m_End) == 0x8);
static_assert(offsetof(AchievementFlagRangeParam, m_Value) == 0xc);

struct __attribute__((packed)) AchievementHardModeBeatParam {
    s32 m_MonsterPartyId; // 0x0
};
static_assert(sizeof(AchievementHardModeBeatParam) == 0x4);
static_assert(offsetof(AchievementHardModeBeatParam, m_MonsterPartyId) == 0x0);

struct __attribute__((packed)) AchievementBeatCountParam {
    s32 m_MonsterPartyId; // 0x0
    s32 m_Count; // 0x4
};
static_assert(sizeof(AchievementBeatCountParam) == 0x8);
static_assert(offsetof(AchievementBeatCountParam, m_MonsterPartyId) == 0x0);
static_assert(offsetof(AchievementBeatCountParam, m_Count) == 0x4);

struct __attribute__((packed)) AchievementGetItemParam {
    s32 m_ItemId; // 0x0
};
static_assert(sizeof(AchievementGetItemParam) == 0x4);
static_assert(offsetof(AchievementGetItemParam, m_ItemId) == 0x0);

struct __attribute__((packed)) AchievementGetDecantParam {
    s32 m_DecantId; // 0x0
};
static_assert(sizeof(AchievementGetDecantParam) == 0x4);
static_assert(offsetof(AchievementGetDecantParam, m_DecantId) == 0x0);

struct __attribute__((packed)) AchievementGetDecantCntParam {
    s32 m_Count; // 0x0
};
static_assert(sizeof(AchievementGetDecantCntParam) == 0x4);
static_assert(offsetof(AchievementGetDecantCntParam, m_Count) == 0x0);

namespace pl {
    enum class PLAYER_TYPES : s32 {
    };
}

namespace common {
    enum class ABILITY_ID : s8 {
    };
}

struct __attribute__((packed)) AchievementLearnSummonParam {
    pl::PLAYER_TYPES m_Player; // 0x0
    common::ABILITY_ID m_AbilityId; // 0x4
    u8 _pad0[3]; // 0x5
};
static_assert(sizeof(AchievementLearnSummonParam) == 0x8);
static_assert(offsetof(AchievementLearnSummonParam, m_Player) == 0x0);
static_assert(offsetof(AchievementLearnSummonParam, m_AbilityId) == 0x4);

struct __attribute__((packed)) AchievementLearnSummonCntParam {
    AchievementFlagParam m_Flags[2]; // 0x0
    u8 _pad0[12]; // 0x18
    pl::PLAYER_TYPES m_Player; // 0x24
    s32 m_Count; // 0x28
};
static_assert(sizeof(AchievementLearnSummonCntParam) == 0x2c);
static_assert(offsetof(AchievementLearnSummonCntParam, m_Flags) == 0x0);
static_assert(offsetof(AchievementLearnSummonCntParam, m_Player) == 0x24);
static_assert(offsetof(AchievementLearnSummonCntParam, m_Count) == 0x28);

struct __attribute__((packed)) AchievementCompletedMapParam {
};

struct __attribute__((packed)) AchievementOverPlayerLevelParam {
    pl::PLAYER_TYPES m_Player; // 0x0
    u32 m_Level; // 0x4
};
static_assert(sizeof(AchievementOverPlayerLevelParam) == 0x8);
static_assert(offsetof(AchievementOverPlayerLevelParam, m_Player) == 0x0);
static_assert(offsetof(AchievementOverPlayerLevelParam, m_Level) == 0x4);

struct __attribute__((packed)) AchievementGillParam {
    u32 m_Amount; // 0x0
};
static_assert(offsetof(AchievementGillParam, m_Amount) == 0x0);

struct __attribute__((packed)) AchievementMonsterKillCountParam {
    u32 m_Count; // 0x0
};
static_assert(sizeof(AchievementMonsterKillCountParam) == 0x4);
static_assert(offsetof(AchievementMonsterKillCountParam, m_Count) == 0x0);

struct __attribute__((packed)) AchievementMonsterDictionaryParam {
    u32 m_Amount; // 0x0
};
static_assert(sizeof(AchievementMonsterDictionaryParam) == 0x4);
static_assert(offsetof(AchievementMonsterDictionaryParam, m_Amount) == 0x0);

struct __attribute__((packed)) AchievementMonsterPartyKillParam {
    u32 m_MonsterPartyId; // 0x0
};
static_assert(sizeof(AchievementMonsterPartyKillParam) == 0x4);
static_assert(offsetof(AchievementMonsterPartyKillParam, m_MonsterPartyId) == 0x0);

struct __attribute__((packed)) AchievementNoCheckParam {
};

struct __attribute__((packed)) AchievementFieldParam {
    AchievementFlagParam m_Flags[2]; // 0x0
    u8 _pad0[12]; // 0x18
    s32 m_FieldType; // 0x24
    s32 m_FieldMatch1; // 0x28
    s32 m_FieldMatch2; // 0x2c
};
static_assert(sizeof(AchievementFieldParam) == 0x30);
static_assert(offsetof(AchievementFieldParam, m_Flags) == 0x0);
static_assert(offsetof(AchievementFieldParam, m_FieldType) == 0x24);
static_assert(offsetof(AchievementFieldParam, m_FieldMatch1) == 0x28);
static_assert(offsetof(AchievementFieldParam, m_FieldMatch2) == 0x2c);

struct __attribute__((packed)) AchievementCompParam {
};

struct __attribute__((packed)) AchievementParam {
    c8 m_Identifier[64]; // 0x0
    enAchievementCheckTimings m_Timing; // 0x40
    enAchievementCheckTypes m_Type; // 0x44
    union {
        s32 m_Data[62];
        AchievementFlagParam m_FlagParam[20];
        AchievementFlagRangeParam m_FlagRangeParam[14];
        AchievementHardModeBeatParam m_HardModeBeatParam;
        AchievementBeatCountParam m_BeatCountParam;
        AchievementGetItemParam m_GetItemParam[60];
        AchievementGetDecantParam m_GetDecantParam[60];
        AchievementGetDecantCntParam m_GetDecantCntParam;
        AchievementLearnSummonParam m_LearnSummonParam[30];
        AchievementLearnSummonCntParam m_LearnSummonCntParam;
        AchievementCompletedMapParam m_CompletedMapParam;
        AchievementOverPlayerLevelParam m_OverPlayerLevelParam[30];
        AchievementGillParam m_GillParam;
        AchievementMonsterKillCountParam m_MonsterKillCountParam;
        AchievementMonsterDictionaryParam m_MonsterDictionaryParam;
        AchievementMonsterPartyKillParam m_MonsterPartyKillParam[60];
        AchievementNoCheckParam m_NoCheckParam;
        AchievementFieldParam m_FieldParam;
        AchievementCompParam m_AchievementCompParam;
    }; // 0x48
};
static_assert(sizeof(AchievementParam) == 0x140);
static_assert(offsetof(AchievementParam, m_Identifier) == 0x0);
static_assert(offsetof(AchievementParam, m_Timing) == 0x40);
static_assert(offsetof(AchievementParam, m_Type) == 0x44);

struct __attribute__((packed)) ReportEntry {
    u32 m_State; // 0x0
    u8 _pad0[4]; // 0x4
    AchievementParam* m_Param; // 0x8
};
static_assert(sizeof(ReportEntry) == 0x10);
static_assert(offsetof(ReportEntry, m_State) == 0x0);
static_assert(offsetof(ReportEntry, m_Param) == 0x8);

struct __attribute__((packed)) FuncEntry {
    void* m_funcPtr; // 0x0
    uptr m_flags; // 0x8
};
static_assert(sizeof(FuncEntry) == 0x10);
static_assert(offsetof(FuncEntry, m_funcPtr) == 0x0);
static_assert(offsetof(FuncEntry, m_flags) == 0x8);

namespace AchievementChecker {
    struct __attribute__((packed)) cls {
        AchievementReporter::cls* m_AchievementReporter; // 0x0
        AchievementCheckFuncs::cls* m_AchievementCheckFuncs; // 0x8
        AchievementResource::cls* m_AchievementResource; // 0x10
    };
    static_assert(sizeof(cls) == 0x18);
    static_assert(offsetof(cls, m_AchievementReporter) == 0x0);
    static_assert(offsetof(cls, m_AchievementCheckFuncs) == 0x8);
    static_assert(offsetof(cls, m_AchievementResource) == 0x10);
}

namespace AchievementCheckFuncs {
    struct __attribute__((packed)) cls {
        void* m_AchievementResource; // 0x0
        FuncEntry m_Funcs[18]; // 0x8
        s32 m_GetDecantCntCache; // 0x128
        s32 m_MonsterKillCountCache; // 0x12c
        s32 m_MonsterDictionaryCache; // 0x130
    };
    static_assert(offsetof(cls, m_AchievementResource) == 0x0);
    static_assert(offsetof(cls, m_Funcs) == 0x8);
    static_assert(offsetof(cls, m_GetDecantCntCache) == 0x128);
    static_assert(offsetof(cls, m_MonsterKillCountCache) == 0x12c);
    static_assert(offsetof(cls, m_MonsterDictionaryCache) == 0x130);
}

namespace AchievementContext {
    struct __attribute__((packed)) cls {
    };
}

namespace AchievementReporter {
    struct __attribute__((packed)) cls {
        ReportEntry m_Entries[256]; // 0x0
        u32 m_Count; // 0x1000
        u32 m__reserved; // 0x1004
        s32 m_RetryFrames; // 0x1008
    };
    static_assert(sizeof(cls) == 0x100c);
    static_assert(offsetof(cls, m_Entries) == 0x0);
    static_assert(offsetof(cls, m_Count) == 0x1000);
    static_assert(offsetof(cls, m__reserved) == 0x1004);
    static_assert(offsetof(cls, m_RetryFrames) == 0x1008);
}

namespace AchievementResource {
    struct __attribute__((packed)) cls {
        void* m_Params; // 0x0
        s32 m_Field8; // 0x8
    };
    static_assert(offsetof(cls, m_Params) == 0x0);
    static_assert(offsetof(cls, m_Field8) == 0x8);
}

namespace btl {
    enum class DEBUG_FLAG : u32 {
        SURELY_MAX_DAMAGE = 0,
        QUICK_WAIT = 1,
        ENEMY_INVINCIBLE = 2,
        FRIEND_INVINCIBLE = 3,
        RESTART = 4,
        SURELY_ESCAPE = 5,
        SURELY_CONDITION = 6,
        TRANSFIX = 7,
        MDEF_INVALIDATION = 8,
        QUICK_EVENT = 9,
        SURELY_HIT = 10,
        SURELY_MISS = 11,
        SURELY_CRITICAL = 12,
        MONSTER_STOP_ACTION = 13,
        DAMAGE_OVER_LIMIT = 14,
        OPEN_ENEMY_HP = 15,
        PHYSICS_RANDOM_MAX = 16,
        MAGIC_RANDOM_MAX = 17,
        QUICK_TURN = 18,
        DRAW_CHAR_TOUCH_RECT = 19,
        MODEL_CHANGE_CHECK = 20,
        MODEL_CHANGE_SOUND = 21,
    };
}

namespace btl::AcquiredGoldDrawer {
    struct __attribute__((packed)) cls {
    };
}

namespace btl::BaseBattleCharacter {
    struct __attribute__((packed)) cls {
        u8 _pad0[268]; // 0x0
        bool m_IsEnemy; // 0x10c
    };
    static_assert(offsetof(cls, m_IsEnemy) == 0x10c);
}

namespace sys2d::Sprite3d {
    struct __attribute__((packed)) cls {
        u8 _pad0[232]; // 0x0
        u32 m_Flags; // 0xe8
        u8 _pad1[12]; // 0xec
        s32 m_PosX; // 0xf8
        s32 m_PosY; // 0xfc
        u8 _pad2[152]; // 0x100
    };
    static_assert(sizeof(cls) == 0x198);
    static_assert(offsetof(cls, m_Flags) == 0xe8);
    static_assert(offsetof(cls, m_PosX) == 0xf8);
    static_assert(offsetof(cls, m_PosY) == 0xfc);
}

namespace btl::Battle2DManager {
    struct __attribute__((packed)) cls {
        u8 _pad0[70440]; // 0x0
        sys2d::Sprite3d::cls m_BattlePause; // 0x11328
    };
    static_assert(offsetof(cls, m_BattlePause) == 0x11328);
}

namespace btl::Battle2DManager {
    enum class WIDGET_CTRL_TYPE : u32 {
        Pause = 0,
    };
}

namespace btl::BattleCommandSelector {
    struct __attribute__((packed)) cls {
    };
}

namespace btl::BattleDebugParameter {
    struct __attribute__((packed)) cls {
    };
}

namespace btl::BattleMonsterParty {
    struct __attribute__((packed)) cls {
    };
}

namespace btl::BattlePart {
    struct __attribute__((packed)) cls {
        u8 _pad0[36]; // 0x0
        bool m_IsPaused; // 0x24
        bool m_ShouldPause; // 0x25
    };
    static_assert(offsetof(cls, m_IsPaused) == 0x24);
    static_assert(offsetof(cls, m_ShouldPause) == 0x25);
}

namespace btl::BattlePlayer {
    struct __attribute__((packed)) cls {
    };
}

namespace btl::BattleSystem {
    struct __attribute__((packed)) cls {
    };
}

namespace btl::CBattleDisplay {
    struct __attribute__((packed)) cls {
        u8 _pad0[440]; // 0x0
        s32 m_ShakeTimer; // 0x1b8
        u8 _pad1[28]; // 0x1bc
        u32 m_ShakeAmplitudeX; // 0x1d8
        u32 m_ShakeAmplitudeY; // 0x1dc
        u32 m_ShakeAmplitudeZ; // 0x1e0
    };
    static_assert(offsetof(cls, m_ShakeTimer) == 0x1b8);
    static_assert(offsetof(cls, m_ShakeAmplitudeX) == 0x1d8);
    static_assert(offsetof(cls, m_ShakeAmplitudeY) == 0x1dc);
    static_assert(offsetof(cls, m_ShakeAmplitudeZ) == 0x1e0);
}

namespace ds::sys3d::CModelTexture {
    struct __attribute__((packed)) cls {
        void* m_Vtable; // 0x0
        void* m_VtableHandler; // 0x8
        u32 m_TexRequiredSize; // 0x10
        u32 m_Tex4x4RequiredSize; // 0x14
        u32 m_PlttRequiredSize; // 0x18
        u8 _pad0[4]; // 0x1c
        NNSG3dResFileHeader* m_RawData; // 0x20
        u32 m_TexKey; // 0x28
        u32 m_Tex4x4Key; // 0x2c
        u32 m_PlttKey; // 0x30
        u8 _pad1[4]; // 0x34
        NNSG3dResTex* m_ResTex; // 0x38
        u32 m_Flags; // 0x40
        u8 _pad2[4]; // 0x44
    };
    static_assert(sizeof(cls) == 0x48);
    static_assert(offsetof(cls, m_Vtable) == 0x0);
    static_assert(offsetof(cls, m_VtableHandler) == 0x8);
    static_assert(offsetof(cls, m_TexRequiredSize) == 0x10);
    static_assert(offsetof(cls, m_Tex4x4RequiredSize) == 0x14);
    static_assert(offsetof(cls, m_PlttRequiredSize) == 0x18);
    static_assert(offsetof(cls, m_RawData) == 0x20);
    static_assert(offsetof(cls, m_TexKey) == 0x28);
    static_assert(offsetof(cls, m_Tex4x4Key) == 0x2c);
    static_assert(offsetof(cls, m_PlttKey) == 0x30);
    static_assert(offsetof(cls, m_ResTex) == 0x38);
    static_assert(offsetof(cls, m_Flags) == 0x40);
}

namespace CTextureDataMng::Slot {
    struct __attribute__((packed)) cls {
        u8 m_Setup; // 0x0
        c8 m_Name[24]; // 0x1
        u8 _pad0[3]; // 0x19
        s32 m_RefCount; // 0x1c
        u8 _pad1[40]; // 0x20
        ds::sys3d::CModelTexture::cls m_CModelTextures; // 0x48
    };
    static_assert(sizeof(cls) == 0x90);
    static_assert(offsetof(cls, m_Setup) == 0x0);
    static_assert(offsetof(cls, m_Name) == 0x1);
    static_assert(offsetof(cls, m_RefCount) == 0x1c);
    static_assert(offsetof(cls, m_CModelTextures) == 0x48);
}

namespace CTextureDataMng {
    struct __attribute__((packed)) cls {
        // [0..35] usable, [36] sentinel
        Slot::cls m_Slots[37]; // 0x0
        s32 m_TotalBytesCounter; // 0x14d0
    };
    static_assert(offsetof(cls, m_Slots) == 0x0);
    static_assert(offsetof(cls, m_TotalBytesCounter) == 0x14d0);
}

namespace CCharacterMng {
    struct __attribute__((packed)) cls {
        u8 _pad0[7408]; // 0x0
        CTextureDataMng::cls m_CTextureDataMng; // 0x1cf0
    };
    static_assert(offsetof(cls, m_CTextureDataMng) == 0x1cf0);
}

namespace common {
    struct __attribute__((packed)) BabilMagicParameter {
    };
}

namespace common::EfficacyDataConvection {
    struct __attribute__((packed)) cls {
        // 0x00 - bit 0: initialized; bit 1: .beld loaded
        u16 m_Status; // 0x0
        u8 _pad0[6]; // 0x2
        // 0x08 - owned (ds::CHeap::alloc_app / free_app)
        void* m_RawData; // 0x8
        // 0x10 - owned (malloc_count); array of 0x10-byte nodes
        HashNode::cls* m_EffectData; // 0x10
        // 0x18 - owned (malloc_count); 127-entry bucket head array
        HashNode::cls** m_HashTable; // 0x18
    };
    static_assert(offsetof(cls, m_Status) == 0x0);
    static_assert(offsetof(cls, m_RawData) == 0x8);
    static_assert(offsetof(cls, m_EffectData) == 0x10);
    static_assert(offsetof(cls, m_HashTable) == 0x18);
}

namespace common::EfficacyDataConvection::HashNode {
    struct __attribute__((packed)) cls {
        // -> EffectData record in m_RawData
        void* m_Data; // 0x0
        // next node in bucket, or NULL
        void* m_Next; // 0x8
    };
    static_assert(sizeof(cls) == 0x10);
    static_assert(offsetof(cls, m_Data) == 0x0);
    static_assert(offsetof(cls, m_Next) == 0x8);
}

namespace debug::CharaViewerPart {
    struct __attribute__((packed)) cls {
    };
}

namespace dgs {
    struct __attribute__((packed)) DGSMSD {
    };
}

namespace ds::CPad {
    struct __attribute__((packed)) cls {
    };
}

namespace ds::CVram {
    struct __attribute__((packed)) cls {
        u8 _pad0[24]; // 0x0
        void* m_TexVramMng; // 0x18
        void* m_PlttVramMng; // 0x20
    };
    static_assert(offsetof(cls, m_TexVramMng) == 0x18);
    static_assert(offsetof(cls, m_PlttVramMng) == 0x20);
}

namespace ds::snd {
    struct __attribute__((packed)) DSSoundDesc {
    };
}

namespace ds::snd::BGMHandle {
    struct __attribute__((packed)) cls {
        void* m_NNSSndHandle; // 0x0
        bool m_pausing; // 0x8
        u8 _pad0[3]; // 0x9
        s32 m_seqNo; // 0xc
    };
    static_assert(offsetof(cls, m_NNSSndHandle) == 0x0);
    static_assert(offsetof(cls, m_pausing) == 0x8);
    static_assert(offsetof(cls, m_seqNo) == 0xc);
}

namespace ds::snd::DSSoundHeap {
    struct __attribute__((packed)) cls {
    };
}

namespace ds::snd::SEHandle {
    struct __attribute__((packed)) cls {
        void* m_NNSSndHandle; // 0x0
    };
    static_assert(offsetof(cls, m_NNSSndHandle) == 0x0);
}

namespace ds::sys3d::CAnimation {
    struct __attribute__((packed)) cls {
        u8 _pad0[32]; // 0x0
        fx32 m_Framerate; // 0x20
    };
    static_assert(offsetof(cls, m_Framerate) == 0x20);
}

namespace ds::sys3d::CAnimSet {
    enum class enTYPE : u32 {
    };
}

namespace ds::sys3d::CCamera {
    struct __attribute__((packed)) cls {
    };
}

namespace ds::sys3d::CMotSet {
    struct __attribute__((packed)) cls {
    };
}

namespace evt::EventCamera {
    struct __attribute__((packed)) cls {
    };
}

namespace FlagManager {
    struct __attribute__((packed)) cls {
        u8 m_Banks[3][1000]; // 0x0
    };
    static_assert(sizeof(cls) == 0xbb8);
    static_assert(offsetof(cls, m_Banks) == 0x0);
}

namespace Font {
    struct __attribute__((packed)) cls {
        GLuint* m_textures; // 0x0
        u8 _pad0[16]; // 0x8
        void* m_buf; // 0x18
    };
    static_assert(offsetof(cls, m_textures) == 0x0);
    static_assert(offsetof(cls, m_buf) == 0x18);
}

namespace itm {
    enum class WEAPON_SYSTEM {
    };
}

namespace itm {
    struct __attribute__((packed)) WeaponParameter {
    };
}

namespace itm {
    struct __attribute__((packed)) ProtectionParameter {
    };
}

namespace itm::EquipParameter {
    struct __attribute__((packed)) cls {
    };
}

namespace itm::ItemManager {
    struct __attribute__((packed)) cls {
        s32 m_ConsumptionCount; // 0x0
        s32 m_WeaponCount; // 0x4
        s32 m_ProtectionCount; // 0x8
        s32 m_ImportantCount; // 0xc
        // owned, freed by free()
        void* m_RawData; // 0x10
        // chain 0, stride 0x30
        void* m_Consumption; // 0x18
        // chain 1, stride 0x58
        void* m_Weapon; // 0x20
        // chain 2, stride 0x54
        void* m_Protection; // 0x28
        // chain 3, stride 0x20
        void* m_Important; // 0x30
    };
    static_assert(offsetof(cls, m_ConsumptionCount) == 0x0);
    static_assert(offsetof(cls, m_WeaponCount) == 0x4);
    static_assert(offsetof(cls, m_ProtectionCount) == 0x8);
    static_assert(offsetof(cls, m_ImportantCount) == 0xc);
    static_assert(offsetof(cls, m_RawData) == 0x10);
    static_assert(offsetof(cls, m_Consumption) == 0x18);
    static_assert(offsetof(cls, m_Weapon) == 0x20);
    static_assert(offsetof(cls, m_Protection) == 0x28);
    static_assert(offsetof(cls, m_Important) == 0x30);
}

namespace itm::PossessionItemManager::Entry {
    struct __attribute__((packed)) cls {
        s16 m_ItemId; // 0x0
        // live stack size, clamped to [0, 99]
        u8 m_Count; // 0x2
        // pending reservation, cleared by reserveCancel()
        u8 m_ReservedCount; // 0x3
    };
    static_assert(sizeof(cls) == 0x4);
    static_assert(offsetof(cls, m_ItemId) == 0x0);
    static_assert(offsetof(cls, m_Count) == 0x2);
    static_assert(offsetof(cls, m_ReservedCount) == 0x3);
}

namespace itm::PossessionItemManager {
    struct __attribute__((packed)) cls {
        // up to 384 normal items
        Entry::cls m_NormalItems[384]; // 0x0
        // live count (low 16 bits used)
        s32 m_NormalCount; // 0x600
        // up to 128 important items
        Entry::cls m_ImportantItems[128]; // 0x604
        // live count (low 16 bits used)
        s32 m_ImportantCount; // 0x804
    };
    static_assert(sizeof(cls) == 0x808);
    static_assert(offsetof(cls, m_NormalItems) == 0x0);
    static_assert(offsetof(cls, m_NormalCount) == 0x600);
    static_assert(offsetof(cls, m_ImportantItems) == 0x604);
    static_assert(offsetof(cls, m_ImportantCount) == 0x804);
}

namespace mon::MonsterManager {
    struct __attribute__((packed)) cls {
        // owned, freed by free()
        void* m_RawData; // 0x0
        // chain 0,  stride 0x98, id at +0x08 (s16)
        void* m_MonsterParameter; // 0x8
        // chain 1,  stride 0x0a, id at +0x00 (s16)
        void* m_DropItem; // 0x10
        // chain 2,  stride 0x1c, indexed (not id-keyed)
        void* m_NormalAttack; // 0x18
        // chain 4,  stride 0x54, id at +0x00 (s16)
        void* m_Offset; // 0x20
        // chain 5,  stride 0x44, id at +0x00 (s16)
        void* m_EffectsInfoAlt; // 0x28
        // chain 11, stride 0x48, id at +0x00 (s16), sub at +0x44 (u16)
        void* m_EffectsInfo; // 0x30
        // chain 6,  stride 0x1c, id0 at +0x00 (u8), id1 at +0x01 (u8)
        void* m_OctmanmosLegInfo; // 0x38
        // chain 8,  stride 0x2c, id at +0x00 (s16)
        void* m_TurnAction; // 0x40
        // chain 9,  stride 0x0c, id at +0x00 (s16)
        void* m_ActionCondition; // 0x48
        // chain 10, stride 0x0e, id at +0x00 (s16)
        void* m_Counter; // 0x50
        // chain 7,  stride 0x16, id at +0x00 (s16)
        void* m_Ai; // 0x58
        u32 m_TurnActionCount; // 0x60
        u32 m_ActionConditionCount; // 0x64
        u32 m_CounterCount; // 0x68
        u32 m_AiCount; // 0x6c
        u32 m_MonsterParameterCount; // 0x70
        u32 m_DropItemCount; // 0x74
        u32 m_NormalAttackCount; // 0x78
        u8 _pad0[4]; // 0x7c
        // chain 3 (unused)
        u32 m_OffsetCount; // 0x80
        u32 m_EffectsInfoAltCount; // 0x84
        u32 m_EffectsInfoCount; // 0x88
        u32 m_OctmanmosLegInfoCount; // 0x8c
    };
    static_assert(sizeof(cls) == 0x90);
    static_assert(offsetof(cls, m_RawData) == 0x0);
    static_assert(offsetof(cls, m_MonsterParameter) == 0x8);
    static_assert(offsetof(cls, m_DropItem) == 0x10);
    static_assert(offsetof(cls, m_NormalAttack) == 0x18);
    static_assert(offsetof(cls, m_Offset) == 0x20);
    static_assert(offsetof(cls, m_EffectsInfoAlt) == 0x28);
    static_assert(offsetof(cls, m_EffectsInfo) == 0x30);
    static_assert(offsetof(cls, m_OctmanmosLegInfo) == 0x38);
    static_assert(offsetof(cls, m_TurnAction) == 0x40);
    static_assert(offsetof(cls, m_ActionCondition) == 0x48);
    static_assert(offsetof(cls, m_Counter) == 0x50);
    static_assert(offsetof(cls, m_Ai) == 0x58);
    static_assert(offsetof(cls, m_TurnActionCount) == 0x60);
    static_assert(offsetof(cls, m_ActionConditionCount) == 0x64);
    static_assert(offsetof(cls, m_CounterCount) == 0x68);
    static_assert(offsetof(cls, m_AiCount) == 0x6c);
    static_assert(offsetof(cls, m_MonsterParameterCount) == 0x70);
    static_assert(offsetof(cls, m_DropItemCount) == 0x74);
    static_assert(offsetof(cls, m_NormalAttackCount) == 0x78);
    static_assert(offsetof(cls, m_OffsetCount) == 0x80);
    static_assert(offsetof(cls, m_EffectsInfoAltCount) == 0x84);
    static_assert(offsetof(cls, m_EffectsInfoCount) == 0x88);
    static_assert(offsetof(cls, m_OctmanmosLegInfoCount) == 0x8c);
}

namespace mon::MonsterManiaManager {
    struct __attribute__((packed)) cls {
        MonsterMania m_Entries[256]; // 0x0
    };
    static_assert(sizeof(cls) == 0x200);
    static_assert(offsetof(cls, m_Entries) == 0x0);
}

namespace mr::MonsterRefDataManager {
    struct __attribute__((packed)) cls {
        // owned, freed by free()
        void* m_RawData; // 0x0
        u32 m_Count; // 0x8
        u8 _pad0[4]; // 0xc
    };
    static_assert(sizeof(cls) == 0x10);
    static_assert(offsetof(cls, m_RawData) == 0x0);
    static_assert(offsetof(cls, m_Count) == 0x8);
}

namespace msg {
    enum class CANVAS : u64 {
    };
}

namespace object::CharacterObject {
    struct __attribute__((packed)) cls {
        u8 _pad0[48]; // 0x0
        VecFx32 m_position; // 0x30
        u8 _pad1[52]; // 0x3c
        s32 m_cid; // 0x70
        u8 _pad2[16]; // 0x74
        s32 m_rotX; // 0x84
        s32 m_rotY; // 0x88
        s32 m_rotZ; // 0x8c
        u8 _pad3[392]; // 0x90
        s32 m_cid2; // 0x218
    };
    static_assert(offsetof(cls, m_position) == 0x30);
    static_assert(offsetof(cls, m_cid) == 0x70);
    static_assert(offsetof(cls, m_rotX) == 0x84);
    static_assert(offsetof(cls, m_rotY) == 0x88);
    static_assert(offsetof(cls, m_rotZ) == 0x8c);
    static_assert(offsetof(cls, m_cid2) == 0x218);
}

namespace part {
    enum class GAMEPART : u32 {
        DebugPart = 0,
        CompanyLogoPart = 1,
        MoviePart = 2,
        TitlePart = 3,
        WorldPart = 6,
        BattlePart = 8,
        BackupRomFormatPart = 23,
        SoundDebugPart = 32,
        SoundPlayerPart = 33,
    };
}

namespace title::TitleSubState {
    struct __attribute__((packed)) cls {
        TitleContents::cls* m_TitleContents; // 0x0
        u8 _pad0[4]; // 0x8
        u32 m_HoveredIdx; // 0xc
    };
    static_assert(offsetof(cls, m_TitleContents) == 0x0);
    static_assert(offsetof(cls, m_HoveredIdx) == 0xc);
}

namespace part::TitlePart {
    struct __attribute__((packed)) cls {
        u8 _pad0[16]; // 0x0
        title::TitleSubState::cls m_TitleSubState; // 0x10
    };
    static_assert(offsetof(cls, m_TitleSubState) == 0x10);
}

namespace world::WorldCamera {
    struct __attribute__((packed)) cls {
    };
}

namespace world::WorldStateContext {
    struct __attribute__((packed)) cls {
        u8 _pad0[96]; // 0x0
        WorldCamera::cls m_WorldCamera; // 0x60
        u8 _pad1[255]; // 0x61
        object::CharacterObject::cls m_PlayerCharacter; // 0x160
    };
    static_assert(offsetof(cls, m_WorldCamera) == 0x60);
    static_assert(offsetof(cls, m_PlayerCharacter) == 0x160);
}

namespace part::WorldPart {
    struct __attribute__((packed)) cls {
        u8 _pad0[1088]; // 0x0
        world::WorldStateContext::cls m_WorldStateContext; // 0x440
    };
    static_assert(offsetof(cls, m_WorldStateContext) == 0x440);
}

namespace pl {
    enum class PARTY_FORMATION_TYPES : u8 {
    };
}

namespace pl {
    enum class HAND_TYPE : u8 {
    };
}

namespace pl {
    enum class EQUIP_POINTS : u8 {
    };
}

namespace pl::LearningAbilityManager {
    struct __attribute__((packed)) cls {
        // per-category bit-array pointers into Contents
        // [0]→+0x00 (32B) [1]→+0x20 (4B) [2]→+0x24 (5B)
        // [3]→+0x29 (5B)  [4]→+0x2e (2B) [5]→+0x30 (2B)
        u8* m_Flags[6]; // 0x0
    };
    static_assert(sizeof(cls) == 0x30);
    static_assert(offsetof(cls, m_Flags) == 0x0);
}

namespace pl::LearningAbilityManager {
    enum class LEARNING_ABILITY_TYPES : u32 {
    };
}

namespace pl::LearningAbilityManagerContents {
    struct __attribute__((packed)) cls {
        // 256 bits, includes the 5 magic-type ability bits
        u8 m_FlagsType0[32]; // 0x0
        // 32  bits
        u8 m_FlagsType1[4]; // 0x20
        // 40  bits
        u8 m_FlagsType2[5]; // 0x24
        // 40  bits
        u8 m_FlagsType3[5]; // 0x29
        // 16  bits
        u8 m_FlagsType4[2]; // 0x2e
        // 16  bits
        u8 m_FlagsType5[2]; // 0x30
    };
    static_assert(sizeof(cls) == 0x32);
    static_assert(offsetof(cls, m_FlagsType0) == 0x0);
    static_assert(offsetof(cls, m_FlagsType1) == 0x20);
    static_assert(offsetof(cls, m_FlagsType2) == 0x24);
    static_assert(offsetof(cls, m_FlagsType3) == 0x29);
    static_assert(offsetof(cls, m_FlagsType4) == 0x2e);
    static_assert(offsetof(cls, m_FlagsType5) == 0x30);
}

namespace ys::BodyParameter {
    struct __attribute__((packed)) cls {
        u8 m_Strength; // 0x0
        u8 m_Speed; // 0x1
        u8 m_Stamina; // 0x2
        u8 m_Intellect; // 0x3
        u8 m_Spirit; // 0x4
        u8 _pad0[1]; // 0x5
        s16 m_MaxMP; // 0x6
    };
    static_assert(offsetof(cls, m_Strength) == 0x0);
    static_assert(offsetof(cls, m_Speed) == 0x1);
    static_assert(offsetof(cls, m_Stamina) == 0x2);
    static_assert(offsetof(cls, m_Intellect) == 0x3);
    static_assert(offsetof(cls, m_Spirit) == 0x4);
    static_assert(offsetof(cls, m_MaxMP) == 0x6);
}

namespace pl::Player {
    struct __attribute__((packed)) cls {
        // slot active flag
        u8 m_Enabled; // 0x0
        u8 _pad0[7]; // 0x1
        // subobject; playerAbilityManager() returns &this
        u8 m_PlayerAbilityManager[260]; // 0x8
        PLAYER_TYPES m_PlayerType; // 0x10c
        ys::BodyParameter::cls m_BodyParameter; // 0x110
        // PlayerEquipParameter; equipParameter() returns &this
        u8 m_PlayerEquipParameter[8]; // 0x118
        // s16[5] of equipment ids
        s16* m_EquipmentItems; // 0x120
        // EquipmentMagicBinder subobject
        u8 m_EquipmentMagicBinder[80]; // 0x128
        // subobject (setPhysicsAttack / updateParameter)
        u8 m_PhysicsAttackParam[44]; // 0x178
        // subobject (calcPhysicsDefense)
        u8 m_PhysicsDefenseParam[18]; // 0x1a4
        // 5 clamped stat bytes (STR/AGI/VIT/LUCK/INT)
        u8 m_Stats[5]; // 0x1b6
        u8 _pad1[1]; // 0x1bb
        s16 m_MagicDefense; // 0x1bc
        // magicDefensePower() returns (s32)this
        s16 m_MagicDefensePower; // 0x1be
        u8 _pad2[4]; // 0x1c0
        // character id; -1 when unassigned
        s32 m_cid; // 0x1c4
        // PlayerEquipmentSymbol[HAND_TYPE]
        u8 m_EquipmentSymbol[2][28]; // 0x1c8
        // LearningAbilityManager; learningAbility() returns &this
        u8 m_LearningAbilityManager[48]; // 0x200
    };
    static_assert(sizeof(cls) == 0x230);
    static_assert(offsetof(cls, m_Enabled) == 0x0);
    static_assert(offsetof(cls, m_PlayerAbilityManager) == 0x8);
    static_assert(offsetof(cls, m_PlayerType) == 0x10c);
    static_assert(offsetof(cls, m_BodyParameter) == 0x110);
    static_assert(offsetof(cls, m_PlayerEquipParameter) == 0x118);
    static_assert(offsetof(cls, m_EquipmentItems) == 0x120);
    static_assert(offsetof(cls, m_EquipmentMagicBinder) == 0x128);
    static_assert(offsetof(cls, m_PhysicsAttackParam) == 0x178);
    static_assert(offsetof(cls, m_PhysicsDefenseParam) == 0x1a4);
    static_assert(offsetof(cls, m_Stats) == 0x1b6);
    static_assert(offsetof(cls, m_MagicDefense) == 0x1bc);
    static_assert(offsetof(cls, m_MagicDefensePower) == 0x1be);
    static_assert(offsetof(cls, m_cid) == 0x1c4);
    static_assert(offsetof(cls, m_EquipmentSymbol) == 0x1c8);
    static_assert(offsetof(cls, m_LearningAbilityManager) == 0x200);
}

namespace pl::PlayerParty::BattleCommand {
    struct __attribute__((packed)) cls {
        u32 m_Field0; // 0x0
        u64 m_StartCommand; // 0x4
    };
    static_assert(sizeof(cls) == 0xc);
    static_assert(offsetof(cls, m_Field0) == 0x0);
    static_assert(offsetof(cls, m_StartCommand) == 0x4);
}

namespace pl::PlayerParty {
    struct __attribute__((packed)) cls {
        // Holds 15 inline Players (stride 0x230 from +0x50), a PlayerSummon, a
        // CurrentAbilityIDList, save/active-member pointer pairs, chain-loaded lookup
        // tables, and 5 BattleCommands. Sub-objects with unknown layouts are raw byte
        // arrays so the static_asserts below stay verifiable.
        Player::cls* m_SaveComposition[5]; // 0x0
        // active party, indexed 0..4 by "order"
        Player::cls* m_Members[5]; // 0x28
        // 15 inline Players;
        // m_Id offsets: 0x15c, 0x38c, 0x5bc, 0x7ec,
        // 0xa1c, 0xc4c, 0xe7c, 0x10ac, 0x12dc, 0x150c,
        // 0x173c, 0x196c, 0x1b9c, 0x1dcc, 0x1ffc
        Player::cls m_Players[15]; // 0x50
        u8 m_PlayerSummon[40]; // 0x2120
        u8 m_CurrentAbilityIDList[152]; // 0x2148
        // owned, freed by free()
        void* m_RawChainData; // 0x21e0
        // chain 0 (unused by dumped funcs)
        void* m_LearnChain0; // 0x21e8
        // chain 1, stride 0x14
        void* m_NormalAttack; // 0x21f0
        // chain 0x20, stride 0x20
        void* m_NormalMagic; // 0x21f8
        s16 m_NormalMagicCount; // 0x2200
        u8 _pad0[6]; // 0x2202
        // chains 2..0x10, per PLAYER_TYPES
        void* m_LearnAbilityData[15]; // 0x2208
        // chain 0x21, stride 8, 0xe68 bytes
        void* m_AbilityList; // 0x2280
        // chain 0x22, stride 0x6c
        void* m_LayoutSceneParameter; // 0x2288
        s32 m_LayoutSceneParameterCount; // 0x2290
        u8 _pad1[4]; // 0x2294
        // chain 0x23, stride 0x30
        void* m_WeaponOffsetParameter; // 0x2298
        // chains 0x11..0x1f, per PLAYER_TYPES
        void* m_LearnAbilityLookup[15]; // 0x22a0
        s16 m_LearnAbilityCount[15]; // 0x2318
        u8 _pad2[2]; // 0x2336
        // ceil(rawSize/0x14)
        u32 m_NormalAttackCount; // 0x2338
        u8 _pad3[4]; // 0x233c
        // chain 0x24, stride 0xa
        void* m_AbilityBounsTable; // 0x2340
        // ceil(rawSize/0xa)
        u32 m_AbilityBounsCount; // 0x2348
        BattleCommand::cls m_BattleCommands[5]; // 0x234c
    };
    static_assert(offsetof(cls, m_SaveComposition) == 0x0);
    static_assert(offsetof(cls, m_Members) == 0x28);
    static_assert(offsetof(cls, m_Players) == 0x50);
    static_assert(offsetof(cls, m_PlayerSummon) == 0x2120);
    static_assert(offsetof(cls, m_CurrentAbilityIDList) == 0x2148);
    static_assert(offsetof(cls, m_RawChainData) == 0x21e0);
    static_assert(offsetof(cls, m_LearnChain0) == 0x21e8);
    static_assert(offsetof(cls, m_NormalAttack) == 0x21f0);
    static_assert(offsetof(cls, m_NormalMagic) == 0x21f8);
    static_assert(offsetof(cls, m_NormalMagicCount) == 0x2200);
    static_assert(offsetof(cls, m_LearnAbilityData) == 0x2208);
    static_assert(offsetof(cls, m_AbilityList) == 0x2280);
    static_assert(offsetof(cls, m_LayoutSceneParameter) == 0x2288);
    static_assert(offsetof(cls, m_LayoutSceneParameterCount) == 0x2290);
    static_assert(offsetof(cls, m_WeaponOffsetParameter) == 0x2298);
    static_assert(offsetof(cls, m_LearnAbilityLookup) == 0x22a0);
    static_assert(offsetof(cls, m_LearnAbilityCount) == 0x2318);
    static_assert(offsetof(cls, m_NormalAttackCount) == 0x2338);
    static_assert(offsetof(cls, m_AbilityBounsTable) == 0x2340);
    static_assert(offsetof(cls, m_AbilityBounsCount) == 0x2348);
    static_assert(offsetof(cls, m_BattleCommands) == 0x234c);
}

namespace pl::SummonAbilityList {
    struct __attribute__((packed)) cls {
    };
}

namespace pl::SummonAbilityList {
    enum class TYPES : u32 {
    };
}

namespace ScriptEngine {
    struct __attribute__((packed)) cls {
    };
}

namespace sys::GameParameter {
    struct __attribute__((packed)) cls {
        u8 _pad0[148]; // 0x0
        u16 m_flags; // 0x94
    };
    static_assert(offsetof(cls, m_flags) == 0x94);
}

namespace sys::GameParameter::PlayerSaveParameter {
    struct __attribute__((packed)) cls {
        u8 _pad0[438]; // 0x0
        ys::BodyParameter::cls m_BodyParameter; // 0x1b6
    };
    static_assert(offsetof(cls, m_BodyParameter) == 0x1b6);
}

namespace title::Title2Ds {
    struct __attribute__((packed)) cls {
        u8 _pad0[336]; // 0x0
        sys2d::Sprite3d::cls m_Continue; // 0x150
        sys2d::Sprite3d::cls m_NewGame; // 0x2e8
        sys2d::Sprite3d::cls m_LoadGame; // 0x480
        u8 _pad1[816]; // 0x618
        sys2d::Sprite3d::cls m_FF4TAY; // 0x948
        sys2d::Sprite3d::cls m_CloudSave; // 0xae0
        sys2d::Sprite3d::cls m_PrivacyPolicy; // 0xc78
        sys2d::Sprite3d::cls m_GooglePlay; // 0xe10
        sys2d::Sprite3d::cls m_Achievements; // 0xfa8
        sys2d::Sprite3d::cls m_SQEX; // 0x1140
        u8 _pad2[624]; // 0x12d8
        u32 m_ContentMask; // 0x1548
        u32 m_FocusedIndex; // 0x154c
    };
    static_assert(offsetof(cls, m_Continue) == 0x150);
    static_assert(offsetof(cls, m_NewGame) == 0x2e8);
    static_assert(offsetof(cls, m_LoadGame) == 0x480);
    static_assert(offsetof(cls, m_FF4TAY) == 0x948);
    static_assert(offsetof(cls, m_CloudSave) == 0xae0);
    static_assert(offsetof(cls, m_PrivacyPolicy) == 0xc78);
    static_assert(offsetof(cls, m_GooglePlay) == 0xe10);
    static_assert(offsetof(cls, m_Achievements) == 0xfa8);
    static_assert(offsetof(cls, m_SQEX) == 0x1140);
    static_assert(offsetof(cls, m_ContentMask) == 0x1548);
    static_assert(offsetof(cls, m_FocusedIndex) == 0x154c);
}

namespace title::TitleContents {
    struct __attribute__((packed)) cls {
        u8 _pad0[8]; // 0x0
        Title2Ds::cls m_Title2Ds; // 0x8
    };
    static_assert(offsetof(cls, m_Title2Ds) == 0x8);
}

namespace ui::CWidgetMng {
    struct __attribute__((packed)) cls {
        // hash table of widget chains, bucket = id & 0x3f
        Widget::cls* m_WidgetTable[64]; // 0x0
        // source data ptr for type-4 sprite widgets (addWidget); cleared in init/terminate
        void* m_field_0x200; // 0x200
        // source data ptr for type-5/6 sprite widgets (addWidget); cleared in init/terminate
        void* m_field_0x208; // 0x208
        // widget id currently selected/pressed (confirmed against the
        // raw disassembly; Ghidra's pseudocode mislabels this field).
        s32 m_SelectedElement; // 0x210
        // scratch state shared by moveWidget (previous-frame hover id)
        // and moveSlide (next slide-state value); same 4 bytes, two uses.
        s32 m_field_0x214; // 0x214
        // write-only from these 31 functions; mirrors m_SelectedElement
        // whenever it changes, presumably read by code outside CWidgetMng.
        s32 m_FocusedElement; // 0x218
        // write-only from these 31 functions; same mirroring as field_0x218.
        s32 m_field_0x21c; // 0x21c
        // frame counter for press-and-hold on m_SelectedElement
        s32 m_SelectedHoldFrames; // 0x220
        // touch-anchor widget id snapshot
        u32 m_field_0x224; // 0x224
        // previous touch x
        s16 m_field_0x228; // 0x228
        // previous touch y
        s16 m_field_0x22a; // 0x22a
        // touch-down x
        s16 m_field_0x22c; // 0x22c
        // touch-down y
        s16 m_field_0x22e; // 0x22e
        // touch panel was down last frame
        bool m_WasTouching; // 0x230
        // set via setDragState
        bool m_DragState; // 0x231
        u8 _pad0[2]; // 0x232
        // moveSlide() state machine state
        s32 m_SlideState; // 0x234
        // current slide offset (signed, animates back to 0)
        s32 m_SlideOffset; // 0x238
        // +1/-1 direction multiplier for the active slide
        s32 m_SlideDirection; // 0x23c
        // x passed to NNS_G2dSetSlide, set by setSlide
        s32 m_SlideX; // 0x240
        // y passed to NNS_G2dSetSlide, set by setSlide
        s32 m_SlideY; // 0x244
        // touch x sampled when the slide drag began
        s16 m_TouchAnchorX; // 0x248
        // touch y sampled when the slide drag began
        s16 m_TouchAnchorY; // 0x24a
        // current scroll offset, set by setScroll(Pos/Rect)
        s32 m_ScrollPos; // 0x24c
        // max scrollable extent (setScroll param_3 / setScrollRect param_5)
        s32 m_ScrollMax; // 0x250
        // x passed to NNS_G2dSetScroll
        s32 m_ScrollX; // 0x254
        // y passed to NNS_G2dSetScroll
        s32 m_ScrollY; // 0x258
        // extra scroll-rect width, only set by setScrollRect
        s32 m_field_0x25c; // 0x25c
        // viewport height, set by setScroll(param_2)/setScrollRect(param_4)
        s32 m_ScrollHeight; // 0x260
        // scroll/select animation inertia value
        float m_ScrollVelocity; // 0x264
        // indexed directly by cursor id (1..6); slot 0 is unused padding,
        // so the array really spans ids 0..6 and id 6's slot is the same
        // 4 bytes as m_ListMax[0] below (harmless aliasing in the original).
        s32 m_CursorItem[6]; // 0x268
        // set/read via setListMax/getListMax; m_ListMax[0] aliases the
        // cursor-id-6 slot above (same bytes, reused by the original code)
        s32 m_ListMax[2]; // 0x280
        // gates the pad-driven selection shortcut in moveWidget
        bool m_PadCheckEnable; // 0x288
        u8 _pad1[3]; // 0x289
        // pad bitmask checked (with 0x10000) to jump to active cursor item
        u32 m_PadMaskSelect; // 0x28c
        // pad bitmask checked for the "no selection yet" shortcut
        u32 m_PadMaskCancel; // 0x290
        u8 _pad2[4]; // 0x294
        // per-frame pad callback, default set by setDefaultPadKeyFunc
        uptr m_PadKeyFunc; // 0x298
        // argument passed to m_PadKeyFunc
        s32 m_PadKeyFuncArg; // 0x2a0
    };
    static_assert(offsetof(cls, m_WidgetTable) == 0x0);
    static_assert(offsetof(cls, m_field_0x200) == 0x200);
    static_assert(offsetof(cls, m_field_0x208) == 0x208);
    static_assert(offsetof(cls, m_SelectedElement) == 0x210);
    static_assert(offsetof(cls, m_field_0x214) == 0x214);
    static_assert(offsetof(cls, m_FocusedElement) == 0x218);
    static_assert(offsetof(cls, m_field_0x21c) == 0x21c);
    static_assert(offsetof(cls, m_SelectedHoldFrames) == 0x220);
    static_assert(offsetof(cls, m_field_0x224) == 0x224);
    static_assert(offsetof(cls, m_field_0x228) == 0x228);
    static_assert(offsetof(cls, m_field_0x22a) == 0x22a);
    static_assert(offsetof(cls, m_field_0x22c) == 0x22c);
    static_assert(offsetof(cls, m_field_0x22e) == 0x22e);
    static_assert(offsetof(cls, m_WasTouching) == 0x230);
    static_assert(offsetof(cls, m_DragState) == 0x231);
    static_assert(offsetof(cls, m_SlideState) == 0x234);
    static_assert(offsetof(cls, m_SlideOffset) == 0x238);
    static_assert(offsetof(cls, m_SlideDirection) == 0x23c);
    static_assert(offsetof(cls, m_SlideX) == 0x240);
    static_assert(offsetof(cls, m_SlideY) == 0x244);
    static_assert(offsetof(cls, m_TouchAnchorX) == 0x248);
    static_assert(offsetof(cls, m_TouchAnchorY) == 0x24a);
    static_assert(offsetof(cls, m_ScrollPos) == 0x24c);
    static_assert(offsetof(cls, m_ScrollMax) == 0x250);
    static_assert(offsetof(cls, m_ScrollX) == 0x254);
    static_assert(offsetof(cls, m_ScrollY) == 0x258);
    static_assert(offsetof(cls, m_field_0x25c) == 0x25c);
    static_assert(offsetof(cls, m_ScrollHeight) == 0x260);
    static_assert(offsetof(cls, m_ScrollVelocity) == 0x264);
    static_assert(offsetof(cls, m_CursorItem) == 0x268);
    static_assert(offsetof(cls, m_ListMax) == 0x280);
    static_assert(offsetof(cls, m_PadCheckEnable) == 0x288);
    static_assert(offsetof(cls, m_PadMaskSelect) == 0x28c);
    static_assert(offsetof(cls, m_PadMaskCancel) == 0x290);
    static_assert(offsetof(cls, m_PadKeyFunc) == 0x298);
    static_assert(offsetof(cls, m_PadKeyFuncArg) == 0x2a0);
}

namespace ui::Widget {
    struct __attribute__((packed)) cls {
        u32 m_id; // 0x0
        s32 m_x; // 0x4
        s32 m_y; // 0x8
        s32 m_width; // 0xc
        s32 m_height; // 0x10
        s32 m_textColor; // 0x14
        s32 m_msd; // 0x18
        // bit0..7: widget "type" (switch in drawWidget); 0x200: scroll-relative;
        // 0x400: alt size variant; 0x800: alt hilight style; 0x1000: disabled
        // (setWidgetEnable); 0x2000: hilighted/selected (setWidgetHilight);
        // 0x4000: dgs msd-name special case (addWidget)
        u32 m_flags; // 0x1c
        // menu::BasicWindow*, only allocated for type 3 widgets
        void* m_basicWindow; // 0x20
        // sys2d::Sprite*/Sprite3d*, allocated for type 4/5/6 widgets
        void* m_sprite; // 0x28
        // next node in this bucket's chain
        Widget::cls* m_next; // 0x30
    };
    static_assert(sizeof(cls) == 0x38);
    static_assert(offsetof(cls, m_id) == 0x0);
    static_assert(offsetof(cls, m_x) == 0x4);
    static_assert(offsetof(cls, m_y) == 0x8);
    static_assert(offsetof(cls, m_width) == 0xc);
    static_assert(offsetof(cls, m_height) == 0x10);
    static_assert(offsetof(cls, m_textColor) == 0x14);
    static_assert(offsetof(cls, m_msd) == 0x18);
    static_assert(offsetof(cls, m_flags) == 0x1c);
    static_assert(offsetof(cls, m_basicWindow) == 0x20);
    static_assert(offsetof(cls, m_sprite) == 0x28);
    static_assert(offsetof(cls, m_next) == 0x30);
}

namespace world {
    enum class VIBRATION_STATE : u32 {
        NONE = 0,
        A = 1,
        B = 2,
    };
}

namespace world::CCameraVibration {
    struct __attribute__((packed)) cls {
        s32 m_State; // 0x0
        u8 _pad0[4]; // 0x4
        s32 m_CurrentTime; // 0x8
        s32 m_TotalTime; // 0xc
        u8 _pad1[8]; // 0x10
        VecFx32 m_Amplitude; // 0x18
        VecFx32 m_LastPosition; // 0x24
        VecFx32 m_Offset; // 0x30
    };
    static_assert(offsetof(cls, m_State) == 0x0);
    static_assert(offsetof(cls, m_CurrentTime) == 0x8);
    static_assert(offsetof(cls, m_TotalTime) == 0xc);
    static_assert(offsetof(cls, m_Amplitude) == 0x18);
    static_assert(offsetof(cls, m_LastPosition) == 0x24);
    static_assert(offsetof(cls, m_Offset) == 0x30);
}

namespace world::MSSConfig {
    struct __attribute__((packed)) cls {
    };
}

namespace world::WorldState {
    struct __attribute__((packed)) cls {
    };
}

namespace world::WorldStateScheduler {
    struct __attribute__((packed)) cls {
    };
}

namespace world::WSMove {
    struct __attribute__((packed)) cls {
    };
}

namespace world::WSVehicleMove {
    struct __attribute__((packed)) cls {
        u8 _pad0[80]; // 0x0
        WorldStateScheduler::cls* m_WSS; // 0x50
        u8 _pad1[20]; // 0x58
        bool m_m_bool; // 0x6c
    };
    static_assert(offsetof(cls, m_WSS) == 0x50);
    static_assert(offsetof(cls, m_m_bool) == 0x6c);
}

namespace ys {
    struct __attribute__((packed)) PhysicsAttackParameter {
    };
}

namespace ys {
    struct __attribute__((packed)) PhysicsDefenseParameter {
    };
}

namespace ys {
    struct __attribute__((packed)) MagicDefenseParameter {
    };
}

namespace ys::Condition {
    struct __attribute__((packed)) cls {
    };
}

extern void (*babilCommand_CE_CameraPos)(ScriptEngine::cls& scriptEngine);
extern void (*babilCommand_CE_SetupCameraMotion)(ScriptEngine::cls& scriptEngine);
extern void (*babilCommand_ClearCountJump)(ScriptEngine::cls& scriptEngine);
extern s32 (*CoreAudioOutInit)(s32, s32, uptr callback);
extern s32 (*CoreAudioOutKick)(void);
extern s32 (*CoreAudioOutQueueBuffer)(void* buffer, size_t size);
extern void (*CoreAudioOutRelease)(void);
extern s32 (*CoreAudioOutResume)(void);
extern s32 (*CoreAudioOutSuspend)(void);
extern void (*free_count)(void*);
extern fx32 (*FX_Div)(fx32 frame, fx32 framerate);
extern void (*G2_SetWnd0Position)(s32 x1, s32 y1, s32 x2, s32 y2);
extern void (*GX_SetWideMode)(void*, bool wide);
extern void (*initApp)(void* env);
extern void* (*malloc_count)(size_t);
extern void (*NNS_G2dTickCellAnimation)(NNSG2dCellAnimation* pCellAnim, fx32 frames);
extern void (*NNS_SndPlayerSetPlayerVolume)(s32 playerNo, s32 volume);
extern void (*NNS_SndPlayerStopSeqBySeqArcIdx)(s32 seqArcNo, s32 index, s32 frames);
extern void (*NNS_SndUpdate)(void);
extern void (*OS_Printf)(const c8* fmt, ...);
extern void (*OSi_Panic)(const c8* file, s32 line, const c8* msg, ...);
extern s32 (*render)(s8*, void*, s32);
extern s32 (*touch)(s32, s32, s32, s32, float, float, float, float);
extern CCharacterMng::cls* characterMng;
extern s32* fontScale;
extern s8** g_env;
extern u32* LCD_HEIGHT;
extern u32* LCD_WIDTH;
extern u32* memCount;
extern u32* texCount;

namespace AchievementChecker {
    extern void (*Ctor)(cls* self);
    extern void (*Dtor)(cls* self);
    extern void (*init)(cls* self, AchievementReporter::cls* reporter, AchievementResource::cls* resource, AchievementCheckFuncs::cls* checkFuncs);
    extern void (*update)(cls* self, enAchievementCheckTimings timing);
}

namespace AchievementCheckFuncs {
    extern u32 (*achievementComp)(cls* self, AchievementParam* param);
    extern u32 (*beatCount)(cls* self, AchievementParam* param);
    extern u32 (*callFunc)(cls* self, AchievementParam* param);
    extern void (*clearCacheParam)(cls* self);
    extern u32 (*completedMap)(cls* self, AchievementParam* param);
    extern void (*Ctor)(cls* self);
    extern void (*Dtor)(cls* self);
    extern u32 (*field)(cls* self, AchievementParam* param);
    extern u32 (*flag)(cls* self, AchievementParam* param);
    extern u32 (*flagRange)(cls* self, AchievementParam* param);
    extern u32 (*getDecant)(cls* self, AchievementParam* param);
    extern u32 (*getDecantCnt)(cls* self, AchievementParam* param);
    extern void (*getDecantItemName)(s32 itemId, c8* outBuf);
    extern u32 (*getItem)(cls* self, AchievementParam* param);
    extern u32 (*gill)(cls* self, AchievementParam* param);
    extern u32 (*hardModeBeat)(cls* self, AchievementParam* param);
    extern void (*init)(cls* self, AchievementResource::cls* resource);
    extern u32 (*learnSummon)(cls* self, AchievementParam* param);
    extern u32 (*learnSummonCnt)(cls* self, AchievementParam* param);
    extern u32 (*monsterDictionary)(cls* self, AchievementParam* param);
    extern u32 (*monsterKillCount)(cls* self, AchievementParam* param);
    extern u32 (*monsterKillParty)(cls* self, AchievementParam* param);
    extern u32 (*noCheck)(cls* self, AchievementParam* param);
    extern u32 (*overPlayerLevel)(cls* self, AchievementParam* param);
    extern s32* DecantItemTbl;
}

namespace AchievementContext {
    extern u8* (*getArchiveReportArray)(cls* self);
    extern cls* instance_;
}

namespace AchievementReporter {
    extern void (*addParam)(cls* self, AchievementParam& param);
    extern void (*Ctor)(cls* self);
    extern void (*Dtor)(cls* self);
    extern void (*eraseReported)(cls* self);
    extern s32 (*finish)(cls* self, const c8* identifier);
    extern void (*init)(cls* self);
    extern bool (*isAllReported)(const cls* self);
    extern bool (*isReportedSecondReport)(cls* self);
    extern void (*syncSaveData)(cls* self, AchievementResource::cls* resource);
    extern void (*update)(cls* self);
}

namespace btl {
    extern u32 (*battleSpeedRate)(void);

    namespace AcquiredGoldDrawer {
        extern void (*update)(cls* self);
    }

    namespace BaseBattleCharacter {
        extern bool (*isInvincible)(cls* self);
    }

    namespace Battle2DManager {
        extern bool (*ctrlWidgetCheck)(cls* self, WIDGET_CTRL_TYPE type);
        extern cls* instance_;
    }

    namespace BattleCommandSelector {
        extern u32 (*commandAction)(cls* self, btl::BattleSystem::cls& system, s32, common::ABILITY_ID abilityId, s8);
        extern void (*initialize)(cls* self, btl::BattleSystem::cls& system, btl::BattlePlayer::cls* player);
    }

    namespace BattleDebugParameter {
        extern bool (*flag)(cls* self, DEBUG_FLAG flag);
    }

    namespace BattleMonsterParty {
        extern u32 (*giftExp)(cls* self);
        extern u32 (*giftGold)(cls* self);
    }

    namespace BattlePart {
        extern void (*doInitialize)(cls* self);
        extern cls* instance_;
    }

    namespace BattlePlayer {
        extern s32 (*playerId)(cls* self);
    }

    namespace BattleWin {
        extern bool (*possessGoldPhase)(btl::BattleSystem::cls& system);
    }

    namespace CBattleDisplay {
        extern bool (*doShakeCamera)(cls* self);
    }
}

namespace CCharacterMng {
    extern c8* (*getCharacterName)(cls* self, s32 cid);
    extern s32 (*getIndexOfSearchingCharacter)(cls* self);
    extern void (*setFrameRate)(cls* self, fx32 framerate);
    extern void (*setHidden)(cls* self, s32 cid, bool hidden);
    extern void (*setPause)(cls* self, s32, bool, ds::sys3d::CAnimSet::enTYPE type);
    extern void (*setPosition)(cls* self, s32 cid, const VecFx32& position);
    extern void (*setRotation)(cls* self, s32 cid, u16 x, u16 y, u16 z);
    extern void (*setShadowAlphaRate)(cls* self, s32 cid, s32 rate);
    extern void (*setShadowEnable)(cls* self, s32 cid, bool enabled);
    extern void (*setShadowType)(cls* self, s32 cid, s32 type);
    extern void (*setTransparencyRate)(cls* self, s32 cid, s32 rate);
    extern void (*startMotion)(cls* self, s32 cid, s32, bool, u32);
}

namespace common {
    namespace EfficacyDataConvection {
        extern void (*Ctor)(cls* self);
        // empty; real cleanup is in terminate()
        extern void (*Dtor)(cls* self);
        // ignores self; reads global instance_
        extern s32* (*getEfficacyData)(cls* self, s32 effectId);
        // m_Status |= 1
        extern void (*initialize)(cls* self);
        // panics on bad magic ('BELD' = 0x444c4542) or wrong state
        extern void (*loadBELD)(cls* self);
        // unloadBELD + clears m_Status
        extern void (*terminate)(cls* self);
        // frees m_HashTable, m_EffectData, m_RawData; leaves m_Status
        extern void (*unloadBELD)(cls* self);
        extern cls* instance_;
    }
}

namespace CTextureDataMng {
    extern void (*delData)(cls* self, s32 index);
    extern void (*end)(cls* self);
    extern ds::sys3d::CModelTexture::cls* (*getTex)(cls* self, s32 index);
    extern void (*init)(cls* self);
    extern void (*initValue)(cls* self, s32 index);
    extern bool (*releaseLnkTexVram)(cls* self, s32 index);
    extern s32 (*searchDataIndex)(cls* self, const c8* name);
    extern s32 (*searchNullIndex)(cls* self);
    extern s32 (*setData)(cls* self, const c8* name, bool async);
    extern s32 (*setData2)(cls* self, const c8* name, const c8* alias, bool async);
    extern s32 (*setTextureData)(cls* self, const c8* name, bool async);
    extern bool (*setupLnkTexVram)(cls* self, s32 index, bool async);
    extern bool (*useTexture)(cls* self, s32 index);
}

namespace debug {
    namespace CharaViewerPart {
        extern void (*doInitialize)(cls* self);
    }
}

namespace dgs {
    extern NNSG2dChar* (*DGSMsdGetString)(u32 msd, s8 line, dgs::DGSMSD* dgsmsd);
}

namespace ds {
    extern CPad::cls* g_Pad;

    namespace CPad {
        extern u32 (*repeat)(cls* self);
    }

    namespace CVram {
        extern void (*releasePlttVramMng)(cls* self);
        extern void (*releaseTexVramMng)(cls* self);
        extern void (*setupPlttVramMng)(cls* self, u32, u32, s8);
        extern void (*setupTexVramMng)(cls* self, u32, u32, u32, s8);
        extern cls* m_instance;
    }

    namespace snd {
        extern void (*dssndInitialize)(const ds::snd::DSSoundDesc* desc);
        extern void (*dssndUnloadBGM1)(void);
        extern void (*dssndUnloadBGM2)(s32);
        extern void (*dssndUnloadBGMAll)(void);
        extern void (*dssndUnloadSE1)(void);
        extern void (*dssndUnloadSE2)(s32);
        extern void (*dssndUnloadSEAll)(void);
        extern void (*dssndUpdate)(void);
        extern DSSoundHeap::cls* g_SoundHeapBGM;
        extern DSSoundHeap::cls* g_SoundHeapSE;

        namespace BGMHandle {
            extern void (*Ctor)(cls* self);
            extern void (*Dtor)(cls* self);
            extern void (*forceReleaseSequence)(cls* self);
            extern bool (*getBGMEnable)(void);
            extern bool (*IsPlaying)(cls* self);
            extern void (*MoveVolume)(cls* self, s32 targetVolume, s32 frames);
            extern void (*Pause)(cls* self, bool pause);
            extern void (*Play)(cls* self, s32 playerNo, s32 bankNo, s32 playerPrio, s32 seqNo);
            extern void (*PlayV)(cls* self, const s32& args);
            extern s32 (*seqNo)(cls* self);
            extern void (*setBGMEnable)(bool enable);
            extern void (*setTempo)(cls* self, s32 ratio);
            extern void (*SetVolume)(cls* self, s32 volume);
            extern void (*Stop)(cls* self, s32 fadeFrame);
        }

        namespace DSSoundHeap {
            extern void (*dsshFinalize)(cls* self);
        }

        namespace SEHandle {
            extern void (*Ctor)(cls* self);
            extern void (*Dtor)(cls* self);
            extern bool (*IsPlaying)(cls* self);
            extern void (*MoveVolume)(cls* self, s32 targetVolume, s32 frames);
            extern void (*Play)(cls* self, s32 seqArcNo, s32 index, s32 volume, s32 pan);
            extern void (*PlayFadeIn)(cls* self, s32 seqArcNo, s32 index, s32 fadeFrames, s32 volume, s32 pan);
            extern void (*setPan)(cls* self, u16 trackBitMask, s32 pan);
            extern void (*setPanRange)(cls* self, u16 trackBitMask, s32 panRange);
            extern void (*setPitch)(cls* self, u16 trackBitMask, s32 pitch);
            extern void (*setTempo)(cls* self, s32 ratio);
            extern void (*setVolume)(cls* self, s32 volume);
            extern void (*stop)(cls* self, s32 fadeFrame);
        }
    }

    namespace sys3d {
        namespace CAnimation {
            extern void (*ctor)(cls* self);
            extern fx32 (*getFrameRate)(cls* self);
            extern void (*setFrameRate)(cls* self, fx32 framerate);
            extern void (*setup)(cls* self, void*, void*, u32, void*);
        }

        namespace CCamera {
            extern void (*setFOV)(cls* self, s32, s32);
        }

        namespace CModelTexture {
            extern void (*bindMdl)(cls* self, NNSG3dResMdl* mdl);
            extern void (*bindMdlSet)(cls* self, NNSG3dResMdlSet* mdlSet);
            extern void (*bindMdlToPltt)(cls* self, NNSG3dResMdl* mdl);
            extern void (*bindMdlToPlttByName)(cls* self, NNSG3dResMdl* mdl, const c8* name);
            extern void (*bindMdlToTex)(cls* self, NNSG3dResMdl* mdl);
            extern void (*bindMdlToTexByName)(cls* self, NNSG3dResMdl* mdl, const c8* name);
            extern void (*bindTexPlt)(cls* self);
            extern void (*cleanup)(cls* self);
            extern void (*clearValue)(cls* self);
            extern void (*Ctor)(cls* self);
            extern void (*Dtor)(cls* self);
            extern u8 (*isModelTexture)(NNSG3dResFileHeader* rawData);
            extern void (*releaseMdl)(cls* self, NNSG3dResMdl* mdl);
            extern void (*releaseMdlSet)(cls* self, NNSG3dResMdlSet* mdlSet);
            extern void (*releaseMdlToPltt)(cls* self, NNSG3dResMdl* mdl);
            extern void (*releaseMdlToPlttByName)(cls* self, NNSG3dResMdl* mdl, const c8* name);
            extern void (*releaseMdlToTex)(cls* self, NNSG3dResMdl* mdl);
            extern void (*releaseMdlToTexByName)(cls* self, NNSG3dResMdl* mdl, const c8* name);
            extern void (*releaseResource)(cls* self);
            extern u8 (*reqReleaseResource)(cls* self);
            extern void (*setup)(cls* self, NNSG3dResFileHeader* rawData, bool async);
            extern void (*tdlhCompletion)(cls* self, s32 status);
        }

        namespace CMotSet {
            extern fx32 (*getFrameRate)(cls* self);
            extern void (*setFrameRate)(cls* self, fx32 framerate);
        }
    }
}

namespace egs {
    extern void (*GilbartInitialize)(bool, bool isDoubleVoice);
    extern bool (*GilbartIsEndOfPlayVoice)(s32 index);
    extern bool (*GilbartIsPlaying)(s32 index);
    extern void (*GilbartPauseVoice)(s32 index, bool pause);
    extern void (*GilbartPlayVoice)(const c8* file, s32 index);
    extern void (*GilbartProcess)(void*);
    extern void (*GilbartSetVolume)(s32 index, float volume);
    extern s32 (*GilbartStatus)(s32 index);
    extern void (*GilbartStopVoice)(s32 index);
    extern void (*GilbartTerminate)(void);
}

namespace evt {
    namespace EventCamera {
        extern void (*startCameraMotion)(cls* self, u32, u32, u32, bool);
    }
}

namespace FlagManager {
    extern void (*Ctor)(cls* self);
    extern void (*Dtor)(cls* self);
    extern bool (*get)(cls* self, u32 bank, u32 offset);
    extern void (*reset)(cls* self, u32 bank, u32 offset);
    extern void (*reverse)(cls* self, u32 bank, u32 offset);
    extern void (*set)(cls* self, u32 bank, u32 offset);
    extern cls* (*singleton)(void);
    extern cls* _instance;
}

namespace Font {
    extern void (*initFont)(cls* self);
}

namespace itm {
    namespace EquipParameter {
        extern bool (*canEquip)(cls* self, pl::PLAYER_TYPES);
    }

    namespace ItemManager {
        extern void* (*consumptionParameter)(cls* self, s16 itemId);
        // weapon ∪ protection
        extern void* (*equipParameter)(cls* self, s16 itemId);
        extern void (*free)(cls* self);
        extern void* (*importantParameter)(cls* self, s16 itemId);
        // 0..3 or -1
        extern s32 (*itemCategory)(cls* self, s16 itemId);
        // all 4 tables in order
        extern void* (*itemParameter)(cls* self, s16 itemId);
        // panics if total ≥ 0x180 or important ≥ 0x80
        extern u8 (*load)(cls* self);
        extern void* (*protectionParameter)(cls* self, s16 itemId);
        extern void* (*weaponParameter)(cls* self, s16 itemId);
        extern cls* instance_;
    }

    namespace PossessionItemManager {
        // m_ImportantCount = 0
        extern void (*eraseAllImportantItem)(cls* self);
        // m_NormalCount = 0
        extern void (*eraseAllNormalItem)(cls* self);
        // important if ItemManager cat byte == 0x1c
        extern void (*eraseItem)(cls* self, s16 itemId, s32 count);
        // panics if index >= m_ImportantCount
        extern Entry::cls* (*importantItem)(cls* self, s32 index);
        extern s32 (*importantItemNum)(cls* self);
        // zeroes both counts
        extern void (*initialize)(cls* self);
        // m_NormalCount + m_ImportantCount
        extern s32 (*itemAllNum)(cls* self);
        // panics if index >= m_NormalCount
        extern Entry::cls* (*normalItem)(cls* self, s32 index);
        extern s32 (*normalItemNum)(cls* self);
        // merges m_ReservedCount into m_Count
        extern void (*reserveCancel)(cls* self);
        // prunes stale ids from m_ImportantItems
        extern void (*resetImportantItemId)(cls* self);
        // prunes stale ids from m_NormalItems
        extern void (*resetItemId)(cls* self);
        // nullptr if not found
        extern Entry::cls* (*searchImportantItem)(cls* self, s16 itemId);
        // nullptr if not found
        extern Entry::cls* (*searchNormalItem)(cls* self, s16 itemId);
        // merges or appends; panics if >= 0x80
        extern void (*storeImportantItem)(cls* self, s16 itemId, s32 count);
        // merges or appends; panics if >= 0x180
        extern void (*storeItem)(cls* self, s16 itemId, s32 count);
        // swaps two normal-item slots
        extern void (*swapItem)(cls* self, s16 itemId1, s16 itemId2);
    }
}

namespace map2d {
    extern c8** NAVI_MAP_TASK_NAME;

    namespace NaviMap {
        extern void (*MapOff)(void* self);
        extern void (*MapOn)(void* self);
        extern void (*NaviMapDrawOff)(void* self);
        extern void (*setAlpha)(void* self, u32 alpha);
    }
}

namespace menu {
    extern void (*playSECommon)(s32 seqArcNo, s32 index);
}

namespace mon {
    namespace MonsterManager {
        // self ignored
        extern void (*acceptMonsterManiaData)(cls* self, mon::MonsterManiaManager::cls& src);
        // panics if id < 0
        extern s16* (*actionCondition)(cls* self, s32 id);
        // panics if id < 0
        extern s16* (*ai)(cls* self, s32 id);
        // panics if id < 0
        extern s16* (*counter)(cls* self, s32 id);
        extern s16* (*dropItem)(cls* self, s32 itemId);
        // primary then alt table
        extern s16* (*effectsInfo)(cls* self, common::ABILITY_ID abilityId, s16 subId);
        extern void (*free)(cls* self);
        // "monster.chaindata.lz"
        extern u8 (*load)(cls* self);
        extern MonsterManiaManager::cls* (*monsterManiaManager)(void);
        extern void* (*monsterParameter)(cls* self, s32 monsterId);
        extern void* (*normalAttack)(cls* self, s32 index);
        extern u8* (*octmanmosLegInfo)(cls* self, u8 legId, u8 partId);
        extern s16* (*offset)(cls* self, s32 id);
        // self ignored
        extern void (*sendMonsterManiaData)(cls* self, mon::MonsterManiaManager::cls& dest);
        // stub
        extern void (*setMonsterIdForMonsterManaia)(void);
        // panics if id < 0
        extern s16* (*turnAction)(cls* self, s32 id);
        extern cls* instance_;
    }

    namespace MonsterManiaManager {
        extern void (*clearMonsterMania)(cls* self);
        // +0x10 per call, capped at 999
        extern void (*kill)(cls* self, s16 monsterId);
        // panics if index >= 0x100
        extern MonsterMania* (*monsterMania)(cls* self, s32 index);
        extern void (*onEncount)(cls* self, s16 monsterId);
        extern void (*onLibrary)(cls* self, s16 monsterId);
        // stub, always returns 1
        extern u8 (*setMonsterManiaForMonsterID)(MonsterMania& mania);
    }
}

namespace mr {
    extern u32 (*getCompleteRate)(void);
    extern MonsterRefDataManager::cls** MRDMng;

    namespace MonsterRefDataManager {
        // zeroes m_RawData only
        extern void (*Ctor)(cls* self);
        // empty
        extern void (*Dtor)(cls* self);
        // frees m_RawData, zeroes both fields
        extern void (*free)(cls* self);
        // "monster_refer.bbd.lz"
        extern void (*load)(cls* self);
        extern s16* (*monsterReference)(cls* self, s32 monsterId);
        // nullptr if out of range
        extern void* (*monsterReferenceFromIndex)(cls* self, s32 index);
    }
}

namespace part {
    namespace WorldPart {
        extern cls* wpInstance_;
    }
}

namespace pl {
    namespace LearningAbilityManager {
        extern void (*clear)(cls* self, LEARNING_ABILITY_TYPES type);
        extern void (*forget)(cls* self, common::ABILITY_ID abilityId);
        // setContents + zero
        extern void (*initialize)(cls* self, LearningAbilityManagerContents::cls& contents);
        extern u8 (*isLearning)(cls* self, common::ABILITY_ID abilityId);
        extern bool (*isMagicTypesAbility)(cls* self, common::ABILITY_ID abilityId);
        extern u8 (*isOn)(cls* self, LEARNING_ABILITY_TYPES type, s32 flagIndex);
        // checks AbilityManager + 3-magic cap
        extern void (*learn)(cls* self, common::ABILITY_ID abilityId);
        // skips isEnableLearn
        extern void (*learnForDebug)(cls* self, common::ABILITY_ID abilityId);
        // 0..5; counts bits in cat-0 array
        extern s8 (*learningMagicTypesNumber)(cls* self);
        extern void (*off)(cls* self, LEARNING_ABILITY_TYPES type, s32 flagIndex);
        extern void (*on)(cls* self, LEARNING_ABILITY_TYPES type, s32 flagIndex);
        extern void (*setContents)(cls* self, LearningAbilityManagerContents::cls& contents);
        extern u32** ARRAY_SIZE;
        extern u32** FLAG_OFFSET;
        // Per-category sizes/offsets (FLAG_OFFSET is referenced by asm but not by any
        // dumped function's pseudocode).
        extern u32** FLAG_SIZE;
    }

    namespace Player {
        // 16-byte sret
        extern void (*abilityBouns)(cls* self);
        // ys::BodyParameter&
        extern void (*addEquipmentBonus)(cls* self, ys::BodyParameter::cls& bodyParam, s32 arg);
        // ys::MagicDefenseParameter&
        extern void (*addMagicDefense)(cls* self, ys::MagicDefenseParameter& outParam, s32 arg);
        // ys::PhysicsDefenseParameter&
        extern void (*addPhysicsDefense_ib)(cls* self, ys::PhysicsDefenseParameter& outParam, s32 arg, bool flag);
        // itm::ProtectionParameter*
        extern void (*addPhysicsDefense_Pb)(cls* self, ys::PhysicsDefenseParameter& outParam, itm::ProtectionParameter* protectionParam, bool flag);
        extern void (*adjustAbility)(cls* self);
        extern void (*attachEquipmentSymbol)(cls* self, bool flag);
        extern void (*attachEquipmentSymbolOne)(cls* self, HAND_TYPE hand, bool flag);
        // -> saveParam+0x1b8
        extern u8 (*avoidance)(cls* self);
        // -> self+0x110 (ys::BodyParameter*)
        extern void* (*bodyAndBonus)(cls* self);
        // Body parameter calculations
        // ys::BodyParameter&
        extern void (*bodyAndBonus_BP)(cls* self, ys::BodyParameter::cls& bodyParam, s16, s16, s16, s16, s16);
        extern void (*calcBonus)(cls* self);
        // static
        extern s32 (*calcHpColor)(s32 hp, s32 maxHp);
        extern void (*calcMagicDefense)(cls* self);
        extern void (*calcPhysicsDefense)(cls* self);
        // Equipment
        extern bool (*canEquip)(cls* self, s16 itemId, HAND_TYPE hand);
        extern bool (*canEquipArrow)(cls* self, s32 arg, itm::WeaponParameter* weaponParam);
        extern bool (*canEquipBow)(cls* self, s32 arg, itm::WeaponParameter* weaponParam, bool flag);
        extern bool (*canEquipHarp)(cls* self, s32 arg, itm::WeaponParameter* weaponParam, bool flag);
        // itm::WeaponParameter*
        extern bool (*canEquipWeapon)(cls* self, s32 arg, itm::WeaponParameter* weaponParam);
        extern bool (*canGivenExp)(cls* self);
        extern bool (*canWinningDirection)(cls* self);
        extern void (*changeStonePalette)(cls* self, bool arg);
        extern void (*changeStonePaletteForEvent)(cls* self, bool arg);
        extern s32 (*checkHpColor)(cls* self);
        extern void (*clearCommand)(cls* self);
        // -> saveParam+0x1c
        extern ys::Condition::cls* (*condition)(cls* self);
        // Lifecycle
        extern void (*Ctor)(cls* self);
        // -> saveParam+1
        extern void* (*decantLevel)(cls* self);
        extern u16 (*defaultMaxHp)(cls* self);
        extern u16 (*defaultMaxMp)(cls* self);
        // -> saveParam+0x1b8
        extern u8 (*deftness)(cls* self);
        extern s32 (*difference)(cls* self, s32 a, s32 b);
        extern bool (*doEquip)(cls* self, s32 point, s16 itemId, bool flag);
        extern void (*Dtor)(cls* self);
        // -> self + hand*0x1c + 0x1c8
        extern void* (*equipmentAttach)(cls* self, HAND_TYPE hand);
        extern s32 (*equipmentBounsHP)(cls* self);
        extern void (*equipmentSymbolVisibility)(cls* self, bool flag);
        // 0x1c-byte sret
        extern void (*equipMenuParameter)(cls* self, s16, s16, s16, s16, s16);
        // -> self+0x118
        extern void* (*equipParameter)(cls* self);
        extern void* (*exp)(cls* self);
        extern void (*fine)(cls* self);
        extern void (*forgetAbility_AI)(cls* self, common::ABILITY_ID abilityId);
        extern void (*forgetAbility_i)(cls* self, s32 arg);
        extern void (*forgetAllAbilityForDebug)(cls* self);
        extern void (*forgetAllBlackMagicForDebug)(cls* self);
        extern void (*forgetAllNinjutsuForDebug)(cls* self);
        extern void (*forgetAllSongForDebug)(cls* self);
        extern void (*forgetAllSummonForDebug)(cls* self);
        extern void (*forgetAllWhiteMagicForDebug)(cls* self);
        extern void (*forgetLearnedMagicByEvent)(cls* self);
        // Stat growth
        extern void (*growParameter)(cls* self, u8 arg);
        // -> saveParam+0xc
        extern void* (*hp)(cls* self);
        extern void (*initialize)(cls* self, PLAYER_TYPES id);
        extern void (*initializeCommand)(cls* self);
        extern void (*initializeDefaultCommand)(cls* self);
        extern void (*initializeName)(cls* self);
        extern bool (*isCanEquipProtection)(cls* self, s32 arg, itm::ProtectionParameter* protectionParam);
        // Command list
        extern void (*isCommand)(cls* self, common::ABILITY_ID abilityId);
        extern bool (*isEquipable)(cls* self, EQUIP_POINTS point, s16 itemId);
        extern bool (*isEquipedItemOnAllPoints)(cls* self, s16 itemId);
        extern bool (*isEquipedItemOnPoint)(cls* self, s32 point, s16 itemId);
        // itm::EquipParameter*
        extern bool (*isEquipItem)(cls* self, itm::EquipParameter::cls* equipParam);
        extern void (*isEquipMetal)(cls* self);
        extern void (*isEquippedBow)(cls* self);
        extern u32 (*isFinishAttack)(cls* self);
        extern bool (*isHand)(cls* self, s32 arg, bool flag);
        extern bool (*isHandCheck)(cls* self, s32 arg, itm::WeaponParameter* weaponParam, itm::ProtectionParameter* protectionParam, bool flag1, bool flag2);
        extern bool (*isHealing)(cls* self);
        extern void (*isKnowledgeInCommand)(cls* self);
        extern void (*isUseMagic_AI)(cls* self, common::ABILITY_ID abilityId, s32 arg, bool flag);
        extern bool (*isUseMagic_BM)(cls* self, common::BabilMagicParameter* magicParam, s32 arg, bool flag);
        extern bool (*isUseMagicCharacter)(cls* self);
        extern void (*learnAbility_AI)(cls* self, common::ABILITY_ID abilityId);
        extern void (*learnAbility_ii)(cls* self, s32 arg1, s32 arg2);
        extern void (*learnAbilityForDebug)(cls* self, common::ABILITY_ID abilityId);
        extern void (*learnAllAbilityForDebug)(cls* self);
        extern void (*learnAllBlackMagicForDebug)(cls* self);
        extern void (*learnAllNinjutsuForDebug)(cls* self);
        extern void (*learnAllSongForDebug)(cls* self);
        extern void (*learnAllSummonForDebug)(cls* self);
        extern void (*learnAllWhiteMagicForDebug)(cls* self);
        // -> self+0x200
        extern LearningAbilityManager::cls* (*learningAbility)(cls* self);
        // -> saveParam+0
        extern void* (*level)(cls* self);
        extern bool (*levelDown)(cls* self, s32 levels);
        extern u64 (*levelUp)(cls* self, s32 levels);
        extern void (*listupCommand)(cls* self, SummonAbilityList::cls& list);
        extern void (*listupCommandMagic)(cls* self, SummonAbilityList::cls& list, SummonAbilityList::TYPES type);
        extern void (*listupCommandNormal)(cls* self, SummonAbilityList::cls& list);
        // ys::MagicDefenseParameter&
        extern void (*magicDefense)(cls* self, ys::MagicDefenseParameter& outParam, s16, s16, s16, s16, s16);
        // (s32)m_MagicDefensePower
        extern s32 (*magicDefensePower)(cls* self);
        // -> saveParam+0x14
        extern void* (*mp)(cls* self);
        // ys::PhysicsAttackParameter&
        extern void (*physicsAttack)(cls* self, ys::PhysicsAttackParameter& outParam, s16, s16, s16, s16, s16);
        // ys::PhysicsDefenseParameter&
        extern void (*physicsDefense)(cls* self, ys::PhysicsDefenseParameter& outParam, s16, s16, s16, s16, s16);
        // Sub-object accessors
        // -> self+0x8
        extern void* (*playerAbilityManager)(cls* self);
        extern void (*recoverHPandMP)(cls* self);
        extern void (*releaseEquipItem)(cls* self, s32 point);
        extern void (*removeEquipmentSymbol)(cls* self);
        extern void (*removeEquipmentSymbolOne)(cls* self, HAND_TYPE hand);
        extern u64 (*saveParameter)(cls* self);
        extern void (*setExp)(cls* self, u8 arg);
        extern void (*setHp)(cls* self, u8 arg);
        extern void (*setParameter)(cls* self, u8 current_lv, u8 next_lv);
        extern void (*setPhysicsAttack)(cls* self);
        // Abilities / magic
        // common::BabilMagicParameter*
        extern u8 (*spendMp)(cls* self, common::BabilMagicParameter* magicParam);
        extern void (*stonePaletteName)(cls* self, c8* out);
        extern void (*updateCondition)(cls* self);
        extern void (*updateEquipmentSymbol)(cls* self);
        extern void (*updateParameter)(cls* self);
        extern void (*useApple)(cls* self, bool arg);
        extern void (*useSomaDrop)(cls* self);
    }

    namespace PlayerParty {
        extern s16* (*abilityBouns)(cls* self, common::ABILITY_ID abilityId);
        extern void* (*abilityList)(cls* self, s32 id);
        extern void (*addItem)(cls* self, s32 itemId, s32 qty);
        // order==-1 → first free
        extern Player::cls* (*addMember)(cls* self, PLAYER_TYPES id, s32 order);
        extern s32 (*aliveNumber)(cls* self);
        extern u32 (*averageLevel)(cls* self, PLAYER_TYPES id);
        extern void (*changeFormation)(void);
        extern void (*changeMember)(cls* self, PLAYER_TYPES idA, PLAYER_TYPES idB);
        extern void (*changeMemberForOrder)(cls* self, u8 orderA, u8 orderB);
        extern void (*changeStonePalette)(cls* self, bool);
        extern void (*changeStonePaletteForEvent)(cls* self, bool);
        // Battle commands
        extern void (*clearBattleCommand)(cls* self);
        extern void (*clearBattleCommandPlayer)(cls* self, s32 index);
        extern void (*clearBattleCondition)(cls* self);
        // Lifecycle
        extern void (*Ctor)(cls* self);
        extern void (*Dtor)(cls* self);
        extern void (*fineAll)(cls* self);
        // Formation (forward to sys::GameParameter)
        extern u32 (*formation)(cls* self, u8 index);
        extern void (*free)(cls* self);
        extern void (*initForNewgame)(cls* self, bool keepPlotItems);
        extern void (*initialize)(cls* self);
        extern void (*initializeName)(cls* self);
        extern u8 (*isCommand)(cls* self, common::ABILITY_ID abilityId);
        extern bool (*isEnableMember)(cls* self);
        extern u8 (*isFrogAll)(cls* self);
        extern u8 (*isJoinedParty)(cls* self, PLAYER_TYPES id);
        extern bool (*isLearnByLevelUp)(cls* self, pl::PLAYER_TYPES playerType, common::ABILITY_ID abilityId);
        extern u8 (*isLilliputAll)(cls* self);
        extern bool (*isPartyFull)(cls* self);
        extern s16* (*layoutSceneParameter)(cls* self, s32 id);
        extern void* (*learningAbility)(cls* self, PLAYER_TYPES playerType, s32 index);
        // Static-data lookups
        // panics if level > 99
        extern void* (*levelParameter)(cls* self, PLAYER_TYPES playerType, s8 level);
        extern void (*listupCommand)(cls* self, SummonAbilityList::cls& list);
        // "player.chaindata.lz"
        extern u8 (*load)(cls* self);
        extern void (*loadComposition)(cls* self);
        extern Player::cls* (*member)(cls* self, PLAYER_TYPES id);
        extern Player::cls* (*memberForOrder)(cls* self, u8 order);
        extern s32 (*memberOrder)(cls* self, PLAYER_TYPES id);
        extern s16* (*normalAttack)(cls* self, s32 id);
        extern s16* (*normalMagic)(cls* self, s32 id);
        extern s32 (*partyMemberEnableNumber)(cls* self);
        extern Player::cls* (*player)(cls* self, PLAYER_TYPES id);
        extern u8 (*releaseMember)(cls* self, PLAYER_TYPES id);
        // Composition save/restore
        extern void (*saveComposition)(cls* self);
        extern void (*setFormation)(PARTY_FORMATION_TYPES formation);
        // Member management
        // id==-1 → nullptr
        extern void (*setMemberForOrder)(cls* self, PLAYER_TYPES id, s32 order);
        extern void (*setSaveStartCommand)(cls* self, s32 slot, s32 val);
        extern void* (*weaponOffsetParameter)(cls* self, itm::WEAPON_SYSTEM weaponSystem);
        extern Player::cls* InvalidPlayer;
        extern cls* playerPartyInstance_;
    }
}

namespace ScriptEngine {
    extern u8 (*getByte)(cls* self);
    extern u32 (*getDword)(cls* self);
    extern u32 (*jump)(cls* self, u32 target);
}

namespace sys {
    namespace GameParameter {
        extern u8* (*abilityName)(cls* self);
        extern void (*changeFormation)(cls* self);
        extern u8 (*coverNum)(const cls* self);
        extern void (*Ctor)(cls* self);
        extern s32 (*fieldSymbolID)(const cls* self);
        extern u8 (*formation)(cls* self);
        extern u8 (*gameClearCount)(cls* self);
        extern void (*getBlackChocoboHome)(cls* self, VecFx32& outPos);
        extern u8 (*getDesionDataValidity)(cls* self);
        extern c8* (*getDesionMapName)(cls* self, s32 historyNo);
        extern u8* (*getDesionPosition)(cls* self, s32 historyNo);
        extern void (*getDesionRot)(cls* self, s32 historyNo, s16& outX, s16& outY, s16& outZ);
        extern u8 (*getMagishipMoonChipX)(cls* self);
        extern u8 (*getMagishipMoonChipZ)(cls* self);
        extern void (*getMagishipPosOffsetOnMoonChip)(cls* self, VecFx32& outPos);
        extern void (*getMagshipEarthPos)(cls* self, VecFx32& outPos);
        extern void (*getMagshipMoonPos)(cls* self, VecFx32& outPos);
        extern c8* (*getMapName)(cls* self);
        extern void* (*getPassagePoint2)(cls* self, s32 index);
        extern u8 (*getSaveNo)(cls* self);
        extern s16* (*getSaveNum)(cls* self);
        extern u8 (*getTVMode)(void);
        extern u8* (*getWorldPlayerPosition)(cls* self);
        extern void (*getWorldPlayerRotation)(cls* self, s32& outX, s32& outY, s32& outZ);
        extern u8* (*getWorldTelepoMapName)(cls* self);
        extern u8* (*getWorldTelepoPosition)(cls* self);
        extern void (*getWorldTelepoRotation)(cls* self, s32& outX, s32& outY, s32& outZ);
        extern u8* (*getWorldVehiclePosition)(cls* self, s32 index);
        extern void (*getWorldVehicleRotation)(cls* self, s32 index, s32& outX, s32& outY, s32& outZ);
        extern u8 (*getWorldVehicleStay)(cls* self, s32 index);
        extern s32* (*gold)(cls* self);
        extern void (*gpCopy)(void* dst);
        extern void (*gpInitialize)(cls* self);
        extern void (*gpLoadDataReflect)(cls* self);
        extern void (*incrementCoverNum)(cls* self);
        extern void (*incrementGameClearCount)(cls* self);
        extern u8 (*isAutoMode)(cls* self);
        extern u8 (*isCloudGuideShowed)(const cls* self);
        extern bool (*isDash)(void);
        extern bool (*isPlayVoice)(const cls* self);
        extern bool (*isShowConteEventTelop)(const cls* self);
        extern u8 (*isTVMode)(void);
        extern itm::PossessionItemManager::cls* (*item)(cls* self);
        extern u8* (*monsterManiaManager)(cls* self);
        extern u8* (*musicPlayerMode)(cls* self);
        extern c8* (*playerName)(void);
        extern PlayerSaveParameter::cls* (*playerSaveParameter)(cls* self, pl::PLAYER_TYPES playerType);
        extern u32* (*playTime)(cls* self);
        extern void (*previousSaveData)(cls* self);
        extern void (*prevSaveForFlag)(cls* self);
        extern void (*prevSaveForNavimap)(cls* self);
        extern void (*prevSaveForNavimapAsync)(cls* self);
        extern void (*prevSaveForTime)(cls* self);
        extern void (*print)(cls* self);
        extern void (*printMemory)(void);
        extern void (*reflectForFlag)(cls* self);
        extern void (*reflectForNavimap2)(cls* self);
        extern void (*reflectForTime)(void);
        extern u8* (*rrc)(cls* self);
        extern void (*setAutoMode)(cls* self, bool autoMode);
        extern void (*setBlackChocoboHome)(cls* self, const VecFx32& pos);
        extern void (*setCloudGuideShowed)(cls* self, bool showed);
        extern void (*setDash)(cls* self, bool dash);
        extern void (*setDesionDataValidity)(cls* self, bool valid);
        extern void (*setDesionMapName)(cls* self, s32 historyNo, const c8* name);
        extern void (*setDesionPosition)(cls* self, s32 historyNo, VecFx32& pos);
        extern void (*setDesionRot)(cls* self, s32 historyNo, s16 x, s16 y, s16 z);
        extern void (*setFieldSymbolID)(cls* self, s32 id);
        extern void (*setFormation)(cls* self, pl::PARTY_FORMATION_TYPES formation);
        extern void (*setMagishipMoonChipX)(cls* self, u8 x);
        extern void (*setMagishipMoonChipZ)(cls* self, u8 z);
        extern void (*setMagishipPosOffsetOnMoonChip)(cls* self, const VecFx32& pos);
        extern void (*setMagshipEarthPos)(cls* self, const VecFx32& pos);
        extern void (*setMagshipMoonPos)(cls* self, const VecFx32& pos);
        extern void (*setMapName)(cls* self, const c8* name);
        extern void (*setMonsterManiaManager)(cls* self, mon::MonsterManiaManager::cls& srcManager);
        extern void (*setPartsHeaderStr)(cls* self);
        extern void (*setPassagePoint2)(cls* self, s32 index, u32* data);
        extern void (*setPlayTime)(cls* self, u32 playTime);
        extern void (*setResetCloudGuideShowed)(void);
        extern void (*setSaveNo)(cls* self, u8 saveNo);
        extern void (*setSaveNum)(cls* self, s32 index, s32 value);
        extern void (*setTVMode)(cls* self, s32 tvMode);
        extern void (*setWorldPlayerPosition)(cls* self, VecFx32 pos);
        extern void (*setWorldPlayerRotation)(cls* self, s32 x, s32 y, s32 z);
        extern void (*setWorldTelepoMapName)(cls* self, const c8* name);
        extern void (*setWorldTelepoPosition)(cls* self, VecFx32 pos);
        extern void (*setWorldTelepoRotation)(cls* self, s32 x, s32 y, s32 z);
        extern void (*setWorldVehiclePosition)(cls* self, s32 index, VecFx32 pos);
        extern void (*setWorldVehicleRotation)(cls* self, s32 index, s32 x, s32 y, s32 z);
        extern void (*setWorldVehicleStay)(cls* self, s32 index, u8 stay);
        extern u8* (*summonSaveParameter)(cls* self);
        extern cls* gpInstance_;
    }

    namespace GGlobal {
        extern part::GAMEPART (*getCurrentPart)(void);
        extern void (*setNextPart)(part::GAMEPART part);
        extern void** heapBGM_;
        extern void** heapSE_;
        extern void** heapSound_;
    }
}

namespace title {
    namespace TitleContents {
        extern void (*update)(cls* self);
    }

    namespace TitleSubState {
        extern void (*update)(cls* self);
    }
}

namespace ui {
    extern CWidgetMng::cls* g_WidgetMng;

    namespace CWidgetMng {
        // type (flags & 0xff): 1=line-list row, 2=scrollbar-list row, 3=window (BasicWindow), 4/5/6=sprite
        // (cell anim) widgets, 7/8/9/10=plain rects of increasing NNS_G2dAddWidget style index.
        extern void (*addWidget)(cls* self, s32 id, s32 x, s32 y, s32 width, s32 height, s32 text_color, s32 msd, u32 flags);
        extern void (*cleanWidget)(cls* self);
        extern void (*DefaultPadKeyFunc)(cls* self, s32 padMask, bool playSE);
        // -> DefaultPadKeyFunc_LineMulti(.., 2, ..)
        extern void (*DefaultPadKeyFunc_Line2)(cls* self, s32 padMask, bool playSE);
        // -> DefaultPadKeyFunc_LineMulti(.., 3, ..)
        extern void (*DefaultPadKeyFunc_Line3)(cls* self, s32 padMask, bool playSE);
        extern void (*DefaultPadKeyFunc_LineMulti)(cls* self, s32 padMask, s32 lineWidth, bool playSE);
        extern void (*deleteWidget)(cls* self, Widget::cls* widget);
        // count==1: delete single id; else delete [id, id+count) range
        extern void (*deleteWidgetRange)(cls* self, s32 id, s32 count);
        // thunk -> drawWidget() on the global g_WidgetMng instance
        extern void (*draw)(void);
        extern void (*drawWidget)(cls* self);
        // moveWidget() + moveSlide()
        extern void (*execute)(cls* self);
        // lookup only; result discarded by the original (likely dead code)
        extern void (*findWidget)(cls* self, s32 id);
        // first of cursors 1..6 whose widget isn't flagged 0x4
        extern s32 (*getActiveCursor)(cls* self);
        // m_CursorItem[getActiveCursor()]
        extern s32 (*getActiveCursorItem)(cls* self);
        // m_ListMax[0 or 1] for the active cursor's item bucket
        extern s32 (*getActiveCursorListMax)(cls* self);
        extern s32 (*getCursorItem)(cls* self, s32 cursor);
        // index < 2
        extern s32 (*getListMax)(cls* self, s32 index);
        extern void (*initialize)(cls* self);
        extern void (*moveSlide)(cls* self);
        extern void (*moveWidget)(cls* self);
        extern void (*setCursor)(cls* self, s32 newWidgetId, s32 anchorWidgetId, s32 msd, u32 flags, bool updateCursorTable);
        extern void (*setDefaultPadKeyFunc)(cls* self);
        extern void (*setDragState)(cls* self, bool dragging);
        // index < 2
        extern void (*setListMax)(cls* self, s32 index, s32 max);
        extern void (*setScroll)(cls* self, s32 y, s32 height, s32 max);
        extern void (*setScrollPos)(cls* self, s32 pos);
        extern void (*setScrollRect)(cls* self, s32 x, s32 y, s32 width, s32 height, s32 max);
        extern void (*setSlide)(cls* self, s32 x, s32 y);
        extern void (*setWidgetEnable)(cls* self, s32 idStart, s32 idCount, s8 enable);
        extern void (*setWidgetHilight)(cls* self, s32 idStart, s32 idCount, s32 hilightId);
        extern void (*terminate)(cls* self);
    }
}

namespace world {
    extern bool (*canChangeSymbol)(void);
    extern void (*disableSPSetting)(WorldStateContext::cls* self);
    extern void (*enableSPSetting)(WorldStateContext::cls* self);
    extern void (*MSSTextScreenClear)(msg::CANVAS canvas);
    extern s32 (*searchNextSymbolByCharacterIDBackward)(s32 id);
    extern s32 (*searchNextSymbolByCharacterIDForward)(s32 id);

    namespace CCameraVibration {
        extern void (*ccbUpdate)(cls* self, VecFx32& out1, VecFx32& out2);
        extern void (*startVibration)(cls* self, VIBRATION_STATE state, s32 time, s32 ticks, s32 x, s32 y, s32 z, bool flag);
    }

    namespace MSSConfig {
        extern void* (*mssInitialize)(cls* self);
        extern void* (*mssProcess)(cls* self);
    }

    namespace WorldState {
        extern c8* (*wsGetString)(cls* self);
        extern void (*wsSetEnd)(cls* self, bool);
    }

    namespace WorldStateScheduler {
        extern bool (*wssAddStateSchedule)(cls* self, const c8* state);
        extern WorldState::cls* (*wssGetCurrentState)(const cls* self);
        extern void (*wssInitialize)(cls* self);
    }

    namespace WorldTask {
        extern void (*deleteTask)(const c8* task);
        extern void* (*queryTask)(const c8* task);
    }

    namespace WSMove {
        extern bool (*wsIsMenuOpen)(cls* self, WorldStateContext::cls* wsc);
        extern u64 (*wsmEncount)(cls* self, WorldStateContext::cls* wsc);
        extern void (*WSMove)(cls* self);
    }

    namespace WSVehicleMove {
        extern u64 (*wsProcess)(cls* self, WorldStateContext::cls* wsc);
    }
}

namespace ys {
    namespace Condition {
        extern bool (*isNotBattleCondition)(cls* self);
    }
}

void init();

namespace dgs {
    inline DGSMSD* DGSMSD_ALL = reinterpret_cast<DGSMSD*>(-1);
}
} // namespace babil
