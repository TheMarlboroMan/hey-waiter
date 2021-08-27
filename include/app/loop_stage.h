#pragma once

namespace app {

class loop_stage {

	public:

			loop_stage(int, int, int, int, int, int, int, int, int);

	int		get_until() const {return until;}
	int		calculate_chance(int, int) const;
	int		get_customer_cooloff() const {return customer_cooloff;}
	int		get_min_orders() const {return min_orders;}
	int		get_max_orders() const {return max_orders;}
	int		get_min_consumables() const {return min_consumables;}
	int		get_max_consumables() const {return max_consumables;}

	private:

	int		until{0},
			customer_chance{0},
			chance_occupied_table_modifier{0},
			chance_dirty_table_modifier{0},
			customer_cooloff{0},
			min_orders{0},
			max_orders{0},
			min_consumables{0},
			max_consumables{0};
};
}
