#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

#include "../../types.h"
#include "../element.h"

#ifndef BIT
#define BIT(n) (1U << (n))
#endif

enum OptionFlags : u32
{
    OptionFlags_None            = 0,
    OptionFlags_CondenseOptions = BIT(0),
    OptionFlags_RequiresRestart = BIT(1)
};

namespace menu::elements {
class Option : public Element
{
  protected:
    std::string m_Name;
    std::vector<std::string> m_Options;
    std::function<void(int)> m_Callback;
    long m_State;
    u32 m_Flags;

  public:
    Option(const char* name, const char** options, size_t options_size, int state, std::function<void(int)> callback, u32 flags = OptionFlags_None);
    void update() override;
    bool down(u32 key) override;
};
}