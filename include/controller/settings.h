#pragma once

#include "states.h"
#include "app/env.h"
#include <app/resources.h>
#include "dfwimpl/config.h"

#include <lda/resource_manager.h>
#include <dfw/controller_interface.h>
#include <dfw/audio.h>
#include <lm/logger.h>

#include <tools/i8n.h>
#include <tools/options_menu.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/view_composer.h>

#include <string>

namespace controller {

class settings:
	public dfw::controller_interface {

	public:

	                            settings(
									lm::logger&,
									const app::env&,
									dfw::audio&,
									const lda::resource_manager&,
									const app::resources&,
									const tools::i8n&,
									dfw::input&,
									dfwimpl::config&,
									const ldtools::ttf_manager& _ttf_manager,
									const ldv::rect&
	                            );

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	private:

	void                        loop_navigate(dfw::input&, const dfw::loop_iteration_data&);
	void                        loop_learn(dfw::input&, const dfw::loop_iteration_data&);
	void                        prev();
	void                        next();
	void                        left(bool, float);
	void                        right(bool, float);
	void                        select();
	void                        sync_view();
	void                        sync_selection();
	void                        start_learning_mode(const std::string&);
	void                        end_learning_mode();
	int                         volume_internal_to_config(int) const;
	int                         volume_config_to_internal(int) const;
	std::string                 int_to_config_name(int) const;
	int                         input_key_to_int(const std::string) const;

	//references
	lm::logger&					log;
	const app::env&				env;
	dfw::audio&					audio;
	const lda::resource_manager& audio_rm;
	const app::resources&		resources;
	const tools::i8n&			i8n;
	dfw::input&					input;
	dfwimpl::config&			config;

	//properties
	ldtools::view_composer      layout;
	tools::options_menu<std::string> menu;
	std::string                 current_selection;
	std::vector<std::string>    name_keys,
	                            value_keys;

	float                       press_timer{0.f};
	bool                        learning_mode{false},
	                            learning_input_entered{false};
	dfw::input_description      learned_input;
	int                         learning_input_id{0};
};

}
