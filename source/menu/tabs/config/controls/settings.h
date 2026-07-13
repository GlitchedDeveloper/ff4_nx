#pragma once

#include "../../../elements/container.h"
#include "../../../tab.h"

namespace menu::tabs::controls {
class Settings : public elements::Container, public Tab
{
  public:
    Settings();
};
}