#include "achievement.h"

#include <GLES/gl.h>

#include <deque>
#include <fstream>
#include <string>

#include "babil.h"
#include "base64.h"
#include "bridge.h"
#include "config.h"
#include "game.h"
#include "imgui/imgui.h"
#include "stb_image.h"
#include "stdext.h"
#include "util.h"

using namespace babil;

namespace achievement {

constexpr float POPUP_DURATION  = 4.0f;
constexpr float POPUP_ANIM_TIME = 0.35f;
constexpr float POPUP_WIDTH     = 350.0f * game::IMGUI_SCALE;
constexpr float POPUP_HEIGHT    = 100.0f * game::IMGUI_SCALE;
constexpr float ICON_SIZE       = 80.0f * game::IMGUI_SCALE;
constexpr float PADDING         = 10.0f * game::IMGUI_SCALE;

struct AchievementInfo
{
    std::string name;
    std::string description;
    bool secret;
    GLuint texture;
};

#pragma pack(push, 1)
struct AchievementRecord
{
    char identifier[256];
    double progress;
};
#pragma pack(pop)

struct Popup
{
    u8 id;
    float timer;
};

int achievementCount = 0;
std::vector<AchievementParam> achievementParams;
std::vector<AchievementInfo> achievementInfo;
std::vector<bool> unlocked;
std::vector<float> progress;
std::vector<float> progressCache;
std::vector<bool> isCached;

static std::deque<Popup> popups;

void reset_progress_cache() {
    for (int i = 0; i < achievementCount; ++i) {
        isCached[i] = false;
    }
}

static GLuint load_icon(const char* path) {
    int w, h, channels;
    unsigned char* data = stbi_load(path, &w, &h, &channels, 4);
    if (!data) {
        debugPrintf("achievement: failed to load icon %s\n", path);
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    return tex;
}

static float smoothstep(float t) {
    return t * t * (3.0f - 2.0f * t);
}

u8 getId(char* identifier) {
    char* decoded = b64decode(identifier, strlen(identifier));
    if (!decoded)
        return 0;
    return static_cast<u8>(decoded[12]);
}

void init() {
    int size;
    u8* achievement_param = bridge::loadFile("achievement_param.bbd", &size);
    if (!achievement_param) {
        return;
    }
    size_t num = size / sizeof(AchievementParam);
    std::vector<AchievementParam> params(reinterpret_cast<AchievementParam*>(achievement_param), reinterpret_cast<AchievementParam*>(achievement_param) + num);
    free(achievement_param);

    u8 max_id = 0;
    for (auto& param : params) {
        u8 id = getId(param.m_Identifier);
        if (id > max_id) {
            max_id = id;
        }
    }

    achievementCount = max_id + 1;
    achievementParams.resize(achievementCount);
    achievementInfo.resize(achievementCount);
    unlocked.resize(achievementCount, false);
    progress.resize(achievementCount, 0.0f);
    progressCache.resize(achievementCount, 0.0f);
    isCached.resize(achievementCount, false);

    for (auto& param : params) {
        u8 id                 = getId(param.m_Identifier);
        achievementParams[id] = param;
    }

    reset_progress_cache();

    std::ifstream in(ACHIEVEMENTS_PATH "/achievements.ini");
    if (!in.is_open()) {
        debugPrintf("achievement::init: failed to open " ACHIEVEMENTS_PATH "achievements.ini\n");
    } else {
        std::string section;
        std::string line;

        while (std::getline(in, line)) {
            line = stdext::trim(line);

            if (line.empty() || line[0] == ';' || line[0] == '#')
                continue;

            if (line.front() == '[' && line.back() == ']') {
                section = line.substr(1, line.size() - 2);
                continue;
            }

            const auto eq = line.find('=');
            if (eq == std::string::npos)
                continue;

            const std::string key   = stdext::trim(line.substr(0, eq));
            const std::string value = stdext::trim(line.substr(eq + 1));

            int current_id = -1;
            try {
                current_id = std::stoi(section);
            } catch (...) {
                continue;
            }

            if (current_id < 0 || current_id >= achievementCount)
                continue;

            AchievementInfo& info = achievementInfo[current_id];
            if (key == "name")
                info.name = value;
            else if (key == "description")
                info.description = value;
            else if (key == "secret")
                info.secret = (value == "true");
        }
    }

    for (int i = 0; i < achievementCount; i++) {
        char icon_path[256];
        snprintf(icon_path, sizeof(icon_path), ACHIEVEMENTS_PATH "%03d.png", i);
        achievementInfo[i].texture = load_icon(icon_path);
    }

    read_achievements();
    debugPrintf("Achievements initialized\n");
}

void draw() {
    if (popups.empty())
        return;

    Popup& popup     = popups.front();
    const uint8_t id = popup.id;

    if (id >= achievementCount) {
        popups.pop_front();
        return;
    }

    float t    = popup.timer;
    float frac = 1.0f;
    if (t < POPUP_ANIM_TIME)
        frac = t / POPUP_ANIM_TIME;
    else if (t > POPUP_DURATION - POPUP_ANIM_TIME)
        frac = (POPUP_DURATION - t) / POPUP_ANIM_TIME;

    frac = smoothstep(std::max(0.0f, std::min(1.0f, frac)));

    const float sw = static_cast<float>(config::screen_width);
    const float x  = sw - POPUP_WIDTH - PADDING + (POPUP_WIDTH + PADDING) * (1.0f - frac);
    const float y  = PADDING;

    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(POPUP_WIDTH, POPUP_HEIGHT), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.88f * frac);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("##achievement_popup", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

    const AchievementInfo& info = achievementInfo[id];

    float icon_y = (POPUP_HEIGHT - ICON_SIZE) * 0.5f;
    ImGui::SetCursorPos(ImVec2(PADDING, icon_y));

    if (info.texture)
        ImGui::Image(static_cast<ImTextureID>(static_cast<uintptr_t>(info.texture)), ImVec2(ICON_SIZE, ICON_SIZE));
    else
        ImGui::Dummy(ImVec2(ICON_SIZE, ICON_SIZE));

    float base_font_size = ImGui::GetFontSize();
    float line1_h        = base_font_size * 0.58f;
    float line2_h        = base_font_size * 0.72f;
    float line3_h        = base_font_size * 0.56f;
    float item_spacing   = ImGui::GetStyle().ItemSpacing.y;

    float total_text_height = line1_h + line2_h + line3_h + (item_spacing * 2.0f);

    float text_x = PADDING + ICON_SIZE + PADDING;
    float text_y = (POPUP_HEIGHT - total_text_height) * 0.5f;
    ImGui::SetCursorPos(ImVec2(text_x, text_y));

    ImGui::BeginGroup();
    ImGui::SetWindowFontScale(0.50f);
    ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.3f, frac), "Achievement Unlocked");
    ImGui::SetWindowFontScale(0.75f);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, frac), "%s", info.name.c_str());
    ImGui::SetWindowFontScale(0.50f);
    ImGui::TextColored(ImVec4(0.78f, 0.78f, 0.78f, frac), "%s", info.description.c_str());
    ImGui::SetWindowFontScale(1.0f);
    ImGui::EndGroup();

    ImGui::End();
    ImGui::PopStyleVar();

    popup.timer += ImGui::GetIO().DeltaTime;
    if (popup.timer >= POPUP_DURATION)
        popups.pop_front();
}

