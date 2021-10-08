#pragma once

//local
#include "states.h"
#include "app/dependency_container.h"
#include "app/env.h"
#include <app/resources.h>

#include <dfw/controller_interface.h>
#include <dfw/audio.h>
#include <lda/resource_manager.h>
#include <lm/logger.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/view_composer.h>
#include <tools/i8n.h>

//std
#include <cmath>

namespace controller {

class how_to_play:
	public dfw::controller_interface {

	public:

								how_to_play(app::dependency_container&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/) {}
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}

	private:

	//references...
	lm::logger&					log;
	const app::env&				env;
	dfw::audio&					audio;
	const lda::resource_manager& audio_rm;
	const tools::i8n&			i8n;

	//properties
	ldtools::view_composer      layout;
};

}
