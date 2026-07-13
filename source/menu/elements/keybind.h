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

enum KeybindFlags : u32
{
    KeybindFlags_None        = 0,
    KeybindFlags_AllowUnbind = BIT(0),
};

namespace menu::elements {
class Keybind : public Element
{
  protected:
    std::string m_Name;
    u64* m_KeyPtr;
    u64 m_Key;
    std::function<void(u64)> m_Callback;
    u32 m_Flags;
    bool m_Activate = false;
    bool m_Active   = false;

  public:
    Keybind(const char* name, u64* keyptr, std::function<void(u64)> callback, u32 flags = KeybindFlags_None);
    void update() override;
    bool down(u32 key) override;
};
}