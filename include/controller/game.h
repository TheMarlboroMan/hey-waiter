#pragma once

//local
#include "states.h"
#include <app/game.h>

#include <app/dependency_container.h>
#include <app/env.h>
#include <app/resources.h>
#include <app/draw.h>
#include <app/score.h>

#include <dfw/controller_interface.h>
#include <lda/resource_manager.h>
#include <dfw/audio.h>
#include <lm/logger.h>
#include <ldv/camera.h>
#include <ldtools/ttf_manager.h>
#include <tools/i8n.h>

#include <cmath>

namespace controller {

class game:
	public dfw::controller_interface {

	public:

								game(app::dependency_container&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	//references...
	lm::logger&					log;
	const app::env&				env;

	//properties
	ldv::camera					camera;
	app::game					game_instance;
	app::draw					draw_instance;
};

}
