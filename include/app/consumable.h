#pragma once

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

	types		type;
};

}
