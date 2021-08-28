#include "../../include/controller/game_over.h"
#include "../../include/input/input.h"

#include <tools/file_utils.h>
#include <tools/json.h>

using namespace controller;

game_over::game_over(
	lm::logger& _log,
	const app::env& _env,
	const app::resources& _resources,
	const ldtools::ttf_manager& _ttf_manager,
	const tools::i8n& _i8n,
	app::hi_score_manager& _hi_scores,
	const app::score& _player_score
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

//	auto set_text=[this, &_i8n](const std::string& _id, const std::string& _key) {

//		static_cast<ldv::ttf_representation*>(layout.get_by_id(_id))->set_text(_i8n.get(_key));
//	};

//	set_text("text_title", "menu-title");
//	set_text("hi_scores_title", "menu-hi_scores");

}

void game_over::awake(
	dfw::input& /*input*/
) {

	wait_timer=5.f;
	current_mode=hi_scores.can_be_submitted(player_score.get())
		? modes::hi_score_input
		: modes::game_over_wait;

	//TODO: set input to text mode if need be!!
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

			//TODO: this is not so.
			if(_input.is_input_down(input::escape)) {

				current_mode=modes::game_over_wait;
			}

		break;
		case modes::game_over_wait:
			wait_timer-=_lid.delta;

			if(_input.is_input_down(input::escape)) {

				set_state(t_states::state_game);
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

	layout.draw(_screen);
}
