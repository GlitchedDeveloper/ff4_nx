#pragma once

#include <string>

#include "element.h"

namespace menu {
class Tab : public virtual Element
{
  public:
    Tab(const char* name);
    std::string m_Name;
    virtual void unfocus() { }
};
}