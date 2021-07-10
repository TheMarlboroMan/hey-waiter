#pragma once

#include <app/definitions.h>

namespace app {

class trash {

	public:

								trash(int, int, int, int, int);

	const box&					get_collision_box() const {return collision_box;}
	const box&					get_interaction_box() const {return interaction_box;}

	private:

	box							collision_box,
								interaction_box;
};

}

