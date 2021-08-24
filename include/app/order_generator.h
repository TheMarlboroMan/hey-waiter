#pragma once

#include <app/order.h>
#include <app/consumable.h>

#include <tools/number_generator.h>
#include <vector>

namespace app {

class order_generator {

	public:

							order_generator();
	std::vector<order>		generate(int, int, int, int);
	consumable::types		get_consumable();

	private:

	tools::int_generator	consumable_generator;
	
};

}
