#pragma once

#include "consumable.h"

#include <vector>

namespace app {

/**
*a order is a bunch of consumables that a customer orders.
*/

class order {

	static const std::size_t min_size{1};
	static const std::size_t max_size{5};

	std::vector<consumable>		products;

};

}

