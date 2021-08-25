#include <app/tray.h>

using namespace app;

tray::tray() {

	contents.reserve(max_size);
}

void tray::pop() {

	if(!contents.size()) {

		return;
	}

	contents.erase(contents.end()-1);
	current_index=0;
}

void tray::reset() {

	contents.clear();
	trash=false;
	current_index=0;
}

consumable tray::remove_current() {

	auto res=contents.at(current_index);
	contents.erase(std::begin(contents)+current_index);
	if(contents.size()) {

		0==current_index
			? next()
			: prev();
	}
	else {
		current_index=0;
	}

	return res;

}

void tray::next() {

	++current_index;
	if(current_index==contents.size()) {

		current_index=0;
	}
}

void tray::prev() {

	if(0==current_index) {

		current_index=contents.size()-1;
		return;
	}

	--current_index;
}
