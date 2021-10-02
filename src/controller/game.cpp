#include "../../include/controller/game.h"
#include "../../include/input/input.h"
#include <app/env.h>
#include <app/input.h>

using namespace controller;

game::game(
	lm::logger& plog,
	const app::env& _env,
	dfw::audio& _audio,
	const lda::resource_manager& _audio_rm,
	const app::resources& _resources,
	const ldtools::ttf_manager& _ttf_manager,
	const tools::i8n& _i8n,
	app::score& _player_score
):
	log(plog),
	env{_env},
	camera{ {0,0,500,500},{0,0} },
	game_instance{plog, _audio, _audio_rm, _player_score},
	draw_instance{_resources, _ttf_manager, _i8n}
{

	camera.set_coordinate_system(ldv::camera::tsystem::cartesian);
	game_instance.init(env.build_data_path("world.dat"));
}

void game::loop(
	dfw::input& _input, 
	const dfw::loop_iteration_data& _lid
) {

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	if( _input.is_input_down(input::escape)) {

		game_instance.reset();
		set_state(t_states::state_menu);
	}

	app::input i;

	if(_input.is_input_pressed(input::down)) {

		i.walk_down=true;
	}
	else if(_input.is_input_pressed(input::up)) {

		i.walk_up=true;
	}

	if(_input.is_input_pressed(input::left)) {

		i.walk_left=true;
	}
	else if(_input.is_input_pressed(input::right)) {

		i.walk_right=true;
	}

	if(_input.is_input_down(input::down)) {

		i.select_down=true;
	}
	else if(_input.is_input_down(input::up)) {

		i.select_up=true;
	}

	if(_input.is_input_down(input::left)) {

		i.select_left=true;
	}
	else if(_input.is_input_down(input::right)) {

		i.select_right=true;
	}

	if(_input.is_input_down(input::interact)) {

		i.interact=true;
	}

	game_instance.set_input(i);
	game_instance.tick(_lid.delta);

	if(game_instance.is_game_over()) {

		game_instance.reset();
		set_state(t_states::state_game_over);
		return;
	}
}

void game::draw(
	ldv::screen& _screen, 
	int /*fps*/
) {

	camera.set_coordinate_system(ldv::camera::tsystem::cartesian);

	const auto& cbox=game_instance.get_world().get_collision_box();
	ldv::rect world_box(cbox.origin.x, cbox.origin.y, cbox.w, cbox.h);
	camera.center_on(world_box);

//	const auto& fc=camera.get_focus_box().origin;
//	std::cout<<fc.x<<", "<<fc.y<<std::endl;

	draw_instance.do_draw(_screen, camera, game_instance);
}

