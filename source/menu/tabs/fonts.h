#pragma once

#include "../../types.h"
#include "../elements/container.h"
#include "../tab.h"

namespace menu::tabs {
class Fonts : public elements::Container, public Tab
{
  protected:
    bool m_IsReordering = false;
    bool m_Dirty        = false;
    std::vector<size_t> m_FontIndices;
    void swap(size_t from, size_t to);

  public:
    Fonts();
    void update() override;
    void unfocus() override;
    bool down(u32 key) override;
    static void rebuildImGuiAtlas();
};
}
