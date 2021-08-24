#pragma once

#include "consumable.h"

#include <vector>
#include <ostream>

namespace app {

/**
*a order is a bunch of consumables that a customer orders.
*/

class order {

	public: 

	std::vector<consumable>		products;
};


}

