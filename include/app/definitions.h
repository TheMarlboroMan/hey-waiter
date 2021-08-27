#pragma once

#include <ldt/box.h>
#include <ldt/point_2d.h>
#include <ldt/vector_2d.h>

namespace app {

using point=ldt::point_2d<double>;
using box=ldt::box< double, double>;
using vector=ldt::vector_2d<double>;
enum class axes{x, y};
enum class directions{up, right, down, left};
}
