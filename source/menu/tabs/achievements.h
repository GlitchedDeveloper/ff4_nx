#pragma once

#include <vector>

#include "../elements/container.h"
#include "../tab.h"

namespace menu::tabs {
class Achievements : public Tab
{
  protected:
    size_t m_CurrentElement = 0;
    bool m_UpdateScroll     = false;
    bool m_TouchStarted     = false;
    float m_TouchStartY     = 0.0f;
    std::vector<int> m_VisibleIds;
    float m_LastMoveTime = 0.0f;

  public:
    Achievements();
    void update() override;
    bool down(u32 key) override;
    bool held(u32 key, float time) override;
};
}