#pragma once

#include "../../../../elements/container.h"
#include "../../../../tab.h"

namespace menu::tabs::gameplay::debug {
class Battle : public elements::Container, public Tab
{
  public:
    Battle();
};
}