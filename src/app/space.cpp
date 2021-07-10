#include <app/space.h>

using namespace app;

bool app::is_left_of(
	const box& _subject,
	const box& _other
) {

	return _subject.origin.x + _subject.w <= _other.origin.x;
}

bool app::is_right_of(
	const box& _subject, 
	const box& _other
) {

	return _subject.origin.x >= _other.origin.x + _other.w;
}

bool app::is_above(
	const box& _subject, 
	const box& _other
) {

	return _subject.origin.y >= _other.origin.y+_other.h;
}

bool app::is_below(
	const box& _subject, 
	const box& _other
) {

	return _subject.origin.y+_subject.h <= _other.origin.y;
}
