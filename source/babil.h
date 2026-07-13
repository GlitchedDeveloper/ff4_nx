#pragma once

#include <GLES/gl.h>

#include "so_util.h"
#include "types.h"

#define HOOK_FUNCTION_BASE(name, dest) so::hook((uintptr_t)name, (uintptr_t)dest)
#define HOOK_FUNCTION_T_BASE(name, dest, trampoline) so::hook((uintptr_t)name, (uintptr_t)dest, (uintptr_t*)&trampoline)
#define HOOK_FUNCTION(name) HOOK_FUNCTION_BASE(babil::name, name)
#define HOOK_FUNCTION_T(name)                    \
    debugPrintf("HOOK_FUNCTION_T(" #name ")\n"); \
    HOOK_FUNCTION_T_BASE(babil::name, name, name##_t)
#define DECLARE_TRAMPOLINE(name, trampoline) static decltype(name) trampoline = nullptr

namespace babil {

extern int (*render)(char*, int, int);
extern int (*touch)(int, int, int, int, float, float, float, float);

typedef s32 fx32;
struct VecFx32
{
    fx32 x;
    fx32 y;
    fx32 z;
};
typedef char16_t NNSG2dChar;

struct NNSG3dResMdl;
struct NNSG3dResMdlSet;
struct NNSG3dResTex;
struct NNSG3dResFileHeader;

extern void (*OSi_Panic)(const char* file, int line, const char* message);
extern void (*OS_Printf)(const char* fmt, ...);

extern fx32 (*FX_Div)(fx32 frame, fx32 framerate);
extern void (*NNS_G2dTickCellAnimation)(void* pCellAnim, fx32 frames);
extern void (*NNS_SndPlayerStopSeqBySeqArcIdx)(s32 seqArcNo, s32 index, s32 frames);

extern void (*GX_SetWideMode)(void* JNIEnv, bool wide);
extern void (*G2_SetWnd0Position)(s32 x1, s32 y1, s32 x2, s32 y2);

namespace ScriptEngine {
    struct cls
    {
    };
    extern u8 (*getByte)(ScriptEngine::cls* scriptEngine);
    extern u32 (*getDword)(ScriptEngine::cls* scriptEngine);
    extern u32 (*jump)(ScriptEngine::cls* scriptEngine, u32 target);
}

extern void (*babilCommand_ClearCountJump)(ScriptEngine::cls* scriptEngine);
extern void (*babilCommand_CE_CameraPos)(ScriptEngine::cls* scriptEngine);
extern void (*babilCommand_CE_SetupCameraMotion)(ScriptEngine::cls* scriptEngine);

extern u32* texCount;
extern u32* memCount;

extern u32* LCD_WIDTH;
extern u32* LCD_HEIGHT;

enum BabilGamePart : u32
{
    BabilGamePart_DebugPart           = 0,
    BabilGamePart_CompanyLogoPart     = 1,
    BabilGamePart_MoviePart           = 2,
    BabilGamePart_TitlePart           = 3,
    BabilGamePart_WorldPart           = 6,
    BabilGamePart_BattlePart          = 8,
    BabilGamePart_BackupRomFormatPart = 23,
    BabilGamePart_SoundDebugPart      = 32,
    BabilGamePart_SoundPlayerPart     = 33,
};

namespace itm {
    namespace PossessionItemManager {
        struct cls;
    }
}

namespace pl {
    enum PLAYER_TYPES : s32;
}

namespace ys {
    namespace Condition {
        struct cls
        {
        };
        extern bool (*isNotBattleCondition)(cls* self);
    }
    namespace BodyParameter {
        struct cls
        {
            u8 m_Strength;
            u8 m_Speed;
            u8 m_Stamina;
            u8 m_Intellect;
            u8 m_Spirit;
            s16 m_MaxMP;
        };
    }
}

namespace sys {
    namespace GGlobal {
        extern u32 (*getCurrentPart)();
        extern void (*setNextPart)(u32 part);
    }
    // Game-wide save-data container (0x3dc0 bytes total). Layout: SaveHeader (0x20),
    // SaveDisplayPartyInfo (0x70), 5× ys::Condition (0x14 each starting at 0x30),
    // config flags (0x94), 15× PlayerSaveParameter (0x1c0 each starting at 0x9c),
    // SummonSaveParameter (0xb4 @ 0x191c), PartySaveData (0x84c @ 0x19d0),
    // WorldSaveData (0x138 @ 0x221c), BattleSaveData (8 @ 0x2354),
    // EventSaveData (0x10c @ 0x235c, holds 2×1000-bit flag arrays at 0x2360/0x23e3),
    // PleasureSaveData (0x208 @ 0x2468), NavimapSaveData (0x1744 @ 0x2670).
    namespace GameParameter {
        struct cls
        {
            u8 unk[0x94];
            u16 flags;
            u8 unk2[0x3d2a];
        };
        static_assert(offsetof(cls, flags) == 0x94);

        namespace PlayerSaveParameter {
            struct cls
            {
                u8 unk[0x1b6];
                ys::BodyParameter::cls m_BodyParameter;
            };
        }

        extern void (*Ctor)(cls* self);
        extern void (*gpInitialize)(cls* self);
        extern void (*gpLoadDataReflect)(cls* self);
        extern void (*previousSaveData)(cls* self);
        extern void (*gpCopy)(void* dst);
        extern void (*print)(cls* self);
        extern void (*printMemory)();
        extern u8 (*getSaveNo)(cls* self);
        extern void (*setSaveNo)(cls* self, u8 saveNo);
        extern s16* (*getSaveNum)(cls* self);
        extern void (*setSaveNum)(cls* self, s32 index, s32 value);
        extern u32* (*playTime)(cls* self);
        extern void (*setPlayTime)(cls* self, u32 playTime);
        extern void (*prevSaveForTime)(cls* self);
        extern void (*reflectForTime)();
        extern void (*prevSaveForFlag)(cls* self);
        extern void (*reflectForFlag)(cls* self);
        extern void (*prevSaveForNavimap)(cls* self);
        extern void (*prevSaveForNavimapAsync)(cls* self);
        extern void (*reflectForNavimap2)(cls* self);
        extern void (*setPartsHeaderStr)(cls* self);
        extern u8 (*isDash)();
        extern void (*setDash)(cls* self, bool dash);
        extern bool (*isCloudGuideShowed)(cls* self);
        extern void (*setCloudGuideShowed)(cls* self, bool showed);
        extern void (*setResetCloudGuideShowed)();
        extern bool (*isShowConteEventTelop)(cls* self);
        extern u8 (*isAutoMode)(cls* self);
        extern void (*setAutoMode)(cls* self, bool autoMode);
        extern u8 (*isTVMode)();
        extern void (*setTVMode)(cls* self, s32 tvMode);
        extern u8 (*getTVMode)();
        extern bool (*isPlayVoice)(cls* self);
        extern s32 (*fieldSymbolID)(cls* self);
        extern void (*setFieldSymbolID)(cls* self, s32 id);
        extern u8 (*gameClearCount)(cls* self);
        extern void (*incrementGameClearCount)(cls* self);
        extern u8 (*coverNum)(cls* self);
        extern void (*incrementCoverNum)(cls* self);
        extern u8 (*formation)(cls* self);
        extern void (*setFormation)(cls* self, u8 formation);
        extern void (*changeFormation)(cls* self);
        extern s32* (*gold)(cls* self);
        extern itm::PossessionItemManager::cls* (*item)(cls* self);
        extern u8* (*rrc)(cls* self);
        extern u8* (*abilityName)(cls* self);
        extern u8* (*playerName)();
        extern char* (*getMapName)(cls* self);
        extern void (*setMapName)(cls* self, const char* name);
        extern u8* (*musicPlayerMode)(cls* self);
        extern u8* (*monsterManiaManager)(cls* self);
        extern void (*setMonsterManiaManager)(cls* self, void* srcManager);
        extern PlayerSaveParameter::cls* (*playerSaveParameter)(cls* self, pl::PLAYER_TYPES playerType);
        extern u8* (*summonSaveParameter)(cls* self);
        extern u8* (*getWorldPlayerPosition)(cls* self);
        extern void (*setWorldPlayerPosition)(cls* self, VecFx32 pos);
        extern void (*getWorldPlayerRotation)(cls* self, s32* outX, s32* outY, s32* outZ);
        extern void (*setWorldPlayerRotation)(cls* self, s32 x, s32 y, s32 z);
        extern u8* (*getWorldVehiclePosition)(cls* self, s32 index);
        extern void (*setWorldVehiclePosition)(cls* self, s32 index, VecFx32 pos);
        extern void (*getWorldVehicleRotation)(cls* self, s32 index, s32* outX, s32* outY, s32* outZ);
        extern void (*setWorldVehicleRotation)(cls* self, s32 index, s32 x, s32 y, s32 z);
        extern u8 (*getWorldVehicleStay)(cls* self, s32 index);
        extern void (*setWorldVehicleStay)(cls* self, s32 index, u8 stay);
        extern u8* (*getWorldTelepoPosition)(cls* self);
        extern void (*setWorldTelepoPosition)(cls* self, VecFx32 pos);
        extern void (*getWorldTelepoRotation)(cls* self, s32* outX, s32* outY, s32* outZ);
        extern void (*setWorldTelepoRotation)(cls* self, s32 x, s32 y, s32 z);
        extern u8* (*getWorldTelepoMapName)(cls* self);
        extern void (*setWorldTelepoMapName)(cls* self, const char* name);
        extern void (*getMagishipPosOffsetOnMoonChip)(cls* self, VecFx32* outPos);
        extern void (*setMagishipPosOffsetOnMoonChip)(cls* self, const VecFx32* pos);
        extern u8 (*getMagishipMoonChipX)(cls* self);
        extern void (*setMagishipMoonChipX)(cls* self, u8 x);
        extern u8 (*getMagishipMoonChipZ)(cls* self);
        extern void (*setMagishipMoonChipZ)(cls* self, u8 z);
        extern void (*getMagshipMoonPos)(cls* self, VecFx32* outPos);
        extern void (*setMagshipMoonPos)(cls* self, const VecFx32* pos);
        extern void (*getMagshipEarthPos)(cls* self, VecFx32* outPos);
        extern void (*setMagshipEarthPos)(cls* self, const VecFx32* pos);
        extern void (*getBlackChocoboHome)(cls* self, VecFx32* outPos);
        extern void (*setBlackChocoboHome)(cls* self, const VecFx32* pos);
        extern char* (*getDesionMapName)(cls* self, s32 historyNo);
        extern void (*setDesionMapName)(cls* self, s32 historyNo, const char* name);
        extern u8* (*getDesionPosition)(cls* self, s32 historyNo);
        extern void (*setDesionPosition)(cls* self, s32 historyNo, VecFx32* pos);
        extern void (*getDesionRot)(cls* self, s32 historyNo, s16* outX, s16* outY, s16* outZ);
        extern void (*setDesionRot)(cls* self, s32 historyNo, s16 x, s16 y, s16 z);
        extern u8 (*getDesionDataValidity)(cls* self);
        extern void (*setDesionDataValidity)(cls* self, bool valid);
        extern void* (*getPassagePoint2)(cls* self, s32 index);
        extern void (*setPassagePoint2)(cls* self, s32 index, u32* data);

        extern cls* gpInstance_;
    }
}

namespace sys2d {
    namespace Sprite3d {
        struct cls
        {
            u8 unk[0xe8];
            u32 flags;
        };
    }
}

namespace map2d {
    extern char* NAVI_MAP_TASK_NAME;
    namespace NaviMap {
        extern void (*MapOn)(void* self);
        extern void (*MapOff)(void* self);
        extern void (*setAlpha)(void* self, u32 alpha);
        extern void (*NaviMapDrawOff)(void* self);
    }
}

namespace movie::MoviePart {
    struct cls
    {
    };
    extern cls* (*getInstance)();
    extern void (*setAfterPart)(cls*, BabilGamePart);
}

namespace ui {
    // A single widget node, allocated individually (malloc_count(0x38)) and stored
    // in CWidgetMng::cls::m_WidgetTable as a singly-linked list, hashed by (id & 0x3f).
    struct Widget
    {
        u32 id; // 0x00
        s32 x; // 0x04
        s32 y; // 0x08
        s32 width; // 0x0c
        s32 height; // 0x10
        s32 textColor; // 0x14
        s32 msd; // 0x18
        u32 flags; // 0x1c - bit0..7: widget "type" (switch in drawWidget); 0x200: scroll-relative;
                   //        0x400: alt size variant; 0x800: alt hilight style; 0x1000: disabled
                   //        (setWidgetEnable); 0x2000: hilighted/selected (setWidgetHilight);
                   //        0x4000: dgs msd-name special case (addWidget)
        void* basicWindow; // 0x20 - menu::BasicWindow*, only allocated for type 3 widgets
        void* sprite; // 0x28 - sys2d::Sprite*/Sprite3d*, allocated for type 4/5/6 widgets
        Widget* next; // 0x30 - next node in this bucket's chain
    };
    static_assert(sizeof(Widget) == 0x38);

    namespace CWidgetMng {
        struct cls
        {
            Widget* m_WidgetTable[0x40]; // 0x000 - hash table of widget chains, bucket = id & 0x3f
            void* field_0x200; // 0x200 - source data ptr for type-4 sprite widgets (addWidget); cleared in init/terminate
            void* field_0x208; // 0x208 - source data ptr for type-5/6 sprite widgets (addWidget); cleared in init/terminate
            s32 m_SelectedElement; // 0x210 - widget id currently selected/pressed (confirmed against the
                                   //         raw disassembly; Ghidra's pseudocode mislabels this field).
            s32 field_0x214; // 0x214 - scratch state shared by moveWidget (previous-frame hover id)
                             //         and moveSlide (next slide-state value); same 4 bytes, two uses.
            s32 m_FocusedElement; // 0x218 - write-only from these 31 functions; mirrors m_SelectedElement
                                  //         whenever it changes, presumably read by code outside CWidgetMng.
            s32 field_0x21c; // 0x21c - write-only from these 31 functions; same mirroring as field_0x218.
            s32 m_SelectedHoldFrames; // 0x220 - frame counter for press-and-hold on m_SelectedElement
            u32 field_0x224; // 0x224 - touch-anchor widget id snapshot
            s16 field_0x228; // 0x228 - previous touch x
            s16 field_0x22a; // 0x22a - previous touch y
            s16 field_0x22c; // 0x22c - touch-down x
            s16 field_0x22e; // 0x22e - touch-down y
            bool m_WasTouching; // 0x230 - touch panel was down last frame
            bool m_DragState; // 0x231 - set via setDragState
            u8 _pad0232[2]; // 0x232
            s32 m_SlideState; // 0x234 - moveSlide() state machine state
            s32 m_SlideOffset; // 0x238 - current slide offset (signed, animates back to 0)
            s32 m_SlideDirection; // 0x23c - +1/-1 direction multiplier for the active slide
            s32 m_SlideX; // 0x240 - x passed to NNS_G2dSetSlide, set by setSlide
            s32 m_SlideY; // 0x244 - y passed to NNS_G2dSetSlide, set by setSlide
            s16 m_TouchAnchorX; // 0x248 - touch x sampled when the slide drag began
            s16 m_TouchAnchorY; // 0x24a - touch y sampled when the slide drag began
            s32 m_ScrollPos; // 0x24c - current scroll offset, set by setScroll(Pos/Rect)
            s32 m_ScrollMax; // 0x250 - max scrollable extent (setScroll param_3 / setScrollRect param_5)
            s32 m_ScrollX; // 0x254 - x passed to NNS_G2dSetScroll
            s32 m_ScrollY; // 0x258 - y passed to NNS_G2dSetScroll
            s32 field_0x25c; // 0x25c - extra scroll-rect width, only set by setScrollRect
            s32 m_ScrollHeight; // 0x260 - viewport height, set by setScroll(param_2)/setScrollRect(param_4)
            float m_ScrollVelocity; // 0x264 - scroll/select animation inertia value
            s32 m_CursorItem[6]; // 0x268 - indexed directly by cursor id (1..6); slot 0 is unused padding,
                                 //         so the array really spans ids 0..6 and id 6's slot is the same
                                 //         4 bytes as m_ListMax[0] below (harmless aliasing in the original).
            s32 m_ListMax[2]; // 0x280 - set/read via setListMax/getListMax; m_ListMax[0] aliases the
                              //         cursor-id-6 slot above (same bytes, reused by the original code)
            bool m_PadCheckEnable; // 0x288 - gates the pad-driven selection shortcut in moveWidget
            u8 _pad0289[3]; // 0x289
            u32 m_PadMaskSelect; // 0x28c - pad bitmask checked (with 0x10000) to jump to active cursor item
            u32 m_PadMaskCancel; // 0x290 - pad bitmask checked for the "no selection yet" shortcut
            u8 _pad0294[4]; // 0x294
            void (*m_PadKeyFunc)(cls* self, s32 arg); // 0x298 - per-frame pad callback, default set by setDefaultPadKeyFunc
            s32 m_PadKeyFuncArg; // 0x2a0 - argument passed to m_PadKeyFunc
        };

        extern void (*initialize)(cls* self);
        extern void (*terminate)(cls* self);
        extern void (*cleanWidget)(cls* self);

        // type (flags & 0xff): 1=line-list row, 2=scrollbar-list row, 3=window (BasicWindow), 4/5/6=sprite
        // (cell anim) widgets, 7/8/9/10=plain rects of increasing NNS_G2dAddWidget style index.
        extern void (*addWidget)(cls* self, s32 id, s32 x, s32 y, s32 width, s32 height, s32 text_color, s32 msd, u32 flags);
        extern void (*deleteWidget)(cls* self, Widget* widget);
        extern void (*deleteWidgetRange)(cls* self, s32 id, s32 count); // count==1: delete single id; else delete [id, id+count) range
        extern void (*findWidget)(cls* self, s32 id); // lookup only; result discarded by the original (likely dead code)

        extern void (*draw)(); // thunk -> drawWidget() on the global g_WidgetMng instance
        extern void (*drawWidget)(cls* self);
        extern void (*execute)(cls* self); // moveWidget() + moveSlide()
        extern void (*moveWidget)(cls* self);
        extern void (*moveSlide)(cls* self);

        extern void (*setWidgetHilight)(cls* self, s32 idStart, s32 idCount, s32 hilightId);
        extern void (*setWidgetEnable)(cls* self, s32 idStart, s32 idCount, char enable);

        extern s32 (*getActiveCursor)(cls* self); // first of cursors 1..6 whose widget isn't flagged 0x4
        extern s32 (*getActiveCursorItem)(cls* self); // m_CursorItem[getActiveCursor()]
        extern s32 (*getActiveCursorListMax)(cls* self); // m_ListMax[0 or 1] for the active cursor's item bucket
        extern s32 (*getCursorItem)(cls* self, s32 cursor);
        extern void (*setCursor)(cls* self, s32 newWidgetId, s32 anchorWidgetId, s32 msd, u32 flags, bool updateCursorTable);

        extern void (*setListMax)(cls* self, s32 index, s32 max); // index < 2
        extern s32 (*getListMax)(cls* self, s32 index); // index < 2

        extern void (*setScroll)(cls* self, s32 y, s32 height, s32 max);
        extern void (*setScrollPos)(cls* self, s32 pos);
        extern void (*setScrollRect)(cls* self, s32 x, s32 y, s32 width, s32 height, s32 max);

        extern void (*setSlide)(cls* self, s32 x, s32 y);

        extern void (*setDefaultPadKeyFunc)(cls* self);
        extern void (*setDragState)(cls* self, bool dragging);

        extern void (*DefaultPadKeyFunc)(cls* self, s32 padMask, bool playSE);
        extern void (*DefaultPadKeyFunc_Line2)(cls* self, s32 padMask, bool playSE); // -> DefaultPadKeyFunc_LineMulti(.., 2, ..)
        extern void (*DefaultPadKeyFunc_Line3)(cls* self, s32 padMask, bool playSE); // -> DefaultPadKeyFunc_LineMulti(.., 3, ..)
        extern void (*DefaultPadKeyFunc_LineMulti)(cls* self, s32 padMask, s32 lineWidth, bool playSE);
    }
    extern CWidgetMng::cls* g_WidgetMng;
}

namespace object {
    namespace CharacterObject {
        struct cls
        {
            u8 unk[0x30];
            VecFx32 position;
            u8 unk2[0x34];
            s32 cid;
            u8 unk3[0x10];
            s32 rotX;
            s32 rotY;
            s32 rotZ;
            u8 unk4[0x188];
            s32 cid2;
        };
    }
}

namespace msg {
    enum CANVAS : unsigned long
    {

    };
}

namespace world {
    namespace WorldCamera {
        struct cls
        {
        };
    }
    namespace WorldStateContext {
        struct cls
        {
            u8 unk[0x60];
            WorldCamera::cls m_WorldCamera;
            u8 unk2[0xfc];
            object::CharacterObject::cls m_PlayerCharacter;
        };
        static_assert(offsetof(cls, m_WorldCamera) == 0x60);
        static_assert(offsetof(cls, m_PlayerCharacter) == 0x160);
    }
    namespace WorldState {
        struct cls
        {
        };
        extern char* (*wsGetString)(cls* self);
        extern void (*wsSetEnd)(cls* self);
    }
    namespace WorldStateScheduler {
        struct cls
        {
        };
        extern void (*wssInitialize)(cls* self);
        extern WorldState::cls* (*wssGetCurrentState)(cls* self);
        extern bool (*wssAddStateSchedule)(cls* self, const char* state);
    }
    namespace WorldTask {
        extern void* (*queryTask)(const char* task);
        extern void (*deleteTask)(const char* task);
    }
    namespace MSSConfig {
        struct cls
        {
        };
        extern void* (*mssInitialize)(cls* self);
        extern void* (*mssProcess)(cls* self);
    }
    namespace WSMove {
        struct cls
        {
        };
        extern void (*WSMove)(cls* self);
        extern u64 (*wsmEncount)(cls* self, WorldStateContext::cls* p_WorldStateContext);
        extern bool (*wsIsMenuOpen)(cls* self, WorldStateContext::cls* p_WorldStateContext);
    }
    namespace WSVehicleMove {
        struct cls
        {
            u8 unk1[0x50];
            WorldStateScheduler::cls* m_WSS;
            u8 unk2[20];
            bool m_bool;
        };
        extern u64 (*wsProcess)(cls* self, WorldStateContext::cls* p_WorldStateContext);
    }
    namespace CCameraVibration {
        struct cls
        {
            s32 m_State;
            s32 unk1;
            s32 m_CurrentTime;
            s32 m_TotalTime;
            s32 unk2;
            s32 unk3;
            VecFx32 m_Amplitude;
            VecFx32 m_LastPosition;
            VecFx32 m_Offset;
        };
        enum VIBRATION_STATE
        {
            NONE = 0,
            A    = 1,
            B    = 2
        };
        extern void (*startVibration)(cls* self, VIBRATION_STATE state, s32 time, s32 ticks, s32 x, s32 y, s32 z, u8);
        extern void (*ccbUpdate)(cls* self, VecFx32*, VecFx32*);
    }
    extern void (*enableSPSetting)(WorldStateContext::cls* self);
    extern void (*disableSPSetting)(WorldStateContext::cls* self);

    extern s32 (*searchNextSymbolByCharacterIDBackward)(s32 id);
    extern s32 (*searchNextSymbolByCharacterIDForward)(s32 id);
    extern bool (*canChangeSymbol)();

    extern void (*MSSTextScreenClear)(msg::CANVAS canvas);
}

namespace btl {
    namespace BattlePart {
        struct cls
        {
            u8 unk[0x24];
            bool m_IsPaused;
            bool m_ShouldPause;
        };
        extern void (*doInitialize)(cls* self);
        extern cls* instance_;
    }
    namespace Battle2DManager {
        struct cls
        {
            u8 unk[0x11328];
            sys2d::Sprite3d::cls m_BattlePause;
        };
        extern cls* instance_;
        enum WIDGET_CTRL_TYPE : u32
        {
            Widget_Pause = 0,
        };
        extern bool (*ctrlWidgetCheck)(cls* self, WIDGET_CTRL_TYPE type);
    }
    namespace BattleMonsterParty {
        struct cls
        {
        };
        extern u32 (*giftGold)(cls* self);
        extern u32 (*giftExp)(cls* self);
    }
    namespace BattleSystem {
        struct cls
        {
        };
    }
    namespace BattlePlayer {
        struct cls
        {
        };
        extern s32 (*playerId)(cls* self);
    }
    namespace BattleCommandSelector {
        struct cls
        {
        };
        extern void (*initialize)(cls* self, BattleSystem::cls* system, BattlePlayer::cls* player);
        enum ABILITY_ID : u8
        {
        };
        extern u32 (*commandAction)(cls* self, BattleSystem::cls* system, int, ABILITY_ID abilityId);
    }
    namespace BaseBattleCharacter {
        struct cls
        {
            u8 unk[0x10c];
            bool m_IsEnemy;
        };
        extern bool (*isInvincible)(cls* self);
    }

    enum DEBUG_FLAG : u32
    {
        SURELY_MAX_DAMAGE,
        QUICK_WAIT,
        ENEMY_INVINCIBLE,
        FRIEND_INVINCIBLE,
        RESTART,
        SURELY_ESCAPE,
        SURELY_CONDITION,
        TRANSFIX,
        MDEF_INVALIDATION,
        QUICK_EVENT,
        SURELY_HIT,
        SURELY_MISS,
        SURELY_CRITICAL,
        MONSTER_STOP_ACTION,
        DAMAGE_OVER_LIMIT,
        OPEN_ENEMY_HP,
        PHYSICS_RANDOM_MAX,
        MAGIC_RANDOM_MAX,
        QUICK_TURN,
        DRAW_CHAR_TOUCH_RECT,
        MODEL_CHANGE_CHECK,
        MODEL_CHANGE_SOUND
    };
    namespace BattleDebugParameter {
        struct cls
        {
        };
        extern bool (*flag)(cls* self, DEBUG_FLAG flag);
    }
    namespace CBattleDisplay {
        struct cls
        {
            u8 unk[0x1b8];
            s32 m_ShakeTimer;
            u8 unk2[0x1C];
            u32 m_ShakeAmplitudeX;
            u32 m_ShakeAmplitudeY;
            u32 m_ShakeAmplitudeZ;
        };
        static_assert(offsetof(cls, m_ShakeTimer) == 0x1b8);
        static_assert(offsetof(cls, m_ShakeAmplitudeX) == 0x1d8);
        static_assert(offsetof(cls, m_ShakeAmplitudeY) == 0x1dc);
        static_assert(offsetof(cls, m_ShakeAmplitudeZ) == 0x1e0);
        extern bool (*doShakeCamera)(cls* self);
    }
    namespace BattleWin {
        extern bool (*possessGoldPhase)(BattleSystem::cls* self);
    }
    namespace AcquiredGoldDrawer {
        struct cls
        {
        };
        extern void (*update)(cls* self);
    }

    extern u32 (*battleSpeedRate)();
}

namespace ds {
    namespace snd {
        namespace BGMHandle {
            struct cls
            {
                void* m_NNSSndHandle;
                bool m_pausing;
                int m_seqNo;
            };
            extern void (*Ctor)(cls* self);
            extern void (*Dtor)(cls* self);
            extern bool (*IsPlaying)(cls* self);
            extern void (*PlayV)(cls* self, int* args);
            extern void (*Play)(cls* self, int playerNo, int bankNo, int playerPrio, int seqNo);
            extern void (*SetVolume)(cls* self, int volume);
            extern void (*MoveVolume)(cls* self, int targetVolume, int frames);
            extern void (*Stop)(cls* self, int fadeFrame);
            extern int (*seqNo)(cls* self);
            extern void (*Pause)(cls* self, bool pause);
            extern void (*setBGMEnable)(bool enable);
            extern bool (*getBGMEnable)();
            extern void (*forceReleaseSequence)(cls* self);
            extern void (*setTempo)(cls* self, int ratio);
        }

        namespace SEHandle {
            struct cls
            {
                void* m_NNSSndHandle;
            };
            extern void (*Ctor)(cls* self);
            extern void (*Dtor)(cls* self);
            extern bool (*IsPlaying)(cls* self);
            extern void (*Play)(cls* self, int seqArcNo, int index, int volume, int pan);
            extern void (*PlayFadeIn)(cls* self, int seqArcNo, int index, int fadeFrames, int volume, int pan);
            extern void (*setPan)(cls* self, u16 trackBitMask, int pan);
            extern void (*setPanRange)(cls* self, u16 trackBitMask, int panRange);
            extern void (*setPitch)(cls* self, u16 trackBitMask, int pitch);
            extern void (*setTempo)(cls* self, int ratio);
            extern void (*setVolume)(cls* self, int volume);
            extern void (*MoveVolume)(cls* self, int targetVolume, int frames);
            extern void (*stop)(cls* self, int fadeFrame);
        }

        extern void (*dssndUnloadSE1)(void);
        extern void (*dssndUnloadSE2)(s32);
        extern void (*dssndUnloadSEAll)(void);

        extern void (*dssndUnloadBGM1)(void);
        extern void (*dssndUnloadBGM2)(s32);
        extern void (*dssndUnloadBGMAll)(void);
    }

    namespace sys3d {
        namespace CAnimation {
            struct cls
            {
                u8 unk[0x20];
                fx32 framerate;
            };
            extern void (*ctor)(cls* self);
            extern fx32 (*getFrameRate)(cls* self);
            extern void (*setup)(cls* self, void*, void*, u32, void*);
            extern void (*setFrameRate)(cls* self, fx32 framerate);
        }
        namespace CMotSet {
            struct cls
            {
            };
            extern fx32 (*getFrameRate)(cls* self);
            extern void (*setFrameRate)(cls* self, fx32 framerate);
        }
        namespace CModelTexture {
            struct cls
            {
                void* m_Vtable; // 0x00
                void* m_VtableHandler; // 0x08
                u32 m_TexRequiredSize; // 0x10
                u32 m_Tex4x4RequiredSize; // 0x14
                u32 m_PlttRequiredSize; // 0x18
                u8 _pad1c[4]; // 0x1c
                NNSG3dResFileHeader* m_RawData; // 0x20
                u32 m_TexKey; // 0x28
                u32 m_Tex4x4Key; // 0x2c
                u32 m_PlttKey; // 0x30
                u8 _pad34[4]; // 0x34
                NNSG3dResTex* m_ResTex; // 0x38
                u8 _pad3c[4]; // 0x3c
                u32 m_Flags; // 0x40
            };
            static_assert(sizeof(cls) == 0x48);

            extern void (*Ctor)(cls* self);
            extern void (*Dtor)(cls* self);
            extern void (*clearValue)(cls* self);
            extern void (*setup)(cls* self, NNSG3dResFileHeader* rawData, bool async);
            extern void (*cleanup)(cls* self);
            extern void (*releaseResource)(cls* self);
            extern u8 (*reqReleaseResource)(cls* self);
            extern void (*tdlhCompletion)(cls* self, int status);
            extern u8 (*isModelTexture)(NNSG3dResFileHeader* rawData);

            extern void (*bindMdl)(cls* self, NNSG3dResMdl* mdl);
            extern void (*releaseMdl)(cls* self, NNSG3dResMdl* mdl);
            extern void (*bindMdlSet)(cls* self, NNSG3dResMdlSet* mdlSet);
            extern void (*releaseMdlSet)(cls* self, NNSG3dResMdlSet* mdlSet);
            extern void (*bindMdlToTex)(cls* self, NNSG3dResMdl* mdl);
            extern void (*releaseMdlToTex)(cls* self, NNSG3dResMdl* mdl);
            extern void (*bindMdlToTexByName)(cls* self, NNSG3dResMdl* mdl, const char* name);
            extern void (*releaseMdlToTexByName)(cls* self, NNSG3dResMdl* mdl, const char* name);
            extern void (*bindMdlToPltt)(cls* self, NNSG3dResMdl* mdl);
            extern void (*releaseMdlToPltt)(cls* self, NNSG3dResMdl* mdl);
            extern void (*bindMdlToPlttByName)(cls* self, NNSG3dResMdl* mdl, const char* name);
            extern void (*releaseMdlToPlttByName)(cls* self, NNSG3dResMdl* mdl, const char* name);
            extern void (*bindTexPlt)(cls* self);
        }

        namespace CCamera {
            struct cls
            {
            };
            extern void (*setFOV)(cls* self, s32, s32);
        }
    }

    namespace CPad {
        struct cls
        {
        };
        extern u32 (*repeat)(cls* self);
    }
    extern CPad::cls* g_Pad;

    namespace CVram {
        struct cls
        {
            u8 unk[0x18];
            void* m_TexVramMng;
            void* m_PlttVramMng;
        };
        extern void (*setupTexVramMng)(cls* self, u32, u32, u32, s8);
        extern void (*setupPlttVramMng)(cls* self, u32, u32, s8);
        extern void (*releasePlttVramMng)(cls* self);
        extern void (*releaseTexVramMng)(cls* self);
        extern void (*releasePlttVramMng)(cls* self);
        extern cls* m_instance;
    }
}

namespace CTextureDataMng {
    struct cls
    {
        struct
        {
            u8 m_Setup; // 0x00
            char m_Name[24]; // 0x01
            s32 m_RefCount; // 0x1c
            u8 unk[0x28]; // 0x20 - ds::fs::CFileData
            ds::sys3d::CModelTexture::cls m_CModelTextures; // 0x48
        } m_Slots[37]; // 0x0000 - [0..35] usable, [36] sentinel
        s32 m_TotalBytesCounter; // 0x14d0
    };
    static_assert(offsetof(cls, m_TotalBytesCounter) == 0x14d0);

    extern void (*init)(cls* self);
    extern void (*end)(cls* self);
    extern void (*initValue)(cls* self, s32 index);
    extern s32 (*setTextureData)(cls* self, const char* name, bool async);
    extern s32 (*setData)(cls* self, const char* name, bool async);
    extern s32 (*setData2)(cls* self, const char* name, const char* alias, bool async);
    extern s32 (*searchDataIndex)(cls* self, const char* name);
    extern s32 (*searchNullIndex)(cls* self);
    extern bool (*useTexture)(cls* self, s32 index);
    extern void (*delData)(cls* self, s32 index);
    extern ds::sys3d::CModelTexture::cls* (*getTex)(cls* self, s32 index);
    extern bool (*setupLnkTexVram)(cls* self, s32 index, bool async);
    extern bool (*releaseLnkTexVram)(cls* self, s32 index);
}

namespace CCharacterMng {
    struct cls
    {
        u8 unk[0x1cf0];
        CTextureDataMng::cls m_CTextureDataMng;
    };
    extern s32 (*getIndexOfSearchingCharacter)(cls* self);
    extern char* (*getCharacterName)(cls* self, s32 cid);
    extern void (*setHidden)(cls* self, s32 cid, bool hidden);
    extern void (*setShadowEnable)(cls* self, s32 cid, bool enabled);
    extern void (*setShadowType)(cls* self, s32 cid, s32 type);
    extern void (*setShadowAlphaRate)(cls* self, s32 cid, s32 rate);
    extern void (*setTransparencyRate)(cls* self, s32 cid, s32 rate);
    extern void (*setPosition)(cls* self, s32 cid, VecFx32 position);
    extern void (*setRotation)(cls* self, s32 cid, s16 x, s16 y, s16 z);
    extern void (*startMotion)(cls* self, s32 cid, s32, bool, u32);
    extern void (*setPause)(cls* self, s32, bool, u32);
    extern void (*setFrameRate)(cls* self, fx32 framerate);
}
extern CCharacterMng::cls* characterMng;

// Forward declarations for types referenced by pl::PlayerParty signatures but defined
// in translation units outside this dump (or later in this file).
namespace itm {
    enum WEAPON_SYSTEM
    {
    };

    // 4-category item static-data tables, loaded from "item_parameter.pak.lz".
    // All lookups are linear scans matching entry+0x02 (s16 item id).
    namespace ItemManager {
        struct cls
        {
            s32 m_ConsumptionCount; // 0x00
            s32 m_WeaponCount; // 0x04
            s32 m_ProtectionCount; // 0x08
            s32 m_ImportantCount; // 0x0c
            void* m_RawData; // 0x10 - owned, freed by free()
            void* m_Consumption; // 0x18 - chain 0, stride 0x30
            void* m_Weapon; // 0x20 - chain 1, stride 0x58
            void* m_Protection; // 0x28 - chain 2, stride 0x54
            void* m_Important; // 0x30 - chain 3, stride 0x20
        };
        static_assert(offsetof(cls, m_ConsumptionCount) == 0x00);
        static_assert(offsetof(cls, m_WeaponCount) == 0x04);
        static_assert(offsetof(cls, m_ProtectionCount) == 0x08);
        static_assert(offsetof(cls, m_ImportantCount) == 0x0c);
        static_assert(offsetof(cls, m_RawData) == 0x10);
        static_assert(offsetof(cls, m_Consumption) == 0x18);
        static_assert(offsetof(cls, m_Weapon) == 0x20);
        static_assert(offsetof(cls, m_Protection) == 0x28);
        static_assert(offsetof(cls, m_Important) == 0x30);

        extern void* (*consumptionParameter)(cls* self, s16 itemId);
        extern void* (*weaponParameter)(cls* self, s16 itemId);
        extern void* (*protectionParameter)(cls* self, s16 itemId);
        extern void* (*importantParameter)(cls* self, s16 itemId);
        extern void* (*equipParameter)(cls* self, s16 itemId); // weapon ∪ protection
        extern void* (*itemParameter)(cls* self, s16 itemId); // all 4 tables in order
        extern s32 (*itemCategory)(cls* self, s16 itemId); // 0..3 or -1
        extern u8 (*load)(cls* self); // panics if total ≥ 0x180 or important ≥ 0x80
        extern void (*free)(cls* self);

        extern cls* instance_;
    }

    // Party inventory. Two fixed-capacity arrays of 4-byte slots with separate
    // live counts. Capacity bounds (0x180 / 0x80) are enforced by storeItem /
    // storeImportantItem which panic with "vector size over." on overflow.
    // resetItemId / resetImportantItemId prune entries whose ids no longer
    // exist in itm::ItemManager (linear scan of all 4 ItemManager tables).
    namespace PossessionItemManager {
        // 4-byte inventory slot. Written as a single u32 by storeItem/storeImportantItem
        // (itemId | count<<16). Counts are clamped to [0, 99]. m_ReservedCount is set
        // by an external "reserve" routine and merged back into m_Count (capped at 99)
        // by reserveCancel(), which then zeroes m_ReservedCount.
        struct Entry
        {
            s16 m_ItemId; // 0x00
            u8 m_Count; // 0x02 - live stack size, clamped to [0, 99]
            u8 m_ReservedCount; // 0x03 - pending reservation, cleared by reserveCancel()
        };
        static_assert(sizeof(Entry) == 0x4);

        struct cls
        {
            Entry m_NormalItems[0x180]; // 0x000 - up to 384 normal items
            s32 m_NormalCount; // 0x600 - live count (low 16 bits used)
            Entry m_ImportantItems[0x80]; // 0x604 - up to 128 important items
            s32 m_ImportantCount; // 0x804 - live count (low 16 bits used)
        };
        static_assert(offsetof(cls, m_NormalItems) == 0x000);
        static_assert(offsetof(cls, m_NormalCount) == 0x600);
        static_assert(offsetof(cls, m_ImportantItems) == 0x604);
        static_assert(offsetof(cls, m_ImportantCount) == 0x804);
        static_assert(sizeof(cls) == 0x808);

        extern Entry* (*normalItem)(cls* self, s32 index); // panics if index >= m_NormalCount
        extern s32 (*itemAllNum)(cls* self); // m_NormalCount + m_ImportantCount
        extern void (*resetItemId)(cls* self); // prunes stale ids from m_NormalItems
        extern void (*initialize)(cls* self); // zeroes both counts
        extern Entry* (*importantItem)(cls* self, s32 index); // panics if index >= m_ImportantCount
        extern void (*storeItem)(cls* self, s16 itemId, s32 count); // merges or appends; panics if >= 0x180
        extern Entry* (*searchNormalItem)(cls* self, s16 itemId); // nullptr if not found
        extern s32 (*normalItemNum)(cls* self);
        extern s32 (*importantItemNum)(cls* self);
        extern void (*eraseAllNormalItem)(cls* self); // m_NormalCount = 0
        extern void (*reserveCancel)(cls* self); // merges m_ReservedCount into m_Count
        extern void (*resetImportantItemId)(cls* self); // prunes stale ids from m_ImportantItems
        extern void (*swapItem)(cls* self, s16 itemId1, s16 itemId2); // swaps two normal-item slots
        extern void (*eraseItem)(cls* self, s16 itemId, s32 count); // important if ItemManager cat byte == 0x1c
        extern void (*eraseAllImportantItem)(cls* self); // m_ImportantCount = 0
        extern void (*storeImportantItem)(cls* self, s16 itemId, s32 count); // merges or appends; panics if >= 0x80
        extern Entry* (*searchImportantItem)(cls* self, s16 itemId); // nullptr if not found
    }

    namespace EquipParameter {
        struct cls
        {
        };
        extern bool (*canEquip)(cls* self, u32);
    }
}

namespace common {
    enum ABILITY_ID : s8
    {
    };

    // Effect-data table loaded from "efficacy.beld.lz". Built as 127 hash buckets
    // (indexed by effectId & 0x7f), each a singly-linked list of HashNode entries
    // chained through HashNode::m_Next. m_Status: bit 0 = initialized, bit 1 = .beld loaded.
    namespace EfficacyDataConvection {
        // 16-byte hash-chain node stored in m_EffectData. m_Data points into m_RawData
        // at an EffectData record whose first 4 bytes are s32 m_EffectId; the rest of
        // that record is type-specific (parsed at load time from per-type cumulative
        // offsets in the BELD header).
        struct HashNode
        {
            void* m_Data; // 0x00 - -> EffectData record in m_RawData
            void* m_Next; // 0x08 - next node in bucket, or NULL
        };
        static_assert(sizeof(HashNode) == 0x10);

        struct cls
        {
            u16 m_Status; // 0x00 - bit 0: initialized; bit 1: .beld loaded
            u8 _pad002[6]; // 0x02
            void* m_RawData; // 0x08 - owned (ds::CHeap::alloc_app / free_app)
            HashNode* m_EffectData; // 0x10 - owned (malloc_count); array of 0x10-byte nodes
            HashNode** m_HashTable; // 0x18 - owned (malloc_count); 127-entry bucket head array
        };
        static_assert(offsetof(cls, m_Status) == 0x00);
        static_assert(offsetof(cls, m_RawData) == 0x08);
        static_assert(offsetof(cls, m_EffectData) == 0x10);
        static_assert(offsetof(cls, m_HashTable) == 0x18);

        extern void (*Ctor)(cls* self);
        extern void (*Dtor)(cls* self); // empty; real cleanup is in terminate()
        extern void (*initialize)(cls* self); // m_Status |= 1
        extern void (*loadBELD)(cls* self); // panics on bad magic ('BELD' = 0x444c4542) or wrong state
        extern void (*unloadBELD)(cls* self); // frees m_HashTable, m_EffectData, m_RawData; leaves m_Status
        extern void (*terminate)(cls* self); // unloadBELD + clears m_Status
        extern s32* (*getEfficacyData)(cls* self, s32 effectId); // ignores self; reads global instance_

        extern cls* instance_;
    }
}

namespace pl {
    // Forward declarations; layouts not part of this dump.
    namespace SummonAbilityList {
        struct cls;
        enum TYPES : u32
        {
        };
    }
    namespace LearningAbilityManager {
        struct cls;
    } // defined later in this file

    // 0x32-byte bit-flag storage backing LearningAbilityManager. Layout matches the
    // 6 m_Flags[i] pointers LearningAbilityManager stores (offsets 0x00, 0x20, 0x24,
    // 0x29, 0x2e, 0x30 with sizes 32/4/5/5/2/2). Each sub-array partitions the global
    // common::ABILITY_ID space:
    //   [0]→0x000..0x5db  [1]→0x5dc..0xf9f  [2]→0xfa0..0x1193
    //   [3]→0x1194..0x12bf  [4]→0x12c0..0x1323  [5]→0x1324..0x1333
    namespace LearningAbilityManagerContents {
        struct cls
        {
            u8 m_FlagsType0[0x20]; // 0x00 - 256 bits, includes the 5 magic-type ability bits
            u8 m_FlagsType1[0x04]; // 0x20 - 32  bits
            u8 m_FlagsType2[0x05]; // 0x24 - 40  bits
            u8 m_FlagsType3[0x05]; // 0x29 - 40  bits
            u8 m_FlagsType4[0x02]; // 0x2e - 16  bits
            u8 m_FlagsType5[0x02]; // 0x30 - 16  bits
        };
        static_assert(sizeof(cls) == 0x32);
    }

    enum PLAYER_TYPES : s32
    {
    };

    enum PARTY_FORMATION_TYPES : u8
    {
    };

    enum HAND_TYPE : u8
    {
    };

    enum EQUIP_POINTS : u8
    {
    };

    namespace Player {
        // Size 0x230 — confirmed by PlayerParty's 15-Player stride. Sub-object
        // offsets verified against the ctor and `equipmentAttach(HAND_TYPE) =
        // self + handType*0x1c + 0x1c8`.
        struct cls
        {
            u8 m_Enabled; // 0x000 - slot active fla g
            u8 _pad001[0x7]; // 0x001
            u8 m_PlayerAbilityManager[0x104]; // 0x008 - subobject; playerAbilityManager() returns &this
            PLAYER_TYPES m_PlayerType;
            ys::BodyParameter::cls m_BodyParameter;
            u8 m_PlayerEquipParameter[0x8]; // 0x118 - PlayerEquipParameter; equipParameter() returns &this
            s16* m_EquipmentItems; // 0x120 - s16[5] of equipment ids
            u8 m_EquipmentMagicBinder[0x50]; // 0x128 - EquipmentMagicBinder subobject
            u8 m_PhysicsAttackParam[0x2c]; // 0x178 - subobject (setPhysicsAttack / updateParameter)
            u8 m_PhysicsDefenseParam[0x12]; // 0x1a4 - subobject (calcPhysicsDefense)
            u8 m_Stats[5]; // 0x1b6 - 5 clamped stat bytes (STR/AGI/VIT/LUCK/INT)
            u8 _pad1bb; // 0x1bb
            s16 m_MagicDefense; // 0x1bc
            s16 m_MagicDefensePower; // 0x1be - magicDefensePower() returns (s32)this
            u8 _pad1c0[4]; // 0x1c0
            s32 cid; // 0x1c4 - character id; -1 when unassigned
            u8 m_EquipmentSymbol[2][0x1c]; // 0x1c8 - PlayerEquipmentSymbol[HAND_TYPE]
            u8 m_LearningAbilityManager[0x30]; // 0x200 - LearningAbilityManager; learningAbility() returns &this
        };
        static_assert(offsetof(cls, m_PlayerType) == 0x10c);
        static_assert(offsetof(cls, cid) == 0x1c4);
        static_assert(sizeof(cls) == 0x230);

        // Lifecycle
        extern void (*Ctor)(cls* self);
        extern void (*Dtor)(cls* self);
        extern void (*initialize)(cls* self, PLAYER_TYPES id);
        extern void (*initializeName)(cls* self);
        extern void (*initializeCommand)(cls* self);
        extern void (*initializeDefaultCommand)(cls* self);
        extern void (*updateParameter)(cls* self);
        extern void (*updateCondition)(cls* self);
        extern void (*recoverHPandMP)(cls* self);
        extern void (*useApple)(cls* self, bool arg);
        extern void (*useSomaDrop)(cls* self);
        extern void (*fine)(cls* self);

        // Sub-object accessors
        extern void* (*playerAbilityManager)(cls* self); // -> self+0x8
        extern LearningAbilityManager::cls* (*learningAbility)(cls* self); // -> self+0x200
        extern void* (*equipParameter)(cls* self); // -> self+0x118
        extern void* (*bodyAndBonus)(cls* self); // -> self+0x110 (ys::BodyParameter*)
        extern void* (*level)(cls* self); // -> saveParam+0
        extern void* (*hp)(cls* self); // -> saveParam+0xc
        extern void* (*mp)(cls* self); // -> saveParam+0x14
        extern void* (*exp)(cls* self);
        extern void* (*decantLevel)(cls* self); // -> saveParam+1
        extern ys::Condition::cls* (*condition)(cls* self); // -> saveParam+0x1c
        extern u64 (*saveParameter)(cls* self);

        // Stat growth
        extern void (*growParameter)(cls* self, u8 arg);
        extern u64 (*levelUp)(cls* self, s32 levels);
        extern bool (*levelDown)(cls* self, s32 levels);
        extern void (*setParameter)(cls* self, u8 current_lv, u8 next_lv);
        extern void (*setExp)(cls* self, u8 arg);
        extern void (*setHp)(cls* self, u8 arg);

        // Body parameter calculations
        extern void (*bodyAndBonus_BP)(cls* self, void* bodyParam, s16, s16, s16, s16, s16); // ys::BodyParameter&
        extern void (*calcBonus)(cls* self);
        extern s32 (*equipmentBounsHP)(cls* self);
        extern void (*abilityBouns)(cls* self, void* outBuf); // 16-byte sret
        extern void (*addEquipmentBonus)(cls* self, void* bodyParam, s32 arg); // ys::BodyParameter&
        extern void (*setPhysicsAttack)(cls* self);
        extern void (*calcPhysicsDefense)(cls* self);
        extern void (*calcMagicDefense)(cls* self);
        extern s32 (*magicDefensePower)(cls* self); // (s32)m_MagicDefensePower
        extern void (*physicsAttack)(cls* self, void* outParam, s16, s16, s16, s16, s16); // ys::PhysicsAttackParameter&
        extern void (*physicsDefense)(cls* self, void* outParam, s16, s16, s16, s16, s16); // ys::PhysicsDefenseParameter&
        extern void (*magicDefense)(cls* self, void* outParam, s16, s16, s16, s16, s16); // ys::MagicDefenseParameter&
        extern void (*addMagicDefense)(cls* self, void* outParam, s32 arg); // ys::MagicDefenseParameter&
        extern void (*addPhysicsDefense_ib)(cls* self, void* outParam, s32 arg, bool flag); // ys::PhysicsDefenseParameter&
        extern void (*addPhysicsDefense_Pb)(cls* self, void* outParam, void* protectionParam, bool flag); // itm::ProtectionParameter*
        extern u8 (*avoidance)(cls* self); // -> saveParam+0x1b8
        extern u8 (*deftness)(cls* self); // -> saveParam+0x1b8
        extern u16 (*defaultMaxHp)(cls* self);
        extern u16 (*defaultMaxMp)(cls* self);
        extern s32 (*calcHpColor)(s32 hp, s32 maxHp); // static
        extern s32 (*checkHpColor)(cls* self);
        extern bool (*isHealing)(cls* self);
        extern s32 (*difference)(cls* self, s32 a, s32 b);
        extern bool (*canWinningDirection)(cls* self);
        extern bool (*canGivenExp)(cls* self);
        extern u32 (*isFinishAttack)(cls* self);
        extern bool (*isUseMagicCharacter)(cls* self);

        // Command list
        extern void (*isCommand)(cls* self, common::ABILITY_ID abilityId);
        extern void (*isKnowledgeInCommand)(cls* self);
        extern void (*listupCommand)(cls* self, SummonAbilityList::cls& list);
        extern void (*listupCommandNormal)(cls* self, SummonAbilityList::cls& list);
        extern void (*listupCommandMagic)(cls* self, SummonAbilityList::cls& list, SummonAbilityList::TYPES type);
        extern void (*clearCommand)(cls* self);
        extern void (*stonePaletteName)(cls* self, char* out);
        extern void (*changeStonePalette)(cls* self, bool arg);
        extern void (*changeStonePaletteForEvent)(cls* self, bool arg);

        // Equipment
        extern bool (*canEquip)(cls* self, s16 itemId, HAND_TYPE hand);
        extern bool (*canEquipWeapon)(cls* self, s32 arg, void* weaponParam); // itm::WeaponParameter*
        extern bool (*canEquipBow)(cls* self, s32 arg, void* weaponParam, bool flag);
        extern bool (*canEquipArrow)(cls* self, s32 arg, void* weaponParam);
        extern bool (*canEquipHarp)(cls* self, s32 arg, void* weaponParam, bool flag);
        extern bool (*isCanEquipProtection)(cls* self, s32 arg, void* protectionParam);
        extern bool (*isHandCheck)(cls* self, s32 arg, void* weaponParam, void* protectionParam, bool flag1, bool flag2);
        extern bool (*isEquipable)(cls* self, EQUIP_POINTS point, s16 itemId);
        extern bool (*isEquipItem)(cls* self, void* equipParam); // itm::EquipParameter*
        extern void (*isEquippedBow)(cls* self);
        extern bool (*isEquipedItemOnPoint)(cls* self, s32 point, s16 itemId);
        extern bool (*isEquipedItemOnAllPoints)(cls* self, s16 itemId);
        extern void (*isEquipMetal)(cls* self);
        extern bool (*isHand)(cls* self, s32 arg, bool flag);
        extern bool (*doEquip)(cls* self, s32 point, s16 itemId, bool flag);
        extern void* (*equipmentAttach)(cls* self, HAND_TYPE hand); // -> self + hand*0x1c + 0x1c8
        extern void (*attachEquipmentSymbol)(cls* self, bool flag);
        extern void (*attachEquipmentSymbolOne)(cls* self, HAND_TYPE hand, bool flag);
        extern void (*removeEquipmentSymbol)(cls* self);
        extern void (*removeEquipmentSymbolOne)(cls* self, HAND_TYPE hand);
        extern void (*equipmentSymbolVisibility)(cls* self, bool flag);
        extern void (*updateEquipmentSymbol)(cls* self);
        extern void (*releaseEquipItem)(cls* self, s32 point);
        extern void (*adjustAbility)(cls* self);
        extern void (*equipMenuParameter)(cls* self, void* outBuf, s16, s16, s16, s16, s16); // 0x1c-byte sret

        // Abilities / magic
        extern u8 (*spendMp)(cls* self, void* magicParam); // common::BabilMagicParameter*
        extern bool (*isUseMagic_BM)(cls* self, void* magicParam, s32 arg, bool flag);
        extern void (*isUseMagic_AI)(cls* self, common::ABILITY_ID abilityId, s32 arg, bool flag);
        extern void (*learnAbility_ii)(cls* self, s32 arg1, s32 arg2);
        extern void (*learnAbility_AI)(cls* self, common::ABILITY_ID abilityId);
        extern void (*forgetAbility_i)(cls* self, s32 arg);
        extern void (*forgetAbility_AI)(cls* self, common::ABILITY_ID abilityId);
        extern void (*forgetLearnedMagicByEvent)(cls* self);
        extern void (*learnAbilityForDebug)(cls* self, common::ABILITY_ID abilityId);
        extern void (*learnAllAbilityForDebug)(cls* self);
        extern void (*learnAllWhiteMagicForDebug)(cls* self);
        extern void (*learnAllBlackMagicForDebug)(cls* self);
        extern void (*learnAllSummonForDebug)(cls* self);
        extern void (*learnAllSongForDebug)(cls* self);
        extern void (*learnAllNinjutsuForDebug)(cls* self);
        extern void (*forgetAllAbilityForDebug)(cls* self);
        extern void (*forgetAllWhiteMagicForDebug)(cls* self);
        extern void (*forgetAllBlackMagicForDebug)(cls* self);
        extern void (*forgetAllSummonForDebug)(cls* self);
        extern void (*forgetAllSongForDebug)(cls* self);
        extern void (*forgetAllNinjutsuForDebug)(cls* self);
    }
    namespace PlayerParty {
        // 0xc-byte slot, packed: m_StartCommand is an unaligned u64 at offset 0x4
        // (str x from 0x2350+i*0xc in clearBattleCommandPlayer).
        struct __attribute__((packed)) BattleCommand
        {
            u32 m_Field0;
            u64 m_StartCommand;
        };
        static_assert(sizeof(BattleCommand) == 0xc);

        // Holds 15 inline Players (stride 0x230 from +0x50), a PlayerSummon, a
        // CurrentAbilityIDList, save/active-member pointer pairs, chain-loaded lookup
        // tables, and 5 BattleCommands. Sub-objects with unknown layouts are raw byte
        // arrays so the static_asserts below stay verifiable.
        struct cls
        {
            Player::cls* m_SaveComposition[5]; // 0x000
            Player::cls* m_Members[5]; // 0x028 - active party, indexed 0..4 by "order"
            u8 m_Players[15 * 0x230]; // 0x050..0x2120 - 15 inline Players;
                                      //   m_Id offsets: 0x15c, 0x38c, 0x5bc, 0x7ec,
                                      //   0xa1c, 0xc4c, 0xe7c, 0x10ac, 0x12dc, 0x150c,
                                      //   0x173c, 0x196c, 0x1b9c, 0x1dcc, 0x1ffc
            u8 m_PlayerSummon[0x28]; // 0x2120
            u8 m_CurrentAbilityIDList[0x98]; // 0x2148
            void* m_RawChainData; // 0x21e0 - owned, freed by free()
            void* m_LearnChain0; // 0x21e8 - chain 0 (unused by dumped funcs)
            void* m_NormalAttack; // 0x21f0 - chain 1, stride 0x14
            void* m_NormalMagic; // 0x21f8 - chain 0x20, stride 0x20
            s16 m_NormalMagicCount; // 0x2200
            u8 _pad2202[6]; // 0x2202
            void* m_LearnAbilityData[15]; // 0x2208 - chains 2..0x10, per PLAYER_TYPES
            void* m_AbilityList; // 0x2280 - chain 0x21, stride 8, 0xe68 bytes
            void* m_LayoutSceneParameter; // 0x2288 - chain 0x22, stride 0x6c
            s32 m_LayoutSceneParameterCount; // 0x2290
            void* m_WeaponOffsetParameter; // 0x2298 - chain 0x23, stride 0x30
            void* m_LearnAbilityLookup[15]; // 0x22a0 - chains 0x11..0x1f, per PLAYER_TYPES
            s16 m_LearnAbilityCount[15]; // 0x2318
            u16 _pad2336; // 0x2336
            u32 m_NormalAttackCount; // 0x2338 - ceil(rawSize/0x14)
            void* m_AbilityBounsTable; // 0x2340 - chain 0x24, stride 0xa
            u32 m_AbilityBounsCount; // 0x2348 - ceil(rawSize/0xa)
            BattleCommand m_BattleCommands[5]; // 0x234c
        };
        static_assert(offsetof(cls, m_SaveComposition) == 0x000);
        static_assert(offsetof(cls, m_Members) == 0x028);
        static_assert(offsetof(cls, m_Players) == 0x050);
        static_assert(offsetof(cls, m_PlayerSummon) == 0x2120);
        static_assert(offsetof(cls, m_CurrentAbilityIDList) == 0x2148);
        static_assert(offsetof(cls, m_RawChainData) == 0x21e0);
        static_assert(offsetof(cls, m_NormalMagic) == 0x21f8);
        static_assert(offsetof(cls, m_NormalMagicCount) == 0x2200);
        static_assert(offsetof(cls, m_LearnAbilityData) == 0x2208);
        static_assert(offsetof(cls, m_AbilityList) == 0x2280);
        static_assert(offsetof(cls, m_LayoutSceneParameter) == 0x2288);
        static_assert(offsetof(cls, m_WeaponOffsetParameter) == 0x2298);
        static_assert(offsetof(cls, m_LearnAbilityLookup) == 0x22a0);
        static_assert(offsetof(cls, m_LearnAbilityCount) == 0x2318);
        static_assert(offsetof(cls, m_NormalAttackCount) == 0x2338);
        static_assert(offsetof(cls, m_AbilityBounsTable) == 0x2340);
        static_assert(offsetof(cls, m_AbilityBounsCount) == 0x2348);
        static_assert(offsetof(cls, m_BattleCommands) == 0x234c);

        extern Player::cls* (*member)(cls* self, PLAYER_TYPES id);
        extern s32 (*memberOrder)(cls* self, PLAYER_TYPES id);
        extern Player::cls* (*memberForOrder)(cls* self, u8 order);
        extern Player::cls* (*player)(cls* self, PLAYER_TYPES id);
        extern u8 (*isJoinedParty)(cls* self, PLAYER_TYPES id);
        extern bool (*isPartyFull)(cls* self);
        extern bool (*isEnableMember)(cls* self);
        extern s32 (*partyMemberEnableNumber)(cls* self);
        extern s32 (*aliveNumber)(cls* self);
        extern u32 (*averageLevel)(cls* self, PLAYER_TYPES id);
        extern u8 (*isFrogAll)(cls* self);
        extern u8 (*isLilliputAll)(cls* self);

        // Composition save/restore
        extern void (*saveComposition)(cls* self);
        extern void (*loadComposition)(cls* self);

        // Member management
        extern void (*setMemberForOrder)(cls* self, PLAYER_TYPES id, s32 order); // id==-1 → nullptr
        extern void (*changeMemberForOrder)(cls* self, u8 orderA, u8 orderB);
        extern void (*changeMember)(cls* self, PLAYER_TYPES idA, PLAYER_TYPES idB);
        extern Player::cls* (*addMember)(cls* self, PLAYER_TYPES id, s32 order); // order==-1 → first free
        extern u8 (*releaseMember)(cls* self, PLAYER_TYPES id);

        // Lifecycle
        extern void (*Ctor)(cls* self);
        extern void (*Dtor)(cls* self);
        extern void (*initialize)(cls* self);
        extern void (*initForNewgame)(cls* self, bool keepPlotItems);
        extern void (*initializeName)(cls* self);
        extern u8 (*load)(cls* self); // "player.chaindata.lz"
        extern void (*free)(cls* self);
        extern void (*fineAll)(cls* self);
        extern void (*changeStonePalette)(cls* self);
        extern void (*changeStonePaletteForEvent)(cls* self);

        // Battle commands
        extern void (*clearBattleCommand)(cls* self);
        extern void (*clearBattleCommandPlayer)(cls* self, s32 index);
        extern void (*setSaveStartCommand)(cls* self, s32 slot, s32 val);
        extern u8 (*isCommand)(cls* self, common::ABILITY_ID abilityId);
        extern void (*listupCommand)(cls* self, SummonAbilityList::cls& list);
        extern void (*clearBattleCondition)(cls* self);
        extern void (*addItem)(cls* self, s32 itemId, s32 qty);

        // Formation (forward to sys::GameParameter)
        extern u32 (*formation)(cls* self, u8 index);
        extern void (*setFormation)(PARTY_FORMATION_TYPES formation);
        extern void (*changeFormation)();

        // Static-data lookups
        extern void* (*levelParameter)(cls* self, PLAYER_TYPES playerType, s8 level); // panics if level > 99
        extern void* (*learningAbility)(cls* self, PLAYER_TYPES playerType, s32 index);
        extern bool (*isLearnByLevelUp)(cls* self, PLAYER_TYPES playerType, common::ABILITY_ID abilityId);
        extern s16* (*normalMagic)(cls* self, s32 id);
        extern s16* (*normalAttack)(cls* self, s32 id);
        extern void* (*abilityList)(cls* self, s32 id);
        extern s16* (*layoutSceneParameter)(cls* self, s32 id);
        extern void* (*weaponOffsetParameter)(cls* self, itm::WEAPON_SYSTEM weaponSystem);
        extern s16* (*abilityBouns)(cls* self, common::ABILITY_ID abilityId);

        extern cls* playerPartyInstance_;
        extern Player::cls* InvalidPlayer;
    }

    // Per-character learned-ability bit-flag manager. 6 categories map to ability-id
    // ranges (learn/isLearning/forget/learnForDebug dispatch on the id):
    //   type 0: 0x000..0x5db    type 1: 0x5dc..0xf9f    type 2: 0xfa0..0x1193
    //   type 3: 0x1194..0x12bf  type 4: 0x12c0..0x1323  type 5: 0x1324..0x1333
    // learn()/learnForDebug() also enforce a 3-magic-types cap for ids in
    // {0x5, 0x6, 0xd, 0x12, 0x53} (the 0x42060 bitmask + explicit 0x53 check),
    // checked via learningMagicTypesNumber().
    namespace LearningAbilityManager {
        enum LEARNING_ABILITY_TYPES : u32
        {
        };

        struct cls
        {
            u8* m_Flags[6]; // 0x00..0x2c - per-category bit-array pointers into Contents
                            //   [0]→+0x00 (32B) [1]→+0x20 (4B) [2]→+0x24 (5B)
                            //   [3]→+0x29 (5B)  [4]→+0x2e (2B) [5]→+0x30 (2B)
        };
        static_assert(sizeof(cls) == 0x30);

        extern void (*initialize)(cls* self, LearningAbilityManagerContents::cls& contents); // setContents + zero
        extern void (*setContents)(cls* self, LearningAbilityManagerContents::cls& contents);
        extern void (*on)(cls* self, LEARNING_ABILITY_TYPES type, s32 flagIndex);
        extern void (*off)(cls* self, LEARNING_ABILITY_TYPES type, s32 flagIndex);
        extern u8 (*isOn)(cls* self, LEARNING_ABILITY_TYPES type, s32 flagIndex);
        extern void (*clear)(cls* self, LEARNING_ABILITY_TYPES type);
        extern bool (*isMagicTypesAbility)(cls* self, common::ABILITY_ID abilityId);
        extern s8 (*learningMagicTypesNumber)(cls* self); // 0..5; counts bits in cat-0 array
        extern void (*learn)(cls* self, common::ABILITY_ID abilityId); // checks AbilityManager + 3-magic cap
        extern void (*learnForDebug)(cls* self, common::ABILITY_ID abilityId); // skips isEnableLearn
        extern void (*forget)(cls* self, common::ABILITY_ID abilityId);
        extern u8 (*isLearning)(cls* self, common::ABILITY_ID abilityId);

        // Per-category sizes/offsets (FLAG_OFFSET is referenced by asm but not by any
        // dumped function's pseudocode).
        extern u32* FLAG_SIZE;
        extern u32* ARRAY_SIZE;
        extern u32* FLAG_OFFSET;
    }
}

namespace menu {
    extern void (*playSECommon)(s32 seqArcNo, s32 index);
}

extern int (*CoreAudioOutInit)(int, int, uintptr_t callback);
extern int (*CoreAudioOutKick)();
extern int (*CoreAudioOutQueueBuffer)(void* buffer, size_t size);
extern void (*CoreAudioOutRelease)();
extern int (*CoreAudioOutResume)();
extern int (*CoreAudioOutSuspend)();
extern void (*NNS_SndUpdate)();
extern void (*NNS_SndPlayerSetPlayerVolume)(int playerNo, int volume);

namespace egs {
    extern void (*GilbartInitialize)(bool, bool isDoubleVoice);
    extern void (*GilbartTerminate)();
    extern bool (*GilbartIsPlaying)(int index);
    extern bool (*GilbartIsEndOfPlayVoice)(int index);
    extern void (*GilbartSetVolume)(int index, float volume);
    extern void (*GilbartPauseVoice)(int index);
    extern void (*GilbartPlayVoice)(const char* file, int index);
    extern void (*GilbartStopVoice)(int index);
    extern int (*GilbartStatus)(int index);
    extern void (*GilbartProcess)(void*);
}

namespace Font {
    struct cls
    {
        GLuint* textures;
        u8 unk[0x10];
        void* buf;
    };
    extern void (*initFont)(cls* self);
}

namespace dgs {
    struct DGSMSD
    {
    };
    inline dgs::DGSMSD* DGSMSD_ALL = reinterpret_cast<dgs::DGSMSD*>(-1);
    extern NNSG2dChar* (*DGSMsdGetString)(u32 msd, s8 line, DGSMSD* dgsmsd);
}

namespace title {
    namespace TitleContents {
        struct cls
        {
            u8 unk[0x1550];
            u32 m_ContentMask;
            u32 m_FocusedIndex;
        };
        extern void (*update)(cls* self);
    }
    namespace TitleSubState {
        struct cls
        {
            TitleContents::cls* m_TitleContents;
            u8 unk[0x4];
            u32 m_HoveredIdx;
        };
        extern void (*update)(cls* self);
    }
}

namespace part {
    namespace WorldPart {
        struct cls
        {
            u8 unk[0x440];
            world::WorldStateContext::cls m_WorldStateContext;
        };
        extern cls* wpInstance_;
    }
    namespace TitlePart {
        struct cls
        {
            u8 unk[0x10];
            title::TitleSubState::cls m_TitleSubState;
        };
        extern cls* tpInstance_;
    }
}

namespace FlagManager {
    struct cls
    {
        u8 m_Banks[3][1000];
    };
    static_assert(sizeof(cls) == 3000);

    extern cls* (*singleton)();
    extern void (*Ctor)(cls* self);
    extern void (*Dtor)(cls* self);
    extern bool (*get)(cls* self, u32 bank, u32 offset);
    extern void (*set)(cls* self, u32 bank, u32 offset);
    extern void (*reset)(cls* self, u32 bank, u32 offset);
    extern void (*reverse)(cls* self, u32 bank, u32 offset);
    extern cls* _instance;
}

enum enAchievementCheckTimings : u32
{
    enAchievementCheckTimings_EventStart     = BIT(0),
    enAchievementCheckTimings_EventFinish    = BIT(1),
    enAchievementCheckTimings_PlayNewGame    = BIT(2),
    enAchievementCheckTimings_FreeMove       = BIT(3),
    enAchievementCheckTimings_FreeVehicle    = BIT(4),
    enAchievementCheckTimings_BattleEnd      = BIT(5),
    enAchievementCheckTimings_Reported       = BIT(6),
    enAchievementCheckTimings_UseAbilityItem = BIT(7),
    enAchievementCheckTimings_ShopSell       = BIT(8),
};

enum enAchievementCheckTypes : u32
{
    enAchievementCheckTypes_Flag              = 0,
    enAchievementCheckTypes_FlagRange         = 1,
    enAchievementCheckTypes_HardModeBeat      = 2,
    enAchievementCheckTypes_BeatCount         = 3,
    enAchievementCheckTypes_GetItem           = 4,
    enAchievementCheckTypes_GetDecant         = 5,
    enAchievementCheckTypes_GetDecantCnt      = 6,
    enAchievementCheckTypes_LearnSummon       = 7,
    enAchievementCheckTypes_LearnSummonCnt    = 8,
    enAchievementCheckTypes_CompletedMap      = 9,
    enAchievementCheckTypes_OverPlayerLevel   = 10,
    enAchievementCheckTypes_Gill              = 11,
    enAchievementCheckTypes_MonsterKillCount  = 12,
    enAchievementCheckTypes_MonsterDictionary = 13,
    enAchievementCheckTypes_MonsterPartyKill  = 14,
    enAchievementCheckTypes_NoCheck           = 15,
    enAchievementCheckTypes_Field             = 16,
    enAchievementCheckTypes_AchievementComp   = 17
};

struct AchievementParam
{
    char m_Identifier[0x40];
    enAchievementCheckTimings m_Timing;
    enAchievementCheckTypes m_Type;
    struct FlagParam
    {
        s32 m_Bank;
        s32 m_Offset;
        bool m_Value;
    };
    union {
        s32 m_Data[0x3e];
        FlagParam m_FlagParam[20];

        struct
        {
            s32 m_Bank;
            s32 m_Start;
            s32 m_End;
            bool m_Value;
        } m_FlagRangeParam[14];

        struct
        {
            s32 m_MonsterPartyId;
        } m_HardModeBeatParam;

        struct
        {
            s32 m_MonsterPartyId;
            s32 m_Count;
        } m_BeatCountParam;

        struct
        {
            s32 m_ItemId;
        } m_GetItemParam[60];

        struct
        {
            s32 m_DecantId;
        } m_GetDecantParam[60];

        struct
        {
            s32 m_Count;
        } m_GetDecantCntParam;

        struct
        {
            pl::PLAYER_TYPES m_Player;
            common::ABILITY_ID m_AbilityId;
        } m_LearnSummonParam[30];

        struct
        {
            FlagParam m_Flags[2];
            u8 padding[0xc];
            pl::PLAYER_TYPES m_Player;
            s32 m_Count;
        } m_LearnSummonCntParam;

        struct
        {
        } m_CompletedMapParam;

        struct
        {
            pl::PLAYER_TYPES m_Player;
            u32 m_Level;
        } m_OverPlayerLevelParam[30];

        struct
        {
            u32 m_Amount;
        } m_GillParam;

        struct
        {
            u32 m_Count;
        } m_MonsterKillCountParam;

        struct
        {
            u32 m_Amount;
        } m_MonsterDictionaryParam;

        struct
        {
            u32 m_MonsterPartyId;
        } m_MonsterPartyKillParam[60];

        struct
        {
        } m_NoCheckParam;

        struct
        {
            FlagParam m_Flags[2];
            u8 padding[0xc];
            s32 m_FieldType;
            s32 m_FieldMatch1;
            s32 m_FieldMatch2;
        } m_FieldParam;

        struct
        {
        } m_AchievementCompParam;
    };
};
static_assert(sizeof(AchievementParam) == 0x140);

enum AchievementCheckTiming : u32
{
    AchievementCheckTiming_Always = 0,
};

namespace AchievementContext {
    struct cls
    {
    };

    extern u8* (*getArchiveReportArray)(cls* self);
    extern cls* instance_;
}

// AchievementResource - holds the AchievementParam[] array. Helpers (getNum, etc.)
// are only reachable via PLT stubs at 0x00564570/0x00564580/0x005645a0/0x005645b0.
namespace AchievementResource {
    struct cls
    {
        void* m_Params;
        s32 m_Field8;
    };
}

// AchievementReporter - 0x100c bytes. Pending-report queue submitted to Google Play.
namespace AchievementReporter {
    enum ReportState : u32
    {
        ReportState_Pending  = 0,
        ReportState_Reported = 1,
        ReportState_Finished = 2,
    };

    struct ReportEntry
    {
        u32 m_State;
        u32 _pad;
        AchievementParam* m_Param;
    };
    static_assert(sizeof(ReportEntry) == 0x10);

    struct cls
    {
        ReportEntry m_Entries[0x100]; // 0x00
        u32 m_Count; // 0x1000
        u32 _reserved1004; // 0x1004
        s32 m_RetryFrames; // 0x1008
    } __attribute__((packed));
    static_assert(sizeof(cls) == 0x100c);
    static_assert(offsetof(cls, m_Count) == 0x1000);
    static_assert(offsetof(cls, m_RetryFrames) == 0x1008);

    extern void (*Ctor)(cls* self);
    extern void (*Dtor)(cls* self);
    extern void (*init)(cls* self);
    extern void (*update)(cls* self);
    extern void (*addParam)(cls* self, AchievementParam& param);
    extern int (*finish)(cls* self, const char* achievementId);
    extern bool (*isAllReported)(cls* self);
    extern bool (*isReportedSecondReport)(cls* self);
    extern void (*eraseReported)(cls* self);
    extern void (*syncSaveData)(cls* self, AchievementResource::cls* resource);
}

// AchievementCheckFuncs - 0x138 bytes. m_Funcs[18] order: flag, flagRange,
// hardModeBeat, beatCount, getItem, getDecant, getDecantCnt, learnSummon,
// learnSummonCnt, completedMap, overPlayerLevel, gill, monsterKillCount,
// monsterDictionary, monsterKillParty, noCheck, field, achievementComp.
// FuncEntry.flags low bit = indirect (funcPtr is a vtable base, flags>>1 is offset).
namespace AchievementCheckFuncs {
    struct cls
    {
        void* m_AchievementResource;
        struct FuncEntry
        {
            void* funcPtr;
            uintptr_t flags;
        } m_Funcs[18];
        s32 m_GetDecantCntCache; // 0x128
        s32 m_MonsterKillCountCache; // 0x12c
        s32 m_MonsterDictionaryCache; // 0x130
    };
    static_assert(offsetof(cls, m_Funcs) == 0x08);
    static_assert(sizeof(cls::FuncEntry) == 0x10);
    static_assert(sizeof(cls::m_Funcs) == 0x120);
    static_assert(offsetof(cls, m_GetDecantCntCache) == 0x128);
    static_assert(offsetof(cls, m_MonsterKillCountCache) == 0x12c);
    static_assert(offsetof(cls, m_MonsterDictionaryCache) == 0x130);

    extern s32* DecantItemTbl;

    extern void (*Ctor)(cls* self);
    extern void (*Dtor)(cls* self);
    extern void (*init)(cls* self, AchievementResource::cls* resource);
    extern void (*clearCacheParam)(cls* self);
    extern u32 (*callFunc)(cls* self, AchievementParam* param);

    extern void (*getDecantItemName)(s32 itemId, char* outBuf);

    extern u32 (*flag)(cls* self, AchievementParam* param);
    extern u32 (*flagRange)(cls* self, AchievementParam* param);
    extern u32 (*hardModeBeat)(cls* self, AchievementParam* param);
    extern u32 (*beatCount)(cls* self, AchievementParam* param);
    extern u32 (*getItem)(cls* self, AchievementParam* param);
    extern u32 (*getDecant)(cls* self, AchievementParam* param);
    extern u32 (*getDecantCnt)(cls* self, AchievementParam* param);
    extern u32 (*learnSummon)(cls* self, AchievementParam* param);
    extern u32 (*learnSummonCnt)(cls* self, AchievementParam* param);
    extern u32 (*completedMap)(cls* self, AchievementParam* param);
    extern u32 (*overPlayerLevel)(cls* self, AchievementParam* param);
    extern u32 (*gill)(cls* self, AchievementParam* param);
    extern u32 (*monsterKillCount)(cls* self, AchievementParam* param);
    extern u32 (*monsterDictionary)(cls* self, AchievementParam* param);
    extern u32 (*monsterKillParty)(cls* self, AchievementParam* param);
    extern u32 (*noCheck)(cls* self, AchievementParam* param);
    extern u32 (*field)(cls* self, AchievementParam* param);
    extern u32 (*achievementComp)(cls* self, AchievementParam* param);
}

// AchievementChecker - 0x18 bytes. Ghidra mislabels the fields; asm writes
// x1->[0x00], x2->[0x10], x3->[0x08].
namespace AchievementChecker {
    struct cls
    {
        AchievementReporter::cls* m_AchievementReporter;
        AchievementCheckFuncs::cls* m_AchievementCheckFuncs;
        AchievementResource::cls* m_AchievementResource;
    };
    static_assert(sizeof(cls) == 0x18);

    extern void (*Ctor)(cls* self);
    extern void (*Dtor)(cls* self);
    extern void (*init)(cls* self, AchievementReporter::cls* reporter,
        AchievementResource::cls* resource,
        AchievementCheckFuncs::cls* checkFuncs);
    extern void (*update)(cls* self, AchievementCheckTiming timing);
}

namespace mon {
    // Per-monster bestiary entry (2 bytes). kill() caps m_KillCount at 999 and clears
    // bits 14..15 via `& 0x3fff`.
    struct MonsterMania
    {
        union {
            u16 m_Raw;
            struct
            {
                u16 m_Encountered : 1; // bit 0  - set with m_EncounteredAlt by onEncount
                u16 m_EncounteredAlt : 1; // bit 1
                u16 m_InLibrary : 1; // bit 2  - set by onLibrary
                u16 _flag3 : 1; // bit 3
                u16 m_KillCount : 10; // bits 4..13
                u16 _reserved : 2; // bits 14..15
            } m_Bits;
        };
    };
    static_assert(sizeof(MonsterMania) == 2);

    // 0x200-byte flat array of MonsterMania, indexed by monster id (0..0xff). The on*()
    // functions fan out updates to all members of a monster family — e.g. onEncount(0x60)
    // marks slots 0x60, 0xe4..0xe9.
    namespace MonsterManiaManager {
        struct cls
        {
            MonsterMania m_Entries[0x100]; // 0x000
        };
        static_assert(sizeof(cls) == 0x200);

        extern MonsterMania* (*monsterMania)(cls* self, s32 index); // panics if index >= 0x100
        extern void (*onEncount)(cls* self, s16 monsterId);
        extern void (*onLibrary)(cls* self, s16 monsterId);
        extern void (*kill)(cls* self, s16 monsterId); // +0x10 per call, capped at 999
        extern void (*clearMonsterMania)(cls* self);
        extern u8 (*setMonsterManiaForMonsterID)(MonsterMania& mania); // stub, always returns 1
    }

    // Owner of all monster static-data tables. load() decompresses "monster.chaindata.lz"
    // and carves it into 11 sub-tables via pack::ChainPointer. Counts are stored in
    // chain-index order; data pointers follow the original struct field order. Chain 3
    // is unused (count slot at +0x7c stays 0, no data pointer carved).
    namespace MonsterManager {
        struct cls
        {
            void* m_RawData; // 0x00 - owned, freed by free()
            void* m_MonsterParameter; // 0x08 - chain 0,  stride 0x98, id at +0x08 (s16)
            void* m_DropItem; // 0x10 - chain 1,  stride 0x0a, id at +0x00 (s16)
            void* m_NormalAttack; // 0x18 - chain 2,  stride 0x1c, indexed (not id-keyed)
            void* m_Offset; // 0x20 - chain 4,  stride 0x54, id at +0x00 (s16)
            void* m_EffectsInfoAlt; // 0x28 - chain 5,  stride 0x44, id at +0x00 (s16)
            void* m_EffectsInfo; // 0x30 - chain 11, stride 0x48, id at +0x00 (s16), sub at +0x44 (u16)
            void* m_OctmanmosLegInfo; // 0x38 - chain 6,  stride 0x1c, id0 at +0x00 (u8), id1 at +0x01 (u8)
            void* m_TurnAction; // 0x40 - chain 8,  stride 0x2c, id at +0x00 (s16)
            void* m_ActionCondition; // 0x48 - chain 9,  stride 0x0c, id at +0x00 (s16)
            void* m_Counter; // 0x50 - chain 10, stride 0x0e, id at +0x00 (s16)
            void* m_Ai; // 0x58 - chain 7,  stride 0x16, id at +0x00 (s16)
            u32 m_TurnActionCount; // 0x60
            u32 m_ActionConditionCount; // 0x64
            u32 m_CounterCount; // 0x68
            u32 m_AiCount; // 0x6c
            u32 m_MonsterParameterCount; // 0x70
            u32 m_DropItemCount; // 0x74
            u32 m_NormalAttackCount; // 0x78
            u32 _pad07c; // 0x7c - chain 3 (unused)
            u32 m_OffsetCount; // 0x80
            u32 m_EffectsInfoAltCount; // 0x84
            u32 m_EffectsInfoCount; // 0x88
            u32 m_OctmanmosLegInfoCount; // 0x8c
        };
        static_assert(offsetof(cls, m_RawData) == 0x00);
        static_assert(offsetof(cls, m_Ai) == 0x58);
        static_assert(offsetof(cls, m_TurnActionCount) == 0x60);
        static_assert(offsetof(cls, m_OctmanmosLegInfoCount) == 0x8c);
        static_assert(sizeof(cls) == 0x90);

        extern u8 (*load)(cls* self); // "monster.chaindata.lz"
        extern void (*free)(cls* self);
        extern void* (*monsterParameter)(cls* self, s32 monsterId);
        extern s16* (*dropItem)(cls* self, s32 itemId);
        extern s16* (*ai)(cls* self, s32 id); // panics if id < 0
        extern s16* (*counter)(cls* self, s32 id); // panics if id < 0
        extern s16* (*turnAction)(cls* self, s32 id); // panics if id < 0
        extern s16* (*actionCondition)(cls* self, s32 id); // panics if id < 0
        extern s16* (*offset)(cls* self, s32 id);
        extern void* (*normalAttack)(cls* self, s32 index);
        extern u8* (*octmanmosLegInfo)(cls* self, u8 legId, u8 partId);
        extern s16* (*effectsInfo)(cls* self, common::ABILITY_ID abilityId, s16 subId); // primary then alt table
        extern void (*acceptMonsterManiaData)(cls* self, MonsterManiaManager::cls& src); // self ignored
        extern MonsterManiaManager::cls* (*monsterManiaManager)();
        extern void (*sendMonsterManiaData)(cls* self, MonsterManiaManager::cls& dest); // self ignored
        extern void (*setMonsterIdForMonsterManaia)(); // stub

        extern cls* instance_;
    }
}

namespace mr {
    // Monster reference table loaded from "monster_refer.bbd.lz". Entries are 0x20 bytes
    // with the monster id as the first s16 (entry+0x00). m_Count = ceil(rawSize / 0x20).
    namespace MonsterRefDataManager {
        struct cls
        {
            void* m_RawData; // 0x00 - owned, freed by free()
            u32 m_Count; // 0x08
            u8 _pad0c[4]; // 0x0c
        };
        static_assert(offsetof(cls, m_RawData) == 0x00);
        static_assert(offsetof(cls, m_Count) == 0x08);
        static_assert(sizeof(cls) == 0x10);

        extern void (*Ctor)(cls* self); // zeroes m_RawData only
        extern void (*Dtor)(cls* self); // empty
        extern void (*load)(cls* self); // "monster_refer.bbd.lz"
        extern void (*free)(cls* self); // frees m_RawData, zeroes both fields
        extern s16* (*monsterReference)(cls* self, s32 monsterId);
        extern void* (*monsterReferenceFromIndex)(cls* self, s32 index); // nullptr if out of range
    }

    extern u32 (*getCompleteRate)();
    extern MonsterRefDataManager::cls** MRDMng;
}

namespace debug {
    namespace CharaViewerPart {
        struct cls
        {
        };
        extern void (*doInitialize)(cls* self);
    }
}

namespace evt {
    namespace EventCamera {
        struct cls
        {
        };
        extern void (*startCameraMotion)(cls* self, u32, u32, u32, bool);
    }
}

void init();
}