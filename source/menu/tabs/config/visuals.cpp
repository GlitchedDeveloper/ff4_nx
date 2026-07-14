#include "visuals.h"

#include "visuals/font.h"
#include "visuals/movie.h"
#include "visuals/overlay.h"
#include "visuals/ui.h"

namespace menu::tabs {
using namespace visuals;
Visuals::Visuals()
    : TabContainer("Visuals") {
    m_Elements.push_back(new UI());
    m_Elements.push_back(new Overlay());
    m_Elements.push_back(new Font());
    m_Elements.push_back(new Movie());
};
}
