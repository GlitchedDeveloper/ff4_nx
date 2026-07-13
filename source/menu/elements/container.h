#pragma once

#include <string>
#include <vector>

#include "../element.h"
#include "../../types.h"

namespace menu::elements {
class Container : public virtual Element
{
  protected:
    std::vector<Element*> m_Elements;
    size_t m_CurrentElement = 0;

  public:
    Container();
    ~Container();
    void update() override;
    void preUpdate() override;
    void postUpdate() override;
    bool down(u32 key) override;
    bool held(u32 key, float time) override;
};
}