#pragma once

#include <lm/logger.h>

namespace app {

struct order;
struct consumable;
struct table;

lm::logger& operator<<(lm::logger&, const consumable&);
lm::logger& operator<<(lm::logger&, const order&);
lm::logger& operator<<(lm::logger&, const table&);

}
