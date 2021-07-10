#include <app/bar_selector.h>

using namespace app;

void bar_selector::reset() {

	index=0;
}

void bar_selector::next() {

	++index;
	if(index==8) {

		index=0;
	}
}

void bar_selector::prev() {

	if(index==0) {

		index=7;
		return;
	}

	--index;
}

consumable::types bar_selector::get() const {

	switch(index) {

		case 0: return consumable::types::water_bottle;
		case 1: return consumable::types::soda_bottle;
		case 2: return consumable::types::beer_bottle;
		case 3: return consumable::types::beer_jar;
		case 4: return consumable::types::cocktail;
		case 5: return consumable::types::pinneaple;
		case 6: return consumable::types::melon;
		case 7: return consumable::types::watermelon;
	}

	return consumable::types::water_bottle;
}
