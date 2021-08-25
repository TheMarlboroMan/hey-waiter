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

	void						reset() {products.clear();}
	std::vector<consumable>		products;
};


}

