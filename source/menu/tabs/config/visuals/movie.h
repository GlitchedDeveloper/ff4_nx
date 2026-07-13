#pragma once

#include "../../../elements/container.h"
#include "../../../tab.h"

namespace menu::tabs::visuals {
class Movie : public elements::Container, public Tab
{
  public:
    Movie();
    void update() override;
};
}