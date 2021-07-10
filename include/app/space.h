#pragma once

#include "definitions.h"

namespace app {

bool is_left_of(const box&, const box&);
bool is_right_of(const box&, const box&);
bool is_above(const box&, const box&);
bool is_below(const box&, const box&);

}

