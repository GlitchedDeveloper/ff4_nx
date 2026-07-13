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

enum FloatFlags : u32
{
    FloatFlags_None = 0
};

namespace menu::elements {
class Float : public Element
{
  protected:
    std::string m_Name;
    float m_Min;
    float m_Max;
    float m_Step;
    float m_Value;
    std::function<void(float)> m_Callback;
    u32 m_Flags;

  public:
    Float(const char* name, float min, float max, float step, float value, std::function<void(float)> callback, u32 flags = FloatFlags_None);
    void update() override;
    bool down(u32 key) override;
};
}