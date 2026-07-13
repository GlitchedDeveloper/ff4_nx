#pragma once

#include "../../elements/container.h"
#include "../../tab.h"

namespace menu::tabs {
class General : public elements::Container, public Tab
{
  public:
    General();
};
}