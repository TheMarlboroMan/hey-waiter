#include <app/loop_stage.h>

using namespace app;

loop_stage::loop_stage(
	int _until, 
	int _chance, 
	int _cooloff, 
	int _min_orders, 
	int _max_orders, 
	int _min_consumables, 
	int _max_consumables
): 
	until{_until},
	customer_chance{_chance}, 
	customer_cooloff{_cooloff}, 
	min_orders{_min_orders}, 
	max_orders{_max_orders}, 
	min_consumables{_min_consumables}, 
	max_consumables{_max_consumables}
{

}

