#pragma once

#include <ostream>

namespace app {

class consumable {

	public:

	enum class types{
		water_bottle,
		soda_bottle,
		beer_bottle, 
		beer_jar, 
		cocktail, 
		pinneaple,
		melon,
		watermelon
	};

	bool		operator==(const consumable& _c) const {return type==_c.type;}

	types		type;
};

}
