#include <app/serving.h>

using namespace app;

void serving::add(
	const consumable& _item
) {

	products.push_back(_item);
}
