#include <app/table.h>

using namespace app;

table::table(
	int _id,
	int _x,
	int _y,
	int _w,
	int _h,
	int _margin,
	directions _customer_direction
):
	id{_id},
	collision_box(_x, _y, _w, _h),
	interaction_box(_x-_margin, _y-_margin, _w+(2*_margin), _h+(2*_margin)),
	customer_direction{_customer_direction}
{

}

void table::tick(
	float _delta
) {

	time_counter-=_delta;
}

void table::reset() {

	customer_served_orders=0;
	time_counter=0.f;
	state=states::free;
	orders.clear();
	current_order.products.clear();
	dirty=false;
}

void table::set_to_customer_arrival(
	float _timer, 
	std::vector<order>&& _orders
) {

	time_counter=_timer;
	orders=std::move(_orders);
	state=states::customer_arriving;
}

void table::set_to_demand_attention(
	float _timer
) {

	state=states::demanding_attention;
	time_counter=_timer;
}

void table::set_to_customer_leaving(
	float _timer
) {

	//the reset will fuck this up.
	auto copy_served=customer_served_orders;

	reset();

	dirty=copy_served;
	time_counter=_timer;
	state=states::customer_leaving;
}

void table::serve() {

	++customer_served_orders;
}

void table::set_to_wait_for_order(
	float _timer
) {

	state=states::waiting_order;
	time_counter=_timer;
}

void table::pop_order() {

	current_order=orders.back();
	orders.pop_back();
}

void table::set_to_consuming(
	float _timer
) {
	state=states::consuming;
	current_order.products.clear();
	time_counter=_timer;
}
