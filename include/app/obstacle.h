#pragma once

#include <app/definitions.h>

namespace app {

class obstacle {

	public:
								obstacle(int _x, int _y, int _w, int _h);

	const box&					get_collision_box() const {return collision_box;}

	private:

	box							collision_box;
};

}
