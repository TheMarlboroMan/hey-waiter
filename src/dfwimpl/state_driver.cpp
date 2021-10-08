#include "../../include/dfwimpl/state_driver.h"
#include "../../include/input/input.h"
#include "../../include/controller/states.h"

#include <lm/sentry.h>
#include <tools/string_utils.h>

#include <algorithm>

using namespace dfwimpl;

state_driver::state_driver(
	dfw::kernel& kernel, 
	dfwimpl::config& c,
	const app::env& _env
):
	state_driver_interface(controller::t_states::state_menu),
	config(c), 
	log(kernel.get_log())
{

	dependency_container.set_env(_env);

	lm::log(log, lm::lvl::info)<<"setting state check function..."<<std::endl;

	states.set_function([](int v){
		return v > controller::state_min && v < controller::state_max;
	});

	lm::log(log, lm::lvl::info)<<"init state driver building: preparing video..."<<std::endl;
	prepare_video(kernel);

	lm::log(log, lm::lvl::info)<<"preparing audio..."<<std::endl;
	prepare_audio(kernel);

	lm::log(log, lm::lvl::info)<<"preparing input..."<<std::endl;
	prepare_input(kernel);

	lm::log(log, lm::lvl::info)<<"preparing resources..."<<std::endl;
	prepare_resources(kernel);

	lm::log(log, lm::lvl::info)<<"registering controllers..."<<std::endl;
	register_controllers(kernel);

	lm::log(log, lm::lvl::info)<<"virtualizing input..."<<std::endl;
	virtualize_input(kernel.get_input());

	lm::log(log, lm::lvl::info)<<"state driver fully constructed"<<std::endl;
}

void state_driver::prepare_video(dfw::kernel& kernel) {

	kernel.init_video_system({
		config.int_from_path("video:window_w_px"),
		config.int_from_path("video:window_h_px"),
		500,
		500,
		config.string_from_path("video:window_title"),
		config.bool_from_path("video:window_show_cursor"),
		config.get_screen_vsync()
	});

	auto& screen=kernel.get_screen();
	screen.set_fullscreen(config.bool_from_path("video:fullscreen"));
}

void state_driver::prepare_audio(dfw::kernel& kernel) {

	kernel.init_audio_system({
		config.get_audio_ratio(),
		config.get_audio_out(),
		config.get_audio_buffers(),
		config.get_audio_channels(),
		config.get_audio_volume(),
		config.get_music_volume()
	});
}

void state_driver::prepare_input(dfw::kernel& kernel) {

	using namespace dfw;

	std::vector<input_pair> pairs{
		{{input_description::types::keyboard, SDL_SCANCODE_ESCAPE, 0}, input::escape},
		{input_description_from_config_token(config.token_from_path("input:left")), input::left},
		{input_description_from_config_token(config.token_from_path("input:right")), input::right},
		{input_description_from_config_token(config.token_from_path("input:up")), input::up},
		{input_description_from_config_token(config.token_from_path("input:down")), input::down},
		{input_description_from_config_token(config.token_from_path("input:interact")), input::interact}
	};

	kernel.init_input_system(pairs);
}

void state_driver::prepare_resources(
	dfw::kernel& _kernel
) {

	dependency_container.set_input(_kernel.get_input());
	dependency_container.set_config(config);
	dependency_container.set_audio(_kernel.get_audio());
	dependency_container.set_audio_resource_manager(_kernel.get_audio_resource_manager());
	dependency_container.set_logger(log);
	dependency_container.set_screen(_kernel.get_screen());

	dfw::resource_loader r_loader(
		_kernel.get_video_resource_manager(), 
		_kernel.get_audio_resource_manager(),
		dependency_container.get_env().build_data_path("")
	);

//	r_loader.generate_textures(tools::explode_lines_from_file(std::string("data/resources/textures.txt")));

	r_loader.generate_sounds(tools::explode_lines_from_file(dependency_container.get_env().build_data_path("audio.txt")));
	r_loader.generate_music(tools::explode_lines_from_file(dependency_container.get_env().build_data_path("music.txt")));

	//force loading of stuff now!
	dependency_container.get_i8n();
	dependency_container.get_ttf_manager();
}

void state_driver::register_controllers(
	dfw::kernel& /*_kernel*/
) {

	auto reg=[this](ptr_controller& _ptr, int _i, dfw::controller_interface * _ci) {
		_ptr.reset(_ci);
		register_controller(_i, *_ptr);
	};

	reg(
		c_game, 
		controller::t_states::state_game,
		new controller::game(dependency_container)
	);

	reg(
		c_menu, 
		controller::t_states::state_menu, 
		new controller::menu(dependency_container)
	);

	reg(
		c_game_over, 
		controller::t_states::state_game_over, 
		new controller::game_over(dependency_container)
	);

	reg(
		c_settings, 
		controller::t_states::state_settings, 
		new controller::settings(dependency_container)
	);

	reg(
		c_how_to_play, 
		controller::t_states::state_how_to_play, 
		new controller::how_to_play(dependency_container)
	);
	//[new-controller-mark]
}

void state_driver::prepare_state(int /*next*/, int /*current*/) {

/*
	switch(next) {
		case t_states::state_placeholder:
		break;
		default:
		break;
	}
*/
}

void state_driver::common_pre_loop_input(dfw::input& input, float /*delta*/) {

	if(input().is_event_joystick_connected()) {
		lm::log(log, lm::lvl::info)<<"New joystick detected..."<<std::endl;
		virtualize_input(input);
	}
}

void state_driver::common_loop_input(dfw::input& /*input*/, float /*delta*/) {

}

void state_driver::common_pre_loop_step(float /*delta*/) {

}

void state_driver::common_loop_step(float /*delta*/) {

}

void state_driver::virtualize_input(dfw::input& input) {

	lm::log(log, lm::lvl::info)<<"trying to virtualize "<<input().get_joysticks_size()<<" controllers..."<<std::endl;

	for(size_t i=0; i < input().get_joysticks_size(); ++i) {
		input().virtualize_joystick_hats(i);
		input().virtualize_joystick_axis(i, 15000);
		lm::log(log, lm::lvl::info)<<"Joystick virtualized "<<i<<std::endl;
	}
}
