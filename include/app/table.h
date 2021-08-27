#pragma once

#include <app/definitions.h>
#include <app/order.h>

#include <memory>
#include <vector>

namespace app {

class table {

	public:

								table(int, int, int, int, int, int, directions);
	bool						can_roll_for_customer() const {return state==states::free && time_counter <= 0.f;}
	bool						is_free() const {return state==states::free;}
	bool						is_demanding_attention() const {return state==states::demanding_attention;}
	bool						is_customer_arriving() const {return state==states::customer_arriving;}
	bool						is_customer_arrived() const {return state==states::customer_arriving && time_counter <= 0.f;}
	bool						is_customer_tired_of_waiting() const {return state==states::demanding_attention && time_counter <= 0.f;}
	bool						is_customer_leaving() const {return state==states::customer_leaving;}
	bool						is_waiting_order() const {return state==states::waiting_order;}
	bool						is_done_with_order() const {return state==states::consuming && time_counter <= 0.f;}
	bool						is_dirty() const {return dirty;}
	bool						is_consuming() const {return state==states::consuming;}
	bool						can_be_interacted_with() const {return state==states::demanding_attention || state==states::waiting_order || dirty;}
	bool						is_timer_done() const {return time_counter <= 0.f;}
	bool						has_customer_left() const {return state==states::customer_leaving && time_counter <= 0.f;}
	bool						has_orders_left() const {return orders.size();}
	const box&					get_collision_box() const {return collision_box;}
	const box&					get_interaction_box() const {return interaction_box;}
	void						tick(float _delta);
	void						serve();
	//TODO: Should this include time_counter value???
	void						clean() {dirty=false;}
	void						reset();
	void						set_to_customer_arrival(float, std::vector<order>&&);
	void						set_to_demand_attention(float);
	void						set_to_wait_for_order(float);
	void						set_to_customer_leaving(float);
	void						set_timer(float _val) {time_counter=_val;}
	void						set_to_consuming(float _val);
	void						set_free() {state=states::free;}
	//!pops the last order, puts it into "current_order"
	void						pop_order();
	int							get_id() const {return id;}
	const order&				get_current_order() const {return current_order;}

	enum class	states{free, customer_arriving, demanding_attention, waiting_order, consuming, customer_leaving};

	private:

	int							id{0},
								customer_served_orders{0};
	states 						state{states::free};
	box							collision_box,
								interaction_box;
	directions					customer_direction;
	float						time_counter{0.f};
	std::vector<order>			orders;
	order						current_order;
	bool						dirty{false};
};

}

