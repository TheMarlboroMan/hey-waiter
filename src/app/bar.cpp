#include <app/bar.h>

using namespace app;

bar::bar(
	int _x,
	int _y,
	int _w,
	int _h,
	int _margin
):
	collision_box(_x, _y, _w, _h),
	interaction_box(_x-_margin, _y-_margin, _w+(2*_margin), _h+(2*_margin))
{

}

