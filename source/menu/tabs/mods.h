#pragma once

#include "../elements/container.h"
#include "../tab.h"
#include "../../types.h"

namespace menu::tabs {
class Mods : public elements::Container, public Tab
{
  protected:
    bool m_IsReordering = false;
    std::vector<size_t> m_ObbIndices;
    void swap(size_t from, size_t to);

  public:
    Mods();
    void update() override;
    bool down(u32 key) override;
};
}