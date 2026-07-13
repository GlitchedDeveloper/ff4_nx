#include "battle.h"

#include "../../../../../config.h"
#include "../../../../elements/boolean.h"

namespace menu::tabs::gameplay::debug {
using namespace elements;

Battle::Battle()
    : Tab("Battle") {
    m_Elements.push_back(new Boolean("SURELY_MAX_DAMAGE", config::SURELY_MAX_DAMAGE, [](bool value) {
        config::SURELY_MAX_DAMAGE = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("QUICK_WAIT", config::QUICK_WAIT, [](bool value) {
        config::QUICK_WAIT = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("ENEMY_INVINCIBLE", config::ENEMY_INVINCIBLE, [](bool value) {
        config::ENEMY_INVINCIBLE = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("FRIEND_INVINCIBLE", config::FRIEND_INVINCIBLE, [](bool value) {
        config::FRIEND_INVINCIBLE = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("RESTART", config::RESTART, [](bool value) {
        config::RESTART = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("SURELY_ESCAPE", config::SURELY_ESCAPE, [](bool value) {
        config::SURELY_ESCAPE = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("SURELY_CONDITION", config::SURELY_CONDITION, [](bool value) {
        config::SURELY_CONDITION = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("TRANSFIX", config::TRANSFIX, [](bool value) {
        config::TRANSFIX = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("MDEF_INVALIDATION", config::MDEF_INVALIDATION, [](bool value) {
        config::MDEF_INVALIDATION = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("QUICK_EVENT", config::QUICK_EVENT, [](bool value) {
        config::QUICK_EVENT = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("SURELY_HIT", config::SURELY_HIT, [](bool value) {
        config::SURELY_HIT = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("SURELY_MISS", config::SURELY_MISS, [](bool value) {
        config::SURELY_MISS = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("SURELY_CRITICAL", config::SURELY_CRITICAL, [](bool value) {
        config::SURELY_CRITICAL = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("MONSTER_STOP_ACTION", config::MONSTER_STOP_ACTION, [](bool value) {
        config::MONSTER_STOP_ACTION = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("DAMAGE_OVER_LIMIT", config::DAMAGE_OVER_LIMIT, [](bool value) {
        config::DAMAGE_OVER_LIMIT = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("OPEN_ENEMY_HP", config::OPEN_ENEMY_HP, [](bool value) {
        config::OPEN_ENEMY_HP = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("PHYSICS_RANDOM_MAX", config::PHYSICS_RANDOM_MAX, [](bool value) {
        config::PHYSICS_RANDOM_MAX = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("MAGIC_RANDOM_MAX", config::MAGIC_RANDOM_MAX, [](bool value) {
        config::MAGIC_RANDOM_MAX = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("QUICK_TURN", config::QUICK_TURN, [](bool value) {
        config::QUICK_TURN = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("DRAW_CHAR_TOUCH_RECT", config::DRAW_CHAR_TOUCH_RECT, [](bool value) {
        config::DRAW_CHAR_TOUCH_RECT = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("MODEL_CHANGE_CHECK", config::MODEL_CHANGE_CHECK, [](bool value) {
        config::MODEL_CHANGE_CHECK = value;
        config::mark_for_write();
    }));
    m_Elements.push_back(new Boolean("MODEL_CHANGE_SOUND", config::MODEL_CHANGE_SOUND, [](bool value) {
        config::MODEL_CHANGE_SOUND = value;
        config::mark_for_write();
    }));
};
}