void unlock(const char* identifier) {
    if (!identifier)
        return;

    char* decoded = b64decode(identifier, strlen(identifier));
    if (!decoded)
        return;

    u8 id = static_cast<u8>(decoded[12]);

    if (id >= achievementCount || unlocked[id])
        return;

    unlocked[id] = true;
    progress[id] = 100.0f;
    isCached[id] = true;

    if (config::show_achievement_popup) {
        Popup popup;
        popup.id    = id;
        popup.timer = 0.0f;
        popups.push_back(popup);
    }

    debugPrintf("achievement::unlock: %03d %s\n", id, achievementInfo[id].name.c_str());
}

void read_achievements() {
    char path[256];
    snprintf(path, sizeof(path), "%s/report_achi.bin", config::save_filename);

    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) {
        debugPrintf("achievement::read_achievements: failed to open %s\n", path);
        return;
    }

    u8 count = 0;
    if (!in.read(reinterpret_cast<char*>(&count), 1)) {
        return;
    }

    for (int i = 0; i < count; i++) {
        AchievementRecord rec;
        if (!in.read(reinterpret_cast<char*>(&rec), sizeof(rec)))
            break;

        if (rec.identifier[0] == '\0')
            continue;

        char* decoded = b64decode(rec.identifier, strlen(rec.identifier));
        if (!decoded)
            continue;

        u8 id = static_cast<u8>(decoded[12]);

        if (id < achievementCount) {
            if (rec.progress >= 100.0)
                unlocked[id] = true;
            progress[id] = rec.progress;
        }
    }

    reset_progress_cache();
    debugPrintf("Read %d saved achievements\n", count);
}

