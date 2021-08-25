#pragma once

#include "consumable.h"

#include <vector>

namespace app {

struct check_result {

	int					ok{0}, //!< consumables given that were in the order.
						missing{0}, //!< consumables in the order, but not given.
						failed{0}; //!< consumables given that weren't in the order.
};

class order_checker {

	public:

	check_result		check(const std::vector<consumable>&, const std::vector<consumable>&);
};

}
