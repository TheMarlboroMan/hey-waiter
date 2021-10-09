#include <app/obstacle.h>

using namespace app;

obstacle::obstacle(
	int _x,
	int _y,
	int _w,
	int _h
):
	collision_box(_x, _y, _w, _h)
{

}

