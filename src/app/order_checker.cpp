#include <app/order_checker.h>
#include <algorithm>

#include <iostream>

using namespace app;

check_result order_checker::check(
	const std::vector<consumable>& _given,
	const std::vector<consumable>& _order
) {

	check_result res{0,0,0};

	//A few copies...
	auto given=_given;
	auto order=_order;

	//for each given item, if present in the order, remove from the order and
	//add to success, if not, add to failed. whatever is left if the missing
	//marker.

	while(given.size()) {

		auto consumable=given.back();
		given.pop_back();

		auto it=std::find(std::begin(order), std::end(order), consumable);

		if(std::end(order) == it) {

			++res.failed;
		}
		else {

			order.erase(it);
			++res.ok;
		}
	}

	res.missing=order.size();
	return res;
}
