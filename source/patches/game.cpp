#include "../game.h"

#include <math.h>

#include <cstdint>
#include <cstring>
#define DA_NOSTRUCT
#include <disarm64.h>

#include "../base64.h"
#include "../config.h"
#include "../error.h"
#include "../pad_manager.h"
#include "../patches.h"
#include "../types.h"
#include "../util.h"
#include "../vibration.h"

namespace patches::game {
static uint8_t fake_tls[0x200];
babil::world::WSMove::cls* WSMoveInstance_           = nullptr;
babil::world::WorldStateScheduler::cls* wssInstance_ = nullptr;
s32 g_NextSymbol                                     = -1;
s32 g_LastBattlePlayerId                             = -1;
babil::Font::cls* FontInstance_                      = nullptr;

static s32 achievementParamId(const babil::AchievementParam* param) {
    if (param == nullptr || param->m_Identifier[0] == '\0')
        return -1;
    char* decoded = b64decode(param->m_Identifier, strlen(param->m_Identifier));
    if (decoded == nullptr)
        return -1;
    return static_cast<u8>(decoded[12]);
}

void OSi_Panic(const char* file, int line, const char* message) {
    if (message && strcmp(message, "\nMiss! Item Num Over.\n") == 0) {
        debugPrintf("Bypassing item limit panic! (Line: 0x%X)\n", line);
        return;
    }

    debugBacktrace();
    fatal_error("\n================ !!! OSi_Panic !!! ================\n"
                "File:    %s\n"
                "Line:    0x%X (%d)\n"
                "Reason:  %s\n"
                "===================================================\n",
        file, line, line, message);
}

namespace sys {
    namespace GGlobal {
        DECLARE_TRAMPOLINE(babil::sys::GGlobal::setNextPart, setNextPart_t);
        void setNextPart(u32 part) {
            debugPrintf("setNextPart(%d)\n", part);
            auto currentPart = babil::sys::GGlobal::getCurrentPart();
            if (currentPart == babil::BabilGamePart_MoviePart) {
                return setNextPart_t(babil::BabilGamePart_TitlePart);
            }
            if (config::game_startup_part > -1) {
                if (currentPart == babil::BabilGamePart_BackupRomFormatPart && part == babil::BabilGamePart_CompanyLogoPart) {
                    return setNextPart_t(config::game_startup_part);
                }
            }
            return setNextPart_t(part);
        }
    }

