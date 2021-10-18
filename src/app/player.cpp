#include <app/player.h>

using namespace app;

player::player(int _x, int _y, int _w, int _h):
	collision_box(_x, _y, _w, _h),
	previous_collision_box{collision_box},
	motion{0., 0.}
{

}

void player::set_size(
	int _w, 
	int _h
) {

	collision_box.w=_w;
	collision_box.h=_h;
	previous_collision_box=collision_box;
}

void player::set_position(
	int _x, 
	int _y
) {

	collision_box.origin.x=_x;
	collision_box.origin.y=_y;
}

void player::tick(
	float /*_delta*/
) {

}

void player::backup_position() {

	previous_collision_box=collision_box;
}

void player::apply_motion(
	app::axes _axis,
	float _delta
) {

	switch(_axis) {

		case app::axes::x:

			if(motion.x) {

				collision_box.origin.x+=motion.x * _delta;
			}
		break;
		case app::axes::y:

			if(motion.y) {

				collision_box.origin.y+=motion.y * _delta;
			}
		break;
	}
}

void player::restore_previous_position() {

	collision_box=previous_collision_box;
}

void player::set_motion(
	double _x, 
	double _y
) {
	motion={_x*speed, _y*speed};

	if(_x) {

		current_face=_x < 0
			? facing::west
			: facing::east;
	}

	//north-south prevails over east west.
	if(_y) {

		current_face=_y < 0
			? facing::south
			: facing::north;
	}
}

void player::set_start_position(
	int _x,
	int _y
) {

	start_position.x=_x;
	start_position.y=_y;
}

void player::reset() {

	current_face=facing::north;
	set_position(start_position.x, start_position.y);
	previous_collision_box=collision_box;
	motion={0.,0.};
}
