#pragma once

#include "../types.h"

namespace menu {
class Element
{
  protected:
    bool m_IsFocused = false;

  public:
    virtual ~Element() = default;
    virtual void update();
    virtual void preUpdate();
    virtual void postUpdate();
    virtual bool down(u32 key);
    virtual bool held(u32 key, float time);
    virtual void setFocused(bool focused);
};
}