#pragma once

#include "consumable.h"

namespace app {

class bar_selector {

	public:

	void				reset();
	void				next();
	void				prev();
	consumable::types	get() const;

	private:

	int					index{0};
	
};

}
