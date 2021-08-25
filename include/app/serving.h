#pragma once

#include "order.h"
#include "consumable.h"

#include <vector>

namespace app {

/**
*A serving is what is currently being served at a table.
*/
class serving {

	public:

	std::size_t						size() const {return products.size();}
	void							add(const consumable&);
	const std::vector<consumable>&	get() const {return products;}
	void							reset() {products.clear();}

	private:

	std::vector<consumable>			products;
};

}
