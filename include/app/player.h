#pragma once

#include <app/definitions.h>

namespace app {

class player {

	public:
					player(int, int, int, int);
	void			tick(float);
	void			set_size(int, int);
	void			set_position(int, int);
	//!_x or _y is expected to be between -1 and 1.
	void			set_motion(double _x, double _y);
	void			backup_position();
	void			apply_motion(axes, float);
	void			restore_previous_position();
	void			set_speed(double _speed) {speed=_speed;}
	

	const box&		get_collision_box() const {return collision_box;}
	const box&		get_previous_box() const {return previous_collision_box;}
	box&			ref_collision_box() {return collision_box;}

	private:

	box				collision_box,
					previous_collision_box;
	vector			motion;
	double			speed{0.};
};

}

