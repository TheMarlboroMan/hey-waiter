#pragma once

#include <app/definitions.h>

namespace app {

class world {

	public:
					world(int, int);

	void			set_size(int, int);
	const box&		get_collision_box() const {return collision_box;}

	private:

	box				collision_box;
};

}

