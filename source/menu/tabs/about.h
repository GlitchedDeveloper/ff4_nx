#pragma once

#include "../../types.h"
#include "../elements/container.h"
#include "../tab.h"

namespace menu::tabs {
class About : public Tab
{
  public:
    About();
    void update() override;
};
}