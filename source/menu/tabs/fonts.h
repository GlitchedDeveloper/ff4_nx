#pragma once

#include "../elements/container.h"
#include "../tab.h"

namespace menu::tabs {
class Fonts : public Tab
{
  protected:
    size_t m_Selected        = 0;
    size_t m_CurrentSelected = -1;
    size_t m_CurrentElement  = 0;
    bool m_UpdateScroll      = false;
    std::vector<std::string> m_Fonts;

  public:
    Fonts();
    void update() override;
    void postUpdate() override;
    bool down(u32 key) override;
};
}