bool is_unlocked(u8 id) {
    return id < achievementCount && unlocked[id];
}

int get_count() {
    return achievementCount;
}

const char* get_name(u8 id) {
    if (id >= achievementCount)
        return "";
    return achievementInfo[id].name.c_str();
}

const char* get_description(u8 id) {
    if (id >= achievementCount)
        return "";
    return achievementInfo[id].description.c_str();
}

bool is_secret(u8 id) {
    if (id >= achievementCount)
        return false;
    return achievementInfo[id].secret;
}

unsigned int get_texture(u8 id) {
    if (id >= achievementCount)
        return 0;
    return achievementInfo[id].texture;
}

bool has_decant(s16 id) {
    itm::PossessionItemManager::cls* pim = sys::GameParameter::item(sys::GameParameter::gpInstance_);
    if (itm::PossessionItemManager::searchImportantItem(pim, id) != nullptr) {
        return true;
    }
    void* itemParam = itm::ItemManager::importantParameter(itm::ItemManager::instance_, id);
    if (itemParam == nullptr)
        return false;
    s16 effectId  = *(s16*)((u8*)itemParam + 0x16);
    void* effData = common::EfficacyDataConvection::getEfficacyData(common::EfficacyDataConvection::instance_, effectId);
    if (effData == nullptr)
        return false;
    s32 abilityId = *(s32*)((u8*)effData + 0x0c);
    for (s32 p = 0; p < 15; p++) {
        pl::Player::cls* player = pl::PlayerParty::player(pl::PlayerParty::playerPartyInstance_, (pl::PLAYER_TYPES)p);
        s32 pid                 = *(s32*)((u8*)player + 0x10c);
        u32 k                   = (u32)(pid + 1);
        if (k > 5 || ((1u << k) & 0x23u) != 0)
            continue;
        if (pl::PlayerParty::isLearnByLevelUp(pl::PlayerParty::playerPartyInstance_, (pl::PLAYER_TYPES)pid, (common::ABILITY_ID)abilityId))
            continue;
        pl::LearningAbilityManager::cls* lam = pl::Player::learningAbility(player);
        if (pl::LearningAbilityManager::isLearning(lam, (common::ABILITY_ID)abilityId)) {
            return true;
        }
    }
    return false;
}

