#include <app/world.h>

using namespace app;

world::world(int _w, int _h):
	collision_box(0, 0, _w, _h) {

}

void world::set_size(
	int _w, 
	int _h
) {

	collision_box.w=_w;
	collision_box.h=_h;
}
