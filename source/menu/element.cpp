#include "element.h"

namespace menu {

void Element::update() {
}

void Element::preUpdate() {
}

void Element::postUpdate() {
}

bool Element::down(u32 key) {
    return false;
}

bool Element::held(u32 key, float time) {
    return false;
}

void Element::setFocused(bool focused) {
    m_IsFocused = focused;
}
}