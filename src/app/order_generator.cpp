#include <app/order_generator.h>

#include <stdexcept>
#include <sstream>


using namespace app;

order_generator::order_generator()
	:consumable_generator(0, 7)
{

}

std::vector<order> order_generator::generate(
	int _min_orders,
	int _max_orders,
	int _min_consumables,
	int _max_consumables
) {

	tools::int_generator ordgen{_min_orders, _max_orders};
	tools::int_generator congen{_min_consumables, _max_consumables};

	auto order_count=ordgen();

	std::vector<order> result;
	result.reserve(order_count);
	for(int i=0; i<order_count; i++) {

		order order_instance{};
		auto consumable_count=congen();
		for(int j=0; j<consumable_count; j++) {

			order_instance.products.push_back({get_consumable()});
		}
		
		result.push_back(order_instance);
	}

	return result;
}

consumable::types order_generator::get_consumable() {

	int val=consumable_generator();

	switch(val) {
	
		case 0: return consumable::types::water_bottle;
		case 1:	return consumable::types::soda_bottle;
		case 2:	return consumable::types::beer_bottle;
		case 3:	return consumable::types::beer_jar;
		case 4:	return consumable::types::cocktail;
		case 5:	return consumable::types::pinneaple;
		case 6:	return consumable::types::melon;
		case 7:	return consumable::types::watermelon;
	}

	std::stringstream ss;
	ss<<"unable to get consumable type "<<val;

	throw std::runtime_error(ss.str());
}
