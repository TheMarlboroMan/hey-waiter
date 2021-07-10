#include "../../include/controller/_template.h"

//local
#include "../../include/input/input.h"

using namespace controller;

_template::_template(lm::logger& plog)
	:log(plog) {

}

void _template::loop(dfw::input& _input, const dfw::loop_iteration_data& /*lid*/) {

	if(_input().is_exit_signal() || _input.is_input_down(input::escape)) {
		set_leave(true);
		return;
	}
}

void _template::draw(ldv::screen& screen, int /*fps*/) {
	screen.clear(ldv::rgba8(0, 0, 0, 255));
}
