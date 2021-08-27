#include <app/loop_stage.h>

using namespace app;

loop_stage::loop_stage(
	int _until, 
	int _chance, 
	int _chance_table_mod,
	int _chance_dirty_mod,
	int _cooloff, 
	int _min_orders, 
	int _max_orders, 
	int _min_consumables, 
	int _max_consumables
): 
	until{_until},
	customer_chance{_chance}, 
	chance_occupied_table_modifier{_chance_table_mod},
	chance_dirty_table_modifier{_chance_dirty_mod},
	customer_cooloff{_cooloff}, 
	min_orders{_min_orders}, 
	max_orders{_max_orders}, 
	min_consumables{_min_consumables}, 
	max_consumables{_max_consumables}
{

}

int loop_stage::calculate_chance(
	int _occupied,
	int _dirty
) const {

	auto result=customer_chance;
	result-=_occupied * chance_occupied_table_modifier;
	result-=_dirty * chance_dirty_table_modifier;
	
	return result < 0
		? 0
		: result;
}
