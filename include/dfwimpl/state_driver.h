#pragma once

#include "config.h"
#include <app/env.h>

#include <dfw/state_driver_interface.h>
#include <ldtools/ttf_manager.h>
#include <tools/i8n.h>

#include <memory>

//Controllers.
#include "../../include/controller/game.h"
//[new-controller-header-mark]

//Specific app_config
//#include "../app/placeholder.h"

namespace dfwimpl {

class state_driver:
	public dfw::state_driver_interface {

	public:
							state_driver(
								dfw::kernel& kernel, 
								dfwimpl::config& config,
								const app::env&
								);

	virtual void					common_pre_loop_input(dfw::input& input, float delta);
	virtual void					common_pre_loop_step(float delta);
	virtual void					common_loop_input(dfw::input& input, float delta);
	virtual void					common_loop_step(float delta);
	virtual void					prepare_state(int, int);
	virtual float					get_max_timestep() const {return 0.03f;}

	private:

	void						prepare_video(dfw::kernel&);
	void						prepare_audio(dfw::kernel&);
	void						prepare_input(dfw::kernel&);
	void						prepare_resources(dfw::kernel&);
	void						register_controllers(dfw::kernel&);
	void 						virtualize_input(dfw::input& input);

	//references
	dfwimpl::config&				config;
	lm::logger&					log;
	const app::env&				env;
	
	//properties...
	ldtools::ttf_manager		ttf_manager;
	tools::i8n					i8n;

	typedef std::unique_ptr<dfw::controller_interface>	ptr_controller;
	ptr_controller					c_game;
	//[new-controller-property-mark]
};

}
