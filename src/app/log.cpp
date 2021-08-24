#include <app/log.h>
#include <app/consumable.h>
#include <app/table.h>
#include <app/order.h>

using namespace app;

lm::logger& app::operator<<(
	lm::logger& _stream, 
	const consumable& _consumable
) {

	switch(_consumable.type) {

		case consumable::types::water_bottle: 
			return _stream<<"water bottle";
		case consumable::types::soda_bottle: 
			return _stream<<"soda bottle";
		case consumable::types::beer_bottle:
			return _stream<<"beer bottle";
		case consumable::types::beer_jar:
			return _stream<<"beer jar";
		case consumable::types::cocktail: 
			return _stream<<"cocktail";
		case consumable::types::pinneaple:
			return _stream<<"pineapple";
		case consumable::types::melon:
			return _stream<<"melon";
		case consumable::types::watermelon:
			return _stream<<"watermelon";
	}

	return _stream;
}

lm::logger& app::operator<<(
	lm::logger& _stream, 
	const order& _order
) {

	_stream<<"order ("<<_order.products.size()<<"): ";
	for(const auto& c : _order.products) {
		_stream<<c<<".";
	}

	return _stream;
}

lm::logger& app::operator<<(
	lm::logger& _stream, 
	const table& _table
) {

	_stream<<"table ["<<_table.get_id()<<"]";
	return _stream;
}
