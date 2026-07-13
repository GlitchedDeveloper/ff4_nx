#pragma once

#include "../../elements/container.h"
#include "../../tab.h"

namespace menu::tabs {
class Debug : public elements::Container, public Tab
{
  public:
    Debug();
};
}
