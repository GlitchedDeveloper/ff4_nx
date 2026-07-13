#pragma once

#include "../../types.h"
#include "../elements/container.h"
#include "../tab.h"

namespace menu::tabs {
class Credits : public Tab
{
  public:
    Credits();
    void update() override;
};
}