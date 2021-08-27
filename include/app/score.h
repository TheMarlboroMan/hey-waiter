#pragma once

namespace app {

class score {

	public:

	//!scores a quantity of consumables, knowing missing and failed products.
	//!returns the scored value
	int						score_consumables(int, int, int);
	//!scores trash. returns the scored value.
	int						score_trash();
	void					reset();
	void					set_per_consumable(int _val) {per_consumable=_val;}
	void					set_per_trash(int _val) {per_trash=_val;}
	int						get() const {return score;}
	

	private:

	int						score{0};
	int						per_consumable{0};
	int						per_trash{0};
};


}
