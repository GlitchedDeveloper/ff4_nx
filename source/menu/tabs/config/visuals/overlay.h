#pragma once

#include "../../../elements/container.h"
#include "../../../tab.h"

namespace menu::tabs::visuals {
class Overlay : public elements::Container, public Tab
{
  public:
    Overlay();
};
}