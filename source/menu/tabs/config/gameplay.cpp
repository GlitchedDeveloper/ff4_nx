#include "gameplay.h"

#include "gameplay/boosts.h"
#include "gameplay/cheats.h"
#include "gameplay/debug.h"
// #include "gameplay/misc.h"
#include "gameplay/multiplayer.h"

namespace menu::tabs {
using namespace gameplay;
Gameplay::Gameplay()
    : TabContainer("Gameplay") {
    m_Elements.push_back(new Multiplayer());
    m_Elements.push_back(new Boosts());
    m_Elements.push_back(new Cheats());
    // m_Elements.push_back(new Misc());
    m_Elements.push_back(new Debug());
};
}
