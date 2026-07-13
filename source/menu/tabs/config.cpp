#include "config.h"

#include "config/controls.h"
#include "config/debug.h"
#include "config/gameplay.h"
#include "config/general.h"
#include "config/visuals.h"

namespace menu::tabs {
Config::Config()
    : TabContainer("Configuration") {
    m_Elements.push_back(new General());
    m_Elements.push_back(new Visuals());
    m_Elements.push_back(new Gameplay());
    m_Elements.push_back(new Controls());
    m_Elements.push_back(new Debug());
};
}
