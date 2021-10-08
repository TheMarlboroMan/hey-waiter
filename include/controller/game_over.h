#pragma once

//local
#include "states.h"
#include <app/dependency_container.h>
#include <app/env.h>
#include <app/resources.h>
#include <app/hi_score.h>
#include <app/score.h>
#include <app/enter_name.h>


#include <ldtools/ttf_manager.h>
#include <tools/i8n.h>
#include <ldtools/view_composer.h>
#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <cmath>

namespace controller {

class game_over:
	public dfw::controller_interface {

	public:

								game_over(app::dependency_container&);
	virtual void 				loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void 				draw(ldv::screen&, int);
	virtual void 				awake(dfw::input& /*input*/);
	virtual void 				slumber(dfw::input& /*input*/) {}
	virtual bool				can_leave_state() const {return true;}
	virtual void 				request_draw(dfw::controller_view_manager& cvm);

	private:

	void		 				hi_score_input(dfw::input&, const dfw::loop_iteration_data&);
	void						set_caret();

	enum class modes{hi_score_input, game_over_wait} current_mode{modes::game_over_wait};

	//references...
	lm::logger&					log;
	const app::env&				env;
	const tools::i8n&			i8n;
	app::hi_score_manager&		hi_scores;
	app::score&					player_score; //not const, resets before exiting.

	//properties
	ldtools::view_composer		layout;
	float						wait_timer{0.f};
	app::enter_name				enter_name;
	bool						with_high_score{false};
};

}
