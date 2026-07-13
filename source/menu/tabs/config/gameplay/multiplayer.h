#pragma once

#include "../../../elements/container.h"
#include "../../../tab.h"

namespace menu::tabs::gameplay {
class Multiplayer : public elements::Container, public Tab
{
  public:
    Multiplayer();
};
}