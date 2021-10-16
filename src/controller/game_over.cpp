#include "../../include/controller/game_over.h"
#include "../../include/input/input.h"
#include <app/input.h>
#include <app/layout.h>

#include <ldv/ttf_representation.h>

#include <iostream>

using namespace controller;

game_over::game_over(app::dependency_container& _dc)
	:log(_dc.get_log()),
	env{_dc.get_env()},
	i8n{_dc.get_i8n()},
	hi_scores{_dc.get_hi_score_manager()},
	player_score{_dc.get_score()}
{

	layout.map_font(
		"game_over_font", 
		_dc.get_ttf_manager().get(
			"game_over",
			_dc.get_resources().game_over_font_size
		)
	);
	_dc.get_layout().parse_into("game_over", layout);

	auto set_text=[this](const std::string& _id, const std::string& _key) {

		static_cast<ldv::ttf_representation*>(layout.get_by_id(_id))->set_text(i8n.get(_key));
	};

	set_text("text_title", "game_over-title");
	set_text("got_hi_score_title", "game_over-hi_score");
	set_text("your_score_title", "game_over-your_score");
	set_text("bad_game", "game_over-bad_game");

	static_cast<ldv::ttf_representation*>(layout.get_by_id("enter_hi_score"))->set_text(enter_name.get_player_name());
}

void game_over::awake(
	dfw::input& /*input*/
) {

	static_cast<ldv::ttf_representation*>(layout.get_by_id("your_score_value"))->set_text(std::to_string(player_score.get()));
	with_high_score=hi_scores.can_be_submitted(player_score.get());

	current_mode=with_high_score
		? modes::hi_score_input
		: modes::game_over_wait;

	if(with_high_score) {

		enter_name.reset();
	}
	else {

		wait_timer=5.f;
	}

	layout.get_by_id("bad_game")->set_visible(!player_score.get());
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

			if(_input().is_event_input() && wait_timer < 0.f) {

				set_state(t_states::state_menu);

				//TODO: hmmm... this must not be reset... right?
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
		wait_timer=5.f;
		hi_scores.submit({player_score.get(), enter_name.get_player_name()});
		current_mode=modes::game_over_wait;
		layout.get_by_id("caret")->set_visible(false);
	}
}

void game_over::set_caret() {

	std::string val{"          "};
	val[enter_name.get_current_position()]='_';
	static_cast<ldv::ttf_representation*>(layout.get_by_id("caret"))->set_text(val);
}