float get_progress(u8 id) {
    if (id >= achievementCount)
        return 0.0f;

    if (isCached[id]) {
        return progressCache[id];
    }

    float result = progress[id];

    if (result == 0.0f && id != 16) {
        auto gp = sys::GameParameter::gpInstance_;
        if (gp != nullptr) {
            const auto& param = achievementParams[id];
            if (param.m_Type == enAchievementCheckTypes::Flag) {
                auto flagManager = FlagManager::singleton();
                if (flagManager) {
                    int complete = 0,
                        total    = 0;

                    for (u8 i = 0; i < 20; i++) {
                        auto& condition = param.m_FlagParam[i];
                        if (condition.m_Bank < 0)
                            break;

                        total++;
                        if (FlagManager::get(flagManager, condition.m_Bank, condition.m_Offset) == condition.m_Value)
                            complete++;
                    }

                    result = (float)complete / (float)total * 100.0f;
                }
            } else if (param.m_Type == enAchievementCheckTypes::FlagRange) {
                auto flagManager = FlagManager::singleton();
                if (flagManager) {
                    int complete = 0,
                        total    = 0;

                    for (u8 i = 0; i < 20; i++) {
                        auto& condition = param.m_FlagRangeParam[i];
                        if (condition.m_Bank < 0)
                            break;

                        for (int j = condition.m_Start; j <= condition.m_End; j++) {
                            total++;
                            if (FlagManager::get(flagManager, condition.m_Bank, j) == condition.m_Value)
                                complete++;
                        }
                    }

                    result = (float)complete / (float)total * 100.0f;
                }
            } else if (param.m_Type == enAchievementCheckTypes::BeatCount) {
                result = (float)sys::GameParameter::gameClearCount(gp) / (float)param.m_BeatCountParam.m_Count * 100.0f;
            } else if (param.m_Type == enAchievementCheckTypes::GetItem) {
                auto pim = sys::GameParameter::item(gp);
                if (pim) {
                    int complete = 0,
                        total    = 0;

                    for (u8 i = 0; i < 60; i++) {
                        s16 itemId = param.m_GetItemParam[i].m_ItemId;
                        if (itemId < 0)
                            break;
                        total++;
                        if (itm::PossessionItemManager::searchNormalItem(pim, itemId) != nullptr) {
                            complete++;
                            continue;
                        }
                        for (s32 p = 0; p < 15; p++) {
                            auto player    = pl::PlayerParty::player(pl::PlayerParty::playerPartyInstance_, (pl::PLAYER_TYPES)p);
                            auto ep        = pl::Player::equipParameter(player);
                            s16* slots     = *(s16**)((u8*)ep + 0x08);
                            bool foundItem = false;
                            for (s32 s = 0; s < 5; s++) {
                                if (slots[s] == itemId) {
                                    complete++;
                                    foundItem = true;
                                    break;
                                }
                            }
                            if (foundItem)
                                break;
                        }
                    }

                    result = (float)complete / (float)total * 100.0f;
                }
            } else if (param.m_Type == enAchievementCheckTypes::GetDecant) {
                int complete = 0,
                    total    = 0;

                for (u8 i = 0; i < 60; i++) {
                    s16 id = param.m_GetDecantParam[i].m_DecantId;
                    if (id < 0)
                        break;
                    total++;
                    if (has_decant(id))
                        complete++;
                }

                result = (float)complete / (float)total * 100.0f;
            } else if (param.m_Type == enAchievementCheckTypes::GetDecantCnt) {
                int complete = 0;

                for (u8 i = 0; i < 39; i++) {
                    s32 id = AchievementCheckFuncs::DecantItemTbl[i];
                    if (has_decant(id))
                        complete++;
                }

                result = (float)complete / (float)param.m_GetDecantCntParam.m_Count * 100.0f;
            } else if (param.m_Type == enAchievementCheckTypes::LearnSummon) {
                int complete = 0,
                    total    = 0;

                for (u8 i = 0; i < 30; i++) {
                    auto& condition = param.m_LearnSummonParam[i];
                    if ((s32)condition.m_Player < 0)
                        break;
                    total++;
                    auto player        = pl::PlayerParty::player(pl::PlayerParty::playerPartyInstance_, condition.m_Player);
                    bool playerInParty = pl::PlayerParty::memberOrder(pl::PlayerParty::playerPartyInstance_, condition.m_Player) != -1;
                    if (player && playerInParty) {
                        auto lam = pl::Player::learningAbility(player);
                        if (lam) {
                            if (pl::LearningAbilityManager::isLearning(lam, condition.m_AbilityId))
                                complete++;
                        }
                    }
                }

                result = (float)complete / (float)total * 100.0f;
            } else if (param.m_Type == enAchievementCheckTypes::LearnSummonCnt) {
                auto flagManager = FlagManager::singleton();
                if (flagManager) {
                    auto& condition = param.m_LearnSummonCntParam;
                    bool has_flags  = true;
                    for (u8 i = 0; i < 20; i++) {
                        auto& condition = param.m_FlagParam[i];
                        if (condition.m_Bank < 0)
                            break;

                        if (!FlagManager::get(flagManager, condition.m_Bank, condition.m_Offset) == condition.m_Value)
                            has_flags = false;
                    }
                    if (has_flags) {
                        auto player        = pl::PlayerParty::player(pl::PlayerParty::playerPartyInstance_, condition.m_Player);
                        bool playerInParty = pl::PlayerParty::memberOrder(pl::PlayerParty::playerPartyInstance_, condition.m_Player) != -1;
                        if (player && playerInParty) {
                            auto lam = pl::Player::learningAbility(player);
                            if (lam) {
                                int complete = 0,
                                    total    = 0;

                                for (s16 i = 1500; i <= 1517; i++) {
                                    total++;
                                    if (pl::LearningAbilityManager::isLearning(lam, (common::ABILITY_ID)i))
                                        complete++;
                                }

                                result = (float)complete / (float)total * 100.0f;
                            }
                        }
                    }
                }
            } else if (param.m_Type == enAchievementCheckTypes::OverPlayerLevel) {
                int complete = 0,
                    total    = 0;

                for (u8 i = 0; i < 30; i++) {
                    auto& condition = param.m_OverPlayerLevelParam[i];
                    if ((s32)condition.m_Player < 0)
                        break;

                    total += condition.m_Level;

                    bool playerInParty = pl::PlayerParty::memberOrder(pl::PlayerParty::playerPartyInstance_, condition.m_Player) != -1;
                    if (!playerInParty)
                        continue;

                    void* sp = sys::GameParameter::playerSaveParameter(gp, condition.m_Player);
                    if (sp == nullptr)
                        continue;

                    u8 level = *(u8*)sp;
                    if (level > condition.m_Level)
                        complete += condition.m_Level;
                    else
                        complete += level;
                }

                result = (float)complete / (float)total * 100.0f;
            } else if (param.m_Type == enAchievementCheckTypes::Gill) {
                result = (float)*sys::GameParameter::gold(gp) / (float)param.m_GillParam.m_Amount * 100.0f;
            } else if (param.m_Type == enAchievementCheckTypes::MonsterKillCount) {
                auto mrdm = *mr::MRDMng;
                if (mrdm != nullptr) {
                    u32 kills = 0.0f;
                    for (s32 i = 0; i < (s32)mrdm->m_Count; i++) {
                        void* ref = mr::MonsterRefDataManager::monsterReferenceFromIndex(mrdm, i);
                        if (ref == nullptr)
                            continue;

                        s16 monsterId = *(s16*)ref;
                        auto mmm      = mon::MonsterManager::monsterManiaManager();
                        auto mm       = mon::MonsterManiaManager::monsterMania(mmm, (s32)monsterId);

                        if (mm != nullptr) {
                            kills += mm->m_KillCount;
                        }
                    }
                    result = (float)kills / (float)param.m_MonsterKillCountParam.m_Count * 100.0f;
                }
            } else if (param.m_Type == enAchievementCheckTypes::MonsterDictionary) {
                result = (float)mr::getCompleteRate() / (float)param.m_MonsterDictionaryParam.m_Amount * 100.0f;
            } else if (param.m_Type == enAchievementCheckTypes::AchievementComp) {
                int unlocked_count = 0;
                for (int i = 0; i < achievementCount; i++) {
                    if (i == id)
                        continue;
                    if (unlocked[i])
                        unlocked_count++;
                }
                result = (float)unlocked_count / (float)(achievementCount - 1) * 100.0f;
            }
        }
    }

    progressCache[id] = result;
    isCached[id]      = true;
    return result;
}
}