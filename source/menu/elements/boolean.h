#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

#include "option.h"

namespace menu::elements {
class Boolean : public Option
{
  public:
    Boolean(const char* name, bool state, std::function<void(bool)> callback, u32 flags = OptionFlags_None);
};
}