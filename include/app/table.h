#pragma once

#include <app/definitions.h>
#include <app/customer.h>

#include <memory>

namespace app {

class table {

	public:

								table(int, int, int, int, int, directions);
	bool						is_demanding_attention() const {return state==states::demanding_attention;}
	bool						is_waiting_order() const {return state==states::waiting_order;}
	bool						is_dirty() const {return state==states::dirty;}
	bool						can_be_interacted_with() const {return state==states::demanding_attention || state==states::waiting_order || state==states::dirty;}

	const box&					get_collision_box() const {return collision_box;}
	const box&					get_interaction_box() const {return interaction_box;}

	enum class	states{free, demanding_attention, waiting_order, consuming, dirty};

	private:

	states 						state{states::free};
	box							collision_box,
								interaction_box;
	directions					customer_direction;
	std::unique_ptr<customer>	customer_instance{nullptr};
};

}