    namespace GameParameter {
        DECLARE_TRAMPOLINE(babil::sys::GameParameter::isDash, isDash_t);
        u8 isDash() {
            if (config::better_stick_movement) {
                HidAnalogStickState l = padGetStickPos(&pad_manager::g_Pads[pad_manager::g_CurrentPad], 0);
                HidAnalogStickState r = padGetStickPos(&pad_manager::g_Pads[pad_manager::g_CurrentPad], 1);

                s32 l_dist = l.x * l.x + l.y * l.y;
                s32 r_dist = r.x * r.x + r.y * r.y;
                if (l_dist > 700000000 || r_dist > 700000000)
                    return true;
                if (l_dist > 900000 || r_dist > 900000)
                    return false;
            }
            return isDash_t();
        }
        // DECLARE_TRAMPOLINE(babil::sys::GameParameter::gameClearCount, gameClearCount_t);
        // u8 gameClearCount(babil::sys::GameParameter::cls* self) {
        //     return 4;
        //     // return gameClearCount_t(self);
        // }
    }
}

std::string utf16_to_utf8(const char16_t* src) {
    std::string out;
    if (!src)
        return out;

    while (*src) {
        char32_t cp = *src++;

        if (cp >= 0xD800 && cp <= 0xDBFF && *src >= 0xDC00 && *src <= 0xDFFF) {
            cp = (cp << 10) + *src++ - 0x35FDC00;
        }

        if (cp < 0x80) {
            out += (char)cp;
        } else if (cp < 0x800) {
            out += (char)(0xC0 | (cp >> 6));
            out += (char)(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            out += (char)(0xE0 | (cp >> 12));
            out += (char)(0x80 | ((cp >> 6) & 0x3F));
            out += (char)(0x80 | (cp & 0x3F));
        } else {
            out += (char)(0xF0 | (cp >> 18));
            out += (char)(0x80 | ((cp >> 12) & 0x3F));
            out += (char)(0x80 | ((cp >> 6) & 0x3F));
            out += (char)(0x80 | (cp & 0x3F));
        }
    }
    return out;
}

namespace ui {
    namespace CWidgetMng {
        DECLARE_TRAMPOLINE(babil::ui::CWidgetMng::addWidget, addWidget_t);
        void addWidget(babil::ui::CWidgetMng::cls* self, s32 id, s32 x, s32 y, s32 width, s32 height, s32 text_color, s32 msd, s32 flags) {
            // debugPrintf("addWidget: id: 0x%X, x: %d, y: %d, width: %d, height: %d, text_color: %d, msd: %d, flags: %d\n", id, x, y, width, height, text_color, msd, flags);
            if (msd != 0) {
                std::string string = utf16_to_utf8(babil::dgs::DGSMsdGetString(msd, 0, babil::dgs::DGSMSD_ALL)).c_str();
                // debugPrintf("msd: %s\n", string.c_str());
                if (config::hide_back_button && string == "Back")
                    return;
            }

            // if (flags == 10 && id == 0x7) {
            //     return addWidget_t(self, id, x - 44, y, width + 90, height + 36, text_color, msd, flags);
            // }
            addWidget_t(self, id, x, y, width, height, text_color, msd, flags);
        }

        DECLARE_TRAMPOLINE(babil::ui::CWidgetMng::setScroll, setScroll_t);
        void setScroll(babil::ui::CWidgetMng::cls* self, int a, u32 top, u32 height) {
            setScroll_t(self, a, top, height);
        }

        DECLARE_TRAMPOLINE(babil::ui::CWidgetMng::setCursor, setCursor_t);
        void setCursor(babil::ui::CWidgetMng::cls* self, s32 newWidgetId, s32 anchorWidgetId, s32 msd, u32 flags, bool updateCursorTable) {
            debugPrintf("ui::CWidgetMng::setCursor 0x%X\n", newWidgetId);
            // debugBacktrace();
            setCursor_t(self, newWidgetId, anchorWidgetId, msd, flags, updateCursorTable);
        }
    }
}

namespace world {
    DECLARE_TRAMPOLINE(babil::world::disableSPSetting, disableSPSetting_t);
    void disableSPSetting(babil::world::WorldStateContext::cls* self) {
        if (!::game::g_saveAnywhere) {
            disableSPSetting_t(self);
        }
    }

    DECLARE_TRAMPOLINE(babil::world::searchNextSymbolByCharacterIDBackward, searchNextSymbolByCharacterIDBackward_t);
    s32 searchNextSymbolByCharacterIDBackward(s32 id) {
        if (g_NextSymbol != -1) {
            s32 out      = g_NextSymbol;
            g_NextSymbol = -1;
            return out;
        }
        return searchNextSymbolByCharacterIDBackward_t(id);
    }

    DECLARE_TRAMPOLINE(babil::world::searchNextSymbolByCharacterIDForward, searchNextSymbolByCharacterIDForward_t);
    s32 searchNextSymbolByCharacterIDForward(s32 id) {
        if (g_NextSymbol != -1) {
            s32 out      = g_NextSymbol;
            g_NextSymbol = -1;
            return out;
        }
        return searchNextSymbolByCharacterIDForward_t(id);
    }

    DECLARE_TRAMPOLINE(babil::world::canChangeSymbol, canChangeSymbol_t);
    bool canChangeSymbol() {
        bool out = canChangeSymbol_t();
        debugPrintf("canChangeSymbol() = %s\n", out ? "true" : "false");
        return out;
    }

    namespace WorldStateScheduler {
        DECLARE_TRAMPOLINE(babil::world::WorldStateScheduler::wssAddStateSchedule, wssAddStateSchedule_t);
        bool wssAddStateSchedule(babil::world::WorldStateScheduler::cls* self, const char* state) {
            debugPrintf("wssAddStateSchedule(%s)\n", state);
            return wssAddStateSchedule_t(self, state);
        }

        DECLARE_TRAMPOLINE(babil::world::WorldStateScheduler::wssInitialize, wssInitialize_t);
        void wssInitialize(babil::world::WorldStateScheduler::cls* self) {
            debugPrintf("wssInitialize(%p)\n", self);
            wssInstance_ = self;
            return wssInitialize_t(self);
        }
    }

    namespace WSMove {
        DECLARE_TRAMPOLINE(babil::world::WSMove::wsmEncount, wsmEncount_t);
        u64 wsmEncount(babil::world::WSMove::cls* self, babil::world::WorldStateContext::cls* p_WorldStateContext) {
            if (!::game::g_encountersEnabled)
                return false;
            if (config::guaranteed_encounter)
                return true;
            return wsmEncount_t(self, p_WorldStateContext);
        }

        DECLARE_TRAMPOLINE(babil::world::WSMove::WSMove, WSMove_t);
        void WSMove(babil::world::WSMove::cls* self) {
            WSMoveInstance_ = self;
            WSMove_t(self);
        }
    }

    namespace WSVehicleMove {
        DECLARE_TRAMPOLINE(babil::world::WSVehicleMove::wsProcess, wsProcess_t);
        u64 wsProcess(babil::world::WSVehicleMove::cls* self, babil::world::WorldStateContext::cls* p_WorldStateContext) {
            auto ret = wsProcess_t(self, p_WorldStateContext);
            if (config::hide_vehicle_buttons) {
                babil::ui::CWidgetMng::deleteWidgetRange(babil::ui::g_WidgetMng, 0x17, 1);
                babil::ui::CWidgetMng::deleteWidgetRange(babil::ui::g_WidgetMng, 0x18, 1);
                babil::ui::CWidgetMng::deleteWidgetRange(babil::ui::g_WidgetMng, 0x19, 1);
            }
            return ret;
        }
    }

    namespace MSSConfig {
        // widgets:
        // 9 = description
        // 15 = scrollbar
        // 21 = back button
        // 7 = title bar
        // 0x10000 = off button
        // 0x10090 = Help button
        DECLARE_TRAMPOLINE(babil::world::MSSConfig::mssProcess, mssProcess_t);
        void* mssProcess(babil::world::MSSConfig::cls* self) {
            auto ret = mssProcess_t(self);
            // babil::ui::CWidgetMng::deleteWidgetRange(babil::ui::g_WidgetMng, 0x10090, 1);
            u32 scroll_height = *(u32*)((u8*)babil::ui::g_WidgetMng + 0x250);
            if (scroll_height != 440) {
                debugPrintf("scroll_height != 440: %d\n", scroll_height);
                babil::ui::CWidgetMng::addWidget(babil::ui::g_WidgetMng, 0x100a0, 2, 454, 456, 44, 0, 50733, 768);
                babil::ui::CWidgetMng::setScroll(babil::ui::g_WidgetMng, 58, 224, 440);
            }
            return ret;
        }

        static void (*FUN_004d82d0_t)(ushort* param_1, int cursor, int scroll) = nullptr;
        void FUN_004d82d0(ushort* param_1, int cursor, int scroll) {
            debugPrintf("FUN_004d82d0: %p, %i, %i\n", param_1, cursor, scroll);

            babil::world::MSSTextScreenClear((babil::msg::CANVAS)0);
            babil::world::MSSTextScreenClear((babil::msg::CANVAS)1);
            babil::ui::CWidgetMng::cleanWidget(babil::ui::g_WidgetMng);

            // FUN_004d82d0_t(param_1, cursor, scroll);
        }
    }

    namespace CCameraVibration {
        DECLARE_TRAMPOLINE(babil::world::CCameraVibration::startVibration, startVibration_t);
        void startVibration(babil::world::CCameraVibration::cls* self, babil::world::CCameraVibration::VIBRATION_STATE state, s32 time, s32 ticks, s32 x, s32 y, s32 z, u8 unk) {
            debugPrintf("startVibration(%i, %i, %i, %i, %i, %i, %i)\n", state, time, ticks, x, y, z, unk);
            return startVibration_t(self, state, time, ticks, x, y, z, unk);
        }
        DECLARE_TRAMPOLINE(babil::world::CCameraVibration::ccbUpdate, ccbUpdate_t);
        void ccbUpdate(babil::world::CCameraVibration::cls* self, babil::VecFx32* a, babil::VecFx32* b) {
            float absX = fabsf(self->m_Amplitude.x);
            float absY = fabsf(self->m_Amplitude.y);
            float absZ = fabsf(self->m_Amplitude.z);
            float amp  = fmaxf(absX, fmaxf(absY, absZ)) / 4096.0f;
            if (amp > 1.0f)
                amp = 1.0f;
            if (amp < 0.0f)
                amp = 0.0f;

            if (self->m_State == 2) {
                vibration::set_rumble(amp * ((float)self->m_CurrentTime / (float)self->m_TotalTime) * config::world_rumble_intensity, 0.0f);
            } else if (self->m_State == 1) {
                if ((self->m_CurrentTime & 1) == 0) {
                    vibration::set_rumble(amp * config::world_rumble_intensity, 0.0f);
                } else {
                    vibration::set_rumble(0.0f, 0.0f);
                }
            } else {
                vibration::set_rumble(0.0f, 0.0f);
            }

            return ccbUpdate_t(self, a, b);
        }
    }
}

static bool g_ShouldTogglePause;

namespace btl {
    namespace BattleMonsterParty {
        DECLARE_TRAMPOLINE(babil::btl::BattleMonsterParty::giftGold, giftGold_t);
        u32 giftGold(babil::btl::BattleMonsterParty::cls* self) {
            return giftGold_t(self) * ::game::g_gilMultiplier;
        }

        DECLARE_TRAMPOLINE(babil::btl::BattleMonsterParty::giftExp, giftExp_t);
        u32 giftExp(babil::btl::BattleMonsterParty::cls* self) {
            return giftExp_t(self) * ::game::g_expMultiplier;
        }
    }

    namespace BattleCommandSelector {
        DECLARE_TRAMPOLINE(babil::btl::BattleCommandSelector::initialize, initialize_t);
        void initialize(babil::btl::BattleCommandSelector::cls* self, babil::btl::BattleSystem::cls* system, babil::btl::BattlePlayer::cls* player) {
            g_LastBattlePlayerId = babil::btl::BattlePlayer::playerId(player);
            initialize_t(self, system, player);
        }
        DECLARE_TRAMPOLINE(babil::btl::BattleCommandSelector::commandAction, commandAction_t);
        u32 commandAction(babil::btl::BattleCommandSelector::cls* self, babil::btl::BattleSystem::cls* system, int _int, babil::btl::BattleCommandSelector::ABILITY_ID abilityId) {
            debugPrintf("commandAction: %d %d\n", _int, abilityId);
            return commandAction_t(self, system, _int, abilityId);
        }
    }

    namespace Battle2DManager {
        DECLARE_TRAMPOLINE(babil::btl::Battle2DManager::ctrlWidgetCheck, ctrlWidgetCheck_t);
        bool ctrlWidgetCheck(babil::btl::Battle2DManager::cls* self, babil::btl::Battle2DManager::WIDGET_CTRL_TYPE type) {
            if (type == babil::btl::Battle2DManager::Widget_Pause && g_ShouldTogglePause) {
                g_ShouldTogglePause = false;
                return true;
            }
            return ctrlWidgetCheck_t(self, type);
        }
    }

    namespace BattleDebugParameter {
        DECLARE_TRAMPOLINE(babil::btl::BattleDebugParameter::flag, flag_t);
        bool flag(babil::btl::BattleDebugParameter::cls* self, babil::btl::DEBUG_FLAG flag) {
            switch (flag) {
                case babil::btl::SURELY_MAX_DAMAGE:
                    if (config::SURELY_MAX_DAMAGE)
                        return true;
                    break;
                case babil::btl::QUICK_WAIT:
                    if (config::QUICK_WAIT)
                        return true;
                    break;
                case babil::btl::ENEMY_INVINCIBLE:
                    if (config::ENEMY_INVINCIBLE)
                        return true;
                    break;
                case babil::btl::FRIEND_INVINCIBLE:
                    if (config::FRIEND_INVINCIBLE)
                        return true;
                    break;
                case babil::btl::RESTART:
                    if (config::RESTART)
                        return true;
                    break;
                case babil::btl::SURELY_ESCAPE:
                    if (config::SURELY_ESCAPE)
                        return true;
                    break;
                case babil::btl::SURELY_CONDITION:
                    if (config::SURELY_CONDITION)
                        return true;
                    break;
                case babil::btl::TRANSFIX:
                    if (config::TRANSFIX)
                        return true;
                    break;
                case babil::btl::MDEF_INVALIDATION:
                    if (config::MDEF_INVALIDATION)
                        return true;
                    break;
                case babil::btl::QUICK_EVENT:
                    if (config::QUICK_EVENT)
                        return true;
                    break;
                case babil::btl::SURELY_HIT:
                    if (config::SURELY_HIT)
                        return true;
                    break;
                case babil::btl::SURELY_MISS:
                    if (config::SURELY_MISS)
                        return true;
                    break;
                case babil::btl::SURELY_CRITICAL:
                    if (config::SURELY_CRITICAL)
                        return true;
                    break;
                case babil::btl::MONSTER_STOP_ACTION:
                    if (config::MONSTER_STOP_ACTION)
                        return true;
                    break;
                case babil::btl::DAMAGE_OVER_LIMIT:
                    if (config::DAMAGE_OVER_LIMIT)
                        return true;
                    break;
                case babil::btl::OPEN_ENEMY_HP:
                    if (config::OPEN_ENEMY_HP)
                        return true;
                    break;
                case babil::btl::PHYSICS_RANDOM_MAX:
                    if (config::PHYSICS_RANDOM_MAX)
                        return true;
                    break;
                case babil::btl::MAGIC_RANDOM_MAX:
                    if (config::MAGIC_RANDOM_MAX)
                        return true;
                    break;
                case babil::btl::QUICK_TURN:
                    if (config::QUICK_TURN)
                        return true;
                    break;
                default:
                    break;
            }
            return flag_t(self, flag);
        }
    }

    namespace CBattleDisplay {
        DECLARE_TRAMPOLINE(babil::btl::CBattleDisplay::doShakeCamera, doShakeCamera_t);
        void doShakeCamera(babil::btl::CBattleDisplay::cls* self) {
            float amp = fmaxf(self->m_ShakeAmplitudeX, fmaxf(self->m_ShakeAmplitudeY, self->m_ShakeAmplitudeZ)) / 4096.0f;
            if (amp > 1.0f)
                amp = 1.0f;
            if (amp < 0.0f)
                amp = 0.0f;

            if (self->m_ShakeTimer > 0) {
                vibration::set_rumble(amp * config::battle_rumble_intensity, 0.0f);
            }

            doShakeCamera_t(self);

            if (self->m_ShakeTimer == 0) {
                vibration::set_rumble(0.0f, 0.0f);
            }
        }
    }

    DECLARE_TRAMPOLINE(babil::btl::battleSpeedRate, battleSpeedRate_t);
    u32 battleSpeedRate() {
        auto rate = battleSpeedRate_t();
        rate /= config::dfps_battles_mult;
        return rate;
    }

    namespace BattleWin {
        DECLARE_TRAMPOLINE(babil::btl::BattleWin::possessGoldPhase, possessGoldPhase_t);
        bool possessGoldPhase(babil::btl::BattleSystem::cls* self) {
            bool result = possessGoldPhase_t(self);
            s32* gill   = babil::sys::GameParameter::gold(babil::sys::GameParameter::gpInstance_);
            if (*gill > 9999999 && !config::remove_gil_limit)
                *gill = 9999999;
            return result;
        }
    }
}

namespace CCharacterMng {
    DECLARE_TRAMPOLINE(babil::CCharacterMng::setPause, setPause_t);
    void setPause(babil::CCharacterMng::cls* self, int _int, bool _bool, u32 enTYPE) {
        // debugPrintf("setPause(%d, %d, %d)", _int, _bool, enTYPE);
        setPause_t(self, _int, _bool, enTYPE);
    }
    DECLARE_TRAMPOLINE(babil::CCharacterMng::setFrameRate, setFrameRate_t);
    void setFrameRate(babil::CCharacterMng::cls* self, babil::fx32 framerate) {
        // debugPrintf("CCharacterMng::setFrameRate(%d)\n", framerate);
        setFrameRate_t(self, framerate / config::dfps_battles_mult);
    }
}

namespace ds {
    namespace sys3d {
        namespace CAnimation {
            DECLARE_TRAMPOLINE(babil::ds::sys3d::CAnimation::getFrameRate, getFrameRate_t);
            babil::fx32 getFrameRate(babil::ds::sys3d::CAnimation::cls* self) {
                // debugPrintf("CAnimation::getFrameRate\n");
                auto framerate = getFrameRate_t(self);
                if (config::dfps_battles_enabled && babil::sys::GGlobal::getCurrentPart() == babil::BabilGamePart_BattlePart) {
                    framerate /= config::dfps_battles_mult;
                }
                return framerate;
            }

            DECLARE_TRAMPOLINE(babil::ds::sys3d::CAnimation::ctor, ctor_t);
            void ctor(babil::ds::sys3d::CAnimation::cls* self) {
                ctor_t(self);
                if (config::dfps_battles_enabled && babil::sys::GGlobal::getCurrentPart() == babil::BabilGamePart_BattlePart) {
                    self->framerate /= config::dfps_battles_mult;
                }
            }

            DECLARE_TRAMPOLINE(babil::ds::sys3d::CAnimation::setup, setup_t);
            void setup(babil::ds::sys3d::CAnimation::cls* self, void* a, void* b, u32 c, void* d) {
                setup_t(self, a, b, c, d);
                if (config::dfps_battles_enabled && babil::sys::GGlobal::getCurrentPart() == babil::BabilGamePart_BattlePart) {
                    self->framerate /= config::dfps_battles_mult;
                }
            }

            DECLARE_TRAMPOLINE(babil::ds::sys3d::CAnimation::setFrameRate, setFrameRate_t);
            void setFrameRate(babil::ds::sys3d::CAnimation::cls* self, babil::fx32 framerate) {
                // debugPrintf("CAnimation::setFrameRate(%d)\n", framerate);
                setFrameRate_t(self, framerate / config::dfps_battles_mult);
            }
        }
        namespace CMotSet {
            DECLARE_TRAMPOLINE(babil::ds::sys3d::CMotSet::getFrameRate, getFrameRate_t);
            babil::fx32 getFrameRate(babil::ds::sys3d::CMotSet::cls* self) {
                // debugPrintf("CMotSet::getFrameRate\n");
                auto framerate = getFrameRate_t(self);
                if (config::dfps_battles_enabled && babil::sys::GGlobal::getCurrentPart() == babil::BabilGamePart_BattlePart) {
                    framerate /= config::dfps_battles_mult;
                }
                return framerate;
            }

            DECLARE_TRAMPOLINE(babil::ds::sys3d::CMotSet::setFrameRate, setFrameRate_t);
            void setFrameRate(babil::ds::sys3d::CMotSet::cls* self, babil::fx32 framerate) {
                // debugPrintf("CMotSet::setFrameRate(%d)\n", framerate);
                setFrameRate_t(self, framerate / config::dfps_battles_mult);
            }
        }
    }
}

namespace Font {
    DECLARE_TRAMPOLINE(babil::Font::initFont, initFont_t);
    void initFont(babil::Font::cls* self) {
        FontInstance_ = self;
        initFont_t(self);
    }
}

DECLARE_TRAMPOLINE(babil::NNS_G2dTickCellAnimation, NNS_G2dTickCellAnimation_t);
void NNS_G2dTickCellAnimation(void* pCellAnim, babil::fx32 frames) {
    NNS_G2dTickCellAnimation_t(pCellAnim, frames / config::dfps_battles_mult);
}

DECLARE_TRAMPOLINE(babil::babilCommand_ClearCountJump, babilCommand_ClearCountJump_t);
void babilCommand_ClearCountJump(babil::ScriptEngine::cls* scriptEngine) {
    if (config::remove_newgameplus_limit || config::reobtainable_limit_break) {
        u8 clear_count = babil::ScriptEngine::getByte(scriptEngine);
        u32 target     = babil::ScriptEngine::getDword(scriptEngine);
        if (clear_count == 2 && config::remove_newgameplus_limit)
            return;
        if (clear_count == 0 && config::reobtainable_limit_break)
            babil::ScriptEngine::jump(scriptEngine, target);
    } else {
        babilCommand_ClearCountJump_t(scriptEngine);
    }
}

DECLARE_TRAMPOLINE(babil::babilCommand_CE_CameraPos, babilCommand_CE_CameraPos_t);
void babilCommand_CE_CameraPos(babil::ScriptEngine::cls* scriptEngine) {
    babilCommand_CE_CameraPos_t(scriptEngine);
}

DECLARE_TRAMPOLINE(babil::babilCommand_CE_SetupCameraMotion, babilCommand_CE_SetupCameraMotion_t);
void babilCommand_CE_SetupCameraMotion(babil::ScriptEngine::cls* scriptEngine) {
    babilCommand_CE_SetupCameraMotion_t(scriptEngine);
}

namespace debug {
    namespace CharaViewerPart {
        DECLARE_TRAMPOLINE(babil::debug::CharaViewerPart::doInitialize, doInitialize_t);
        void doInitialize(babil::debug::CharaViewerPart::cls* self) {
            auto cvram = babil::ds::CVram::m_instance;
            if (cvram->m_TexVramMng != nullptr)
                babil::ds::CVram::releaseTexVramMng(cvram);
            if (cvram->m_PlttVramMng != nullptr)
                babil::ds::CVram::releasePlttVramMng(cvram);
            doInitialize_t(self);
        }
    }
}

namespace title {
    namespace TitleContents {
        DECLARE_TRAMPOLINE(babil::title::TitleContents::update, update_t);
        void update(babil::title::TitleContents::cls* self) {
            if (config::fix_titlepart) {
                self->m_ContentMask &= 0b111;
                if (babil::ui::g_WidgetMng->m_FocusedElement - 0x10000U >= 3) {
                    bool at_top    = false;
                    bool at_bottom = false;

                    if (self->m_ContentMask & 0b1)
                        at_top = self->m_FocusedIndex == 0;
                    else if (self->m_ContentMask & 0b10)
                        at_top = self->m_FocusedIndex == 1;
                    else if (self->m_ContentMask & 0b100)
                        at_top = self->m_FocusedIndex == 2;

                    if (self->m_ContentMask & 0b100)
                        at_bottom = self->m_FocusedIndex == 2;
                    else if (self->m_ContentMask & 0b10)
                        at_bottom = self->m_FocusedIndex == 1;
                    else if (self->m_ContentMask & 0b1)
                        at_bottom = self->m_FocusedIndex == 0;

                    u32 mask = babil::ds::CPad::repeat(babil::ds::g_Pad);

                    if (mask & 0xf0) {
                        bool going_down;
                        if (mask & (1 << 6)) {
                            going_down = false;
                        } else if (mask & (1 << 7)) {
                            going_down = true;
                        } else if (mask & (1 << 5)) {
                            going_down = false;
                        } else {
                            going_down = true;
                        }

                        if (at_top && !going_down) {
                            self->m_FocusedIndex = 3;
                        } else if (at_bottom && going_down) {
                            self->m_FocusedIndex = 3;
                        }
                    }
                }
            }

            update_t(self);
        }
    }
}

namespace pl {
    namespace Player {
        DECLARE_TRAMPOLINE(babil::pl::Player::canEquip, canEquip_t);
        bool canEquip(babil::pl::Player::cls* self, s16 itemId, babil::pl::HAND_TYPE hand) {
            if (config::equip_anything)
                return true;
            return canEquip_t(self, itemId, hand);
        }

        DECLARE_TRAMPOLINE(babil::pl::Player::setParameter, setParameter_t);
        void setParameter(babil::pl::Player::cls* self, u8 current_lv, u8 next_lv) {
            debugPrintf("setParameter\n");

            auto playerSaveParameter = babil::sys::GameParameter::playerSaveParameter(babil::sys::GameParameter::gpInstance_, self->m_PlayerType);

            if (!playerSaveParameter)
                return;

            auto currentStats = playerSaveParameter->m_BodyParameter;

            setParameter_t(self, current_lv, next_lv);

            if (!config::augmentless_stat_growth || next_lv <= 70 || current_lv > next_lv)
                return;

            const u8 gained    = next_lv - current_lv;
            const u8 remaining = 99 - current_lv;

            auto scaleStat = [&](u8 cur) -> u8 {
                return static_cast<u8>(cur + gained * (99 - cur) / remaining);
            };

            auto& newStats       = playerSaveParameter->m_BodyParameter;
            newStats.m_Strength  = scaleStat(currentStats.m_Strength);
            newStats.m_Speed     = scaleStat(currentStats.m_Speed);
            newStats.m_Stamina   = scaleStat(currentStats.m_Stamina);
            newStats.m_Intellect = scaleStat(currentStats.m_Intellect);
            newStats.m_Spirit    = scaleStat(currentStats.m_Spirit);
        }
    }
}

namespace itm {
    namespace EquipParameter {
        DECLARE_TRAMPOLINE(babil::itm::EquipParameter::canEquip, canEquip_t);
        bool canEquip(babil::itm::EquipParameter::cls* self, u32 a) {
            if (config::equip_anything)
                return true;
            return canEquip_t(self, a);
        }
    }
}

namespace ds {
    namespace CVram {
        DECLARE_TRAMPOLINE(babil::ds::CVram::setupTexVramMng, setupTexVramMng_t);
        void setupTexVramMng(babil::ds::CVram::cls* self, u32 a, u32 b, u32 c, s8 d) {
            if (self->m_TexVramMng != nullptr)
                babil::ds::CVram::releaseTexVramMng(self);
            return setupTexVramMng_t(self, a, b, c, d);
        }
        DECLARE_TRAMPOLINE(babil::ds::CVram::setupPlttVramMng, setupPlttVramMng_t);
        void setupPlttVramMng(babil::ds::CVram::cls* self, u32 a, u32 b, s8 c) {
            if (self->m_PlttVramMng != nullptr)
                babil::ds::CVram::releasePlttVramMng(self);
            return setupPlttVramMng_t(self, a, b, c);
        }
    }
}

namespace evt {
    namespace EventCamera {
        DECLARE_TRAMPOLINE(babil::evt::EventCamera::startCameraMotion, startCameraMotion_t);
        void startCameraMotion(babil::evt::EventCamera::cls* self, u32 a, u32 b, u32 c, bool d) {
            // debugPrintf("startCameraMotion(%i, %i, %i, %s)\n", a, b, c, d ? "true" : "false");
            return startCameraMotion_t(self, a, b, c, d);
        }
    }
}

int getFieldSymbolID() {
    if (babil::sys::GGlobal::getCurrentPart() == babil::BabilGamePart_WorldPart) {
        return babil::sys::GameParameter::fieldSymbolID(babil::sys::GameParameter::gpInstance_);
    } else {
        return -1;
    }
}

int getMemberIDForPartyOrder(u8 order) {
    babil::pl::Player::cls* member = babil::pl::PlayerParty::memberForOrder(babil::pl::PlayerParty::playerPartyInstance_, order);
    if (member == babil::pl::PlayerParty::InvalidPlayer) {
        return -1;
    }
    return member->m_PlayerType;
}

void test() {
    if (babil::sys::GGlobal::getCurrentPart() == babil::BabilGamePart_WorldPart) {
        // ::game::g_playerSelectOpen = true;
        debugPrintf("Is in Overworld = true\n");
        u32 symbolID = babil::sys::GameParameter::fieldSymbolID(babil::sys::GameParameter::gpInstance_);
        debugPrintf("Field Symbol ID = %d\n", symbolID);
        debugPrintf("Member Order = %d\n", babil::pl::PlayerParty::memberOrder(babil::pl::PlayerParty::playerPartyInstance_, (babil::pl::PLAYER_TYPES)symbolID));
        if (wssInstance_ != nullptr) {
            babil::world::WorldState::cls* ws = babil::world::WorldStateScheduler::wssGetCurrentState(wssInstance_);
            debugPrintf("State = %p\n", ws);
            auto currentState = babil::world::WorldState::wsGetString(ws);
            debugPrintf("State = %s\n", currentState);
        }

        // auto wpInstance_ = babil::part::WorldPart::wpInstance_;
        // if (wpInstance_ != nullptr) {
        //     debugPrintf("wpInstance_ = %p\n", wpInstance_);
        //     auto wsc = wpInstance_->m_WorldStateContext;
        //     babil::ds::sys3d::CCamera::setFOV((babil::ds::sys3d::CCamera::cls*)&wsc.m_WorldCamera, 0xffff, 0xffff);
        // }

        // babil::pl::PlayerParty::addItem(babil::pl::PlayerParty::playerPartyInstance_, 0x23E0, 1);
    }
    if (babil::sys::GGlobal::getCurrentPart() == babil::BabilGamePart_BattlePart) {
        auto bp = babil::btl::BattlePart::instance_;

        // babil::ui::CWidgetMng::deleteWidgetRange(babil::ui::g_WidgetMng, 0x25, 1);
        // g_ShouldTogglePause = true;
        // if (!bp->m_IsPaused)
        // debugPrintf("IsPaused: %d | ShouldPause: %d\n", babil::btl::BattlePart::instance_->m_IsPaused, babil::btl::BattlePart::instance_->m_ShouldPause);
        // if (babil::btl::BattlePart::instance_->m_IsPaused) {
        //     babil::btl::BattlePart::instance_->m_IsPaused = false;
        //     babil::btl::Battle2DManager::instance_->m_BattlePause.flags |= 1;

        //     babil::ui::CWidgetMng::deleteWidgetRange(babil::ui::g_WidgetMng, 0x12, 1);
        //     babil::ui::g_WidgetMng->idk1 = 0;
        //     babil::ui::g_WidgetMng->idk2 = 0;
        //     babil::ui::g_WidgetMng->idk3 = 0;
        // } else {
        //     babil::btl::BattlePart::instance_->m_ShouldPause = true;
        // }
        // debugPrintf("IsPaused: %d | ShouldPause: %d\n", babil::btl::Battl
    }

    // *babil::LCD_WIDTH  = 570;
    // *babil::LCD_HEIGHT = 320;

    // achievement::unlock("CgkIp7jFgNUFEAIQKg");

    // OSi_Panic("test", 55, "Goodbye World");

    // AKBSystem::PrintTracks();

    // if (babil::characterMng == nullptr)
    //     return;
    // auto texMng = babil::characterMng->m_CTextureDataMng;
    // auto slots  = texMng.m_Slots;
    // for (int i = 0; i < 36; i++) {
    //     auto slot = slots[i];
    //     if (!slots[i].m_Setup)
    //         continue;
    //     debugPrintf("Texture[%i]: %s\n", i, slot.m_Name);
    // }
}

void patch_max_frame_skip() {
    u32* current = (u32*)so::findAddr_rw("render");

    for (size_t i = 0; i < 200; i++) {
        struct Da64Inst movz;
        da64_decode(current[i], &movz);

        if (movz.mnem != DA64I_MOVZ)
            continue;
        if (movz.ops[0].reggp.sf != 0)
            continue;
        if (movz.ops[1].uimm16 != 3)
            continue;
        if (movz.ops[1].immshift.shift != 0)
            continue;

        u32 mov_rd = movz.ops[0].reg;

        for (size_t j = i + 1; j <= i + 5; j++) {
            struct Da64Inst cmp_inst;
            da64_decode(current[j], &cmp_inst);

            if (cmp_inst.mnem != DA64I_SUBS_IMM)
                continue;
            if (cmp_inst.ops[0].reg != 31)
                continue;
            if (cmp_inst.ops[0].reggp.sf != 1)
                continue;

            u64 cmp_value = (u64)cmp_inst.ops[2].uimm16
                << cmp_inst.ops[2].immshift.shift;
            if (cmp_value != 3)
                continue;

            u32 cmp_rn = cmp_inst.ops[1].reg;
            int sf     = cmp_inst.ops[1].reggp.sf;

            for (size_t k = j + 1; k <= j + 5; k++) {
                struct Da64Inst csel;
                da64_decode(current[k], &csel);

                if (csel.mnem != DA64I_CSEL)
                    continue;

                u32 csel_rd   = csel.ops[0].reg;
                u32 csel_rn   = csel.ops[1].reg;
                u32 csel_rm   = csel.ops[2].reg;
                u32 csel_cond = csel.ops[3].cond;
                int csel_sf   = csel.ops[0].reggp.sf;

                if (csel_rd != csel_rn)
                    continue;
                if (csel_rm != mov_rd)
                    continue;
                if (csel_rn != cmp_rn)
                    continue;
                if (csel_cond == 0)
                    continue;
                if (csel_sf != 1)
                    continue;

                // uintptr_t csel_rw = (uintptr_t)&current[k];
                // uintptr_t csel_rx = so::addr_rw_to_rx(csel_rw);

                // uintptr_t stub_rw = ((uintptr_t)so::trampoline_base + so::trampoline_cursor);
                // uintptr_t stub_rx = ((uintptr_t)so::trampoline_virtbase + so::trampoline_cursor);
                // so::CodeBuf stub { stub_rw, stub_rx };

                // stub.w32(de64_LDRx_pcrel(DA_GP(16), 2));
                // stub.w32(de64_B(3));
                // stub.w64((uintptr_t)&::game::g_MaxUpdatesPerFrame);
                // stub.w32(de64_LDRxu(DA_GP(17), DA_GP(16), 0));
                // stub.w32(de64_MOVx(csel_rm, DA_GP(17)));
                // stub.w32(de64_CMPx_lsl(csel_rn, csel_rm, 0));
                // stub.w32(current[k]);
                // stub.wb(csel_rx + 4);
                // so::trampoline_cursor += stub.count * 4;

                // so::CodeBuf orig { csel_rw, csel_rx };
                // orig.wb(stub_rx);

                current[k] = de64_NOP();
                return;
            }
        }
    }

    debugPrintf("Unable to find max_frame_skip\n");
}

void patch_max_gold() {
    u32* current = (u32*)so::findAddr_rw("_ZN3btl9BattleWin16possessGoldPhaseERNS_12BattleSystemE");

    for (size_t i = 0; i < 400; i++) {
        struct Da64Inst movz;
        da64_decode(current[i], &movz);

        if (movz.mnem != DA64I_MOVZ)
            continue;
        if (movz.ops[0].reggp.sf != 0)
            continue;
        if (movz.ops[1].uimm16 != 0x967f)
            continue;
        if (movz.ops[1].immshift.shift != 0)
            continue;

        u32 reg = movz.ops[0].reg;

        struct Da64Inst movk;
        da64_decode(current[i + 1], &movk);

        if (movk.mnem != DA64I_MOVK)
            continue;
        if (movk.ops[0].reggp.sf != 0)
            continue;
        if (movk.ops[0].reg != reg)
            continue;
        if (movk.ops[1].uimm16 != 0x98)
            continue;
        if (movk.ops[1].immshift.shift != 16)
            continue;

        uintptr_t movz_rw = (uintptr_t)&current[i];
        uintptr_t movz_rx = so::addr_rw_to_rx(movz_rw);
        so::CodeBuf orig { movz_rw, movz_rx };
        orig.w32(de64_MOVZw(reg, 0xFFFF));
        orig.w32(de64_MOVKw(reg, 0x7FFF));

        debugPrintf("Successfully patched gold limit of btl::BattleWin::possessGoldPhase to 2,147,483,647!\n");
        return;
    }

    current = (u32*)so::findAddr_rw("_ZN3btl18AcquiredGoldDrawer6updateEv");

    // for (size_t i = 0; i < 400; i++) {
    //     struct Da64Inst movz;
    //     da64_decode(current[i], &movz);

    //     if (movz.mnem != DA64I_MOVZ)
    //         continue;
    //     if (movz.ops[0].reggp.sf != 0)
    //         continue;
    //     if (movz.ops[1].uimm16 != 0x967f)
    //         continue;
    //     if (movz.ops[1].immshift.shift != 0)
    //         continue;

    //     u32 reg = movz.ops[0].reg;

    //     struct Da64Inst movk;
    //     da64_decode(current[i + 1], &movk);

    //     if (movk.mnem != DA64I_MOVK)
    //         continue;
    //     if (movk.ops[0].reggp.sf != 0)
    //         continue;
    //     if (movk.ops[0].reg != reg)
    //         continue;
    //     if (movk.ops[1].uimm16 != 0x98)
    //         continue;
    //     if (movk.ops[1].immshift.shift != 16)
    //         continue;

    //     uintptr_t movz_rw = (uintptr_t)&current[i];
    //     uintptr_t movz_rx = so::addr_rw_to_rx(movz_rw);
    //     so::CodeBuf orig { movz_rw, movz_rx };
    //     orig.w32(de64_MOVZw(reg, 0xFFFF));
    //     orig.w32(de64_MOVKw(reg, 0x7FFF));

    //     debugPrintf("Successfully patched gold limit of btl::AcquiredGoldDrawer::update to 2,147,483,647!\n");
    //     return;
    // }
}

void patch() {
    HOOK_FUNCTION(OSi_Panic);
    HOOK_FUNCTION_BASE(babil::OS_Printf, debugPrintf);

    HOOK_FUNCTION_T(NNS_G2dTickCellAnimation);

    HOOK_FUNCTION_T(babilCommand_ClearCountJump);
    HOOK_FUNCTION_T(babilCommand_CE_CameraPos);
    HOOK_FUNCTION_T(babilCommand_CE_SetupCameraMotion);

    HOOK_FUNCTION_T(sys::GGlobal::setNextPart);

    HOOK_FUNCTION_T(sys::GameParameter::isDash);
    // HOOK_FUNCTION_T(sys::GameParameter::gameClearCount);

    HOOK_FUNCTION_T(ui::CWidgetMng::addWidget);
    HOOK_FUNCTION_T(ui::CWidgetMng::setScroll);
    HOOK_FUNCTION_T(ui::CWidgetMng::setCursor);

    // HOOK_FUNCTION_T(world::MSSConfig::mssProcess);

    HOOK_FUNCTION_T(world::disableSPSetting);
    HOOK_FUNCTION_T(world::searchNextSymbolByCharacterIDForward);
    HOOK_FUNCTION_T(world::searchNextSymbolByCharacterIDBackward);
    // HOOK_FUNCTION_T(world::canChangeSymbol);

    HOOK_FUNCTION_T(world::WSMove::WSMove);
    HOOK_FUNCTION_T(world::WSMove::wsmEncount);

    HOOK_FUNCTION_T(world::WorldStateScheduler::wssInitialize);
    HOOK_FUNCTION_T(world::WorldStateScheduler::wssAddStateSchedule);
    HOOK_FUNCTION_T(world::WSVehicleMove::wsProcess);

    HOOK_FUNCTION_T(world::CCameraVibration::startVibration);
    HOOK_FUNCTION_T(world::CCameraVibration::ccbUpdate);

    HOOK_FUNCTION_T(btl::battleSpeedRate);

    HOOK_FUNCTION_T(btl::Battle2DManager::ctrlWidgetCheck);

    HOOK_FUNCTION_T(btl::BattleMonsterParty::giftGold);
    HOOK_FUNCTION_T(btl::BattleMonsterParty::giftExp);

    HOOK_FUNCTION_T(btl::BattleCommandSelector::initialize);
    HOOK_FUNCTION_T(btl::BattleCommandSelector::commandAction);

    HOOK_FUNCTION_T(CCharacterMng::setPause);
    HOOK_FUNCTION_T(CCharacterMng::setFrameRate);

    HOOK_FUNCTION_T(ds::sys3d::CAnimation::ctor);
    HOOK_FUNCTION_T(ds::sys3d::CAnimation::getFrameRate);
    HOOK_FUNCTION_T(ds::sys3d::CAnimation::setup);
    HOOK_FUNCTION_T(ds::sys3d::CAnimation::setFrameRate);
    HOOK_FUNCTION_T(ds::sys3d::CMotSet::getFrameRate);
    HOOK_FUNCTION_T(ds::sys3d::CMotSet::setFrameRate);

    HOOK_FUNCTION_T(Font::initFont);

    HOOK_FUNCTION_T(title::TitleContents::update);

    HOOK_FUNCTION_T(pl::Player::setParameter);
    HOOK_FUNCTION_T(pl::Player::canEquip);

    HOOK_FUNCTION_T(itm::EquipParameter::canEquip);

    HOOK_FUNCTION_T(btl::BattleDebugParameter::flag);

    HOOK_FUNCTION_T(btl::CBattleDisplay::doShakeCamera);

    HOOK_FUNCTION_T(debug::CharaViewerPart::doInitialize);

    HOOK_FUNCTION_T(ds::CVram::setupTexVramMng);
    HOOK_FUNCTION_T(ds::CVram::setupPlttVramMng);

    HOOK_FUNCTION_T(evt::EventCamera::startCameraMotion);

    patch_max_frame_skip();
    // patch_max_gold();

    // so::hook(reinterpret_cast<uintptr_t>(so::text_virtbase) + 0x3d82d0, reinterpret_cast<uintptr_t>(world::MSSConfig::FUN_004d82d0), reinterpret_cast<uintptr_t*>(&world::MSSConfig::FUN_004d82d0_t));

    armSetTlsRw(fake_tls);
}
}