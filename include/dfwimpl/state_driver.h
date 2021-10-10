#pragma once

#include "config.h"

#include <app/dependency_container.h>
#include <dfw/state_driver_interface.h>
#include <ldtools/ttf_manager.h>
#include <tools/i8n.h>

#include <memory>

//pimpl, incomplete types and unique_ptr...
#include <app/layout.h>

//Controllers.
#include "../../include/controller/game.h"
#include "../../include/controller/menu.h"
#include "../../include/controller/game_over.h"
#include "../../include/controller/settings.h"
#include "../../include/controller/how_to_play.h"
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
	dfwimpl::config&			config;
	lm::logger&					log;
	app::dependency_container	dependency_container;
	
	//properties...

	typedef std::unique_ptr<dfw::controller_interface>	ptr_controller;
	ptr_controller					c_game;
	ptr_controller					c_menu;
	ptr_controller					c_game_over;
	ptr_controller					c_settings;
	ptr_controller					c_how_to_play;
	//[new-controller-property-mark]
};

}
