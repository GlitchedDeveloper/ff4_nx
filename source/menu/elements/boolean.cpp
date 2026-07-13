#include "boolean.h"

namespace menu::elements {
const char* booleanOptions[2] = { "On", "Off" };
Boolean::Boolean(const char* name, bool state, std::function<void(bool)> callback, u32 flags)
    : Option(name, booleanOptions, 2, state ? 0 : 1, [callback](int value) { callback(value == 0); }, flags) { };
}