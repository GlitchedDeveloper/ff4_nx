#pragma once

#include "../elements/container.h"
#include "../tab.h"
#include "../../types.h"

namespace menu::tabs {
class Launch : public Tab
{
  public:
    Launch();
    void update() override;
    bool down(u32 key) override;
};
}