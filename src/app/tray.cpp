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
}
