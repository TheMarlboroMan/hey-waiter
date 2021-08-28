#pragma once

//local
#include "states.h"
#include <app/env.h>
#include <app/resources.h>
#include <app/hi_score.h>


#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/ttf_manager.h>
#include <tools/i8n.h>
#include <ldtools/view_composer.h>

#include <cmath>

namespace controller {

class menu:
	public dfw::controller_interface {

	public:

								menu(lm::logger&,
									const app::env&,
									const app::resources&,
									const ldtools::ttf_manager&,
									const tools::i8n&,
									app::hi_score_manager&
								);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/);
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	//references...
	lm::logger&					log;
	const app::env&				env;
	const app::resources&		resources;
	const tools::i8n&			i8n;
	app::hi_score_manager&		hi_scores;

	//properties
	ldtools::view_composer		layout;
};

}
