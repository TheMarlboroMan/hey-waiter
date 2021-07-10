#pragma once

//local
#include "states.h"

//framework
#include <dfw/controller_interface.h>

//logger
#include <lm/logger.h>

//std
#include <cmath>

namespace controller {

class _template:
	public dfw::controller_interface {

	public:

								_template(lm::logger&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	//references...
	lm::logger&					log;

	//properties
};

}
