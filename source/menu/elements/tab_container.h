#pragma once

#include <vector>

#include "../tab.h"
#include "container.h"
#include "../../types.h"

namespace menu::elements {
class TabContainer : public Container, public Tab
{
  public:
    TabContainer(const char* name);
    virtual ~TabContainer();
    void update() override;
    bool down(u32 key) override;
};
}