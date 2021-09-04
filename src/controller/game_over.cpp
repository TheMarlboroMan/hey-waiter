#include "../../include/controller/game_over.h"
#include "../../include/input/input.h"
#include <app/input.h>

#include <tools/file_utils.h>
#include <tools/json.h>
#include <ldv/ttf_representation.h>

#include <iostream>

using namespace controller;

game_over::game_over(
	lm::logger& _log,
	const app::env& _env,
	const app::resources& _resources,
	const ldtools::ttf_manager& _ttf_manager,
	const tools::i8n& _i8n,
	app::hi_score_manager& _hi_scores,
	app::score& _player_score
)
	:log(_log),
	env{_env},
	resources{_resources},
	i8n{_i8n},
	hi_scores{_hi_scores},
	player_score{_player_score}
{

	layout.map_font(
		"game_over_font", 
		_ttf_manager.get(
			"game_over", 
			resources.game_over_font_size
		)
	);

	const std::string layout_path=_env.build_data_path("layout/layouts.json");
	auto document=tools::parse_json_string(
		tools::dump_file(layout_path)
	);
	layout.parse(document["game_over"]);

	auto set_text=[this, &_i8n](const std::string& _id, const std::string& _key) {

		static_cast<ldv::ttf_representation*>(layout.get_by_id(_id))->set_text(_i8n.get(_key));
	};

	set_text("text_title", "game_over-title");
	set_text("got_hi_score_title", "game_over-hi_score");
	set_text("your_score_title", "game_over-your_score");

	static_cast<ldv::ttf_representation*>(layout.get_by_id("enter_hi_score"))->set_text(enter_name.get_player_name());
	static_cast<ldv::ttf_representation*>(layout.get_by_id("your_score_value"))->set_text(std::to_string(player_score.get()));
}

void game_over::awake(
	dfw::input& /*input*/
) {

	wait_timer=5.f;

	with_high_score=hi_scores.can_be_submitted(player_score.get());

	current_mode=with_high_score
		? modes::hi_score_input
		: modes::game_over_wait;

	if(with_high_score) {

		enter_name.reset();
	}

	layout.get_by_id("got_hi_score_title")->set_visible(with_high_score);
	layout.get_by_id("enter_hi_score")->set_visible(with_high_score);
	layout.get_by_id("caret")->set_visible(with_high_score);
	set_caret();
}

void game_over::loop(
	dfw::input& _input, 
	const dfw::loop_iteration_data& _lid
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	switch(current_mode) {

		case modes::hi_score_input:
	
			hi_score_input(_input, _lid);

		break;
		case modes::game_over_wait:

			wait_timer-=_lid.delta;

			//TODO: actually, any input will do.
			if(_input.is_input_down(input::interact) && wait_timer < 0.f) {

//TODO: caret must not show.

				set_state(t_states::state_menu);
				player_score.reset();
				return;
			}
		break;
	}
}

void game_over::request_draw(dfw::controller_view_manager& cvm) {

	cvm.add(state_game);
	cvm.add_ptr(this);
}

void game_over::draw(
	ldv::screen& _screen, 
	int /*fps*/
) {

	if(with_high_score) {

		static_cast<ldv::ttf_representation*>(layout.get_by_id("enter_hi_score"))->set_text(enter_name.get_player_name());
	}

	layout.draw(_screen);
}

void game_over::hi_score_input(
	dfw::input& _input, 
	const dfw::loop_iteration_data& _lid
) {

	//TODO: The caret must flicker.

	app::input i;

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

	//TODO: allow cancelling.

	enter_name.set_input(i);
	enter_name.tick(_lid.delta);
	set_caret();

	if(enter_name.is_finished()) {

		//submit hi score...
		hi_scores.submit({player_score.get(), enter_name.get_player_name()});
		current_mode=modes::game_over_wait;
	}
}

void game_over::set_caret() {

	std::string val{"          "};
	val[enter_name.get_current_position()]='_';
	static_cast<ldv::ttf_representation*>(layout.get_by_id("caret"))->set_text(val);
}
