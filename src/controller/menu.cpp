#include "../../include/controller/menu.h"
#include <app/resources.h>

//local
#include "../../include/input/input.h"
#include <tools/json.h>
#include <tools/file_utils.h>
#include <ldv/ttf_representation.h>
#include <sstream>
#include <iomanip>

using namespace controller;

menu::menu(
	lm::logger& _log,
	const app::env& _env,
	dfw::audio& _audio,
	const lda::resource_manager& _audio_rm,
	const app::resources& _resources,
	const ldtools::ttf_manager& _ttf_manager,
	const tools::i8n& _i8n,
	app::hi_score_manager& _hi_scores
)
	:log(_log),
	env{_env},
	audio{_audio},
	audio_rm{_audio_rm},
	resources{_resources},
	i8n{_i8n},
	hi_scores{_hi_scores}
{

	layout.map_font(
		"main_menu_font", 
		_ttf_manager.get(
			"menu", 
			resources.main_menu_font_size
		)
	);

	const std::string layout_path=_env.build_data_path("layout/layouts.json");
	auto document=tools::parse_json_string(
		tools::dump_file(layout_path)
	);
	layout.parse(document["main_menu"]);

	auto set_text=[this, &_i8n](const std::string& _id, const std::string& _key) {

		static_cast<ldv::ttf_representation*>(layout.get_by_id(_id))->set_text(_i8n.get(_key));
	};

	set_text("text_title", "menu-title");
	set_text("hi_scores_title", "menu-hi_scores");
	set_text("menu_start_game", "menu-start_game");
	set_text("menu_how_to_play", "menu-how_to_play");
	set_text("menu_options", "menu-options");
}

void menu::awake(
	dfw::input& /*input*/
) {

	std::stringstream ss;
	for(const auto& score : hi_scores.get()) {

		ss<<std::setw(6)<<std::setfill('0')<<score.score<<"    "<<score.name<<std::endl;
	}

	static_cast<ldv::ttf_representation*>(layout.get_by_id("hi_scores"))->set_text(ss.str());
	current_selection=0;
	refresh();
}

void menu::loop(
	dfw::input& _input, 
	const dfw::loop_iteration_data& /*lid*/
) {

	if(_input().is_exit_signal() || _input.is_input_down(input::escape)) {
		set_leave(true);
		return;
	}

	//TODO: perhaps this should be fixed values too, like space and cursors???

	if(_input.is_input_down(input::up)) {

		prev();
		return;
	}

	if(_input.is_input_down(input::down)) {

		next();
		return;
	}

	if(_input.is_input_down(input::interact)) {

		switch(current_selection) {

				//TODO: with transition
			case 0:
				set_state(t_states::state_game);
				return;
			case 1:
				//TODO: how to play
			case 2:
				set_state(t_states::state_settings);
				return;
			break;

		}
	}
}

void menu::draw(
	ldv::screen& _screen, 
	int /*fps*/
) {

	layout.draw(_screen);
}

void menu::next() {

	if(current_selection < 2) {

		++current_selection;
		refresh();
		audio.play_sound(audio_rm.get_sound(app::resources::snd_default));
	}
}

void menu::prev() {

	if(0==current_selection) {

		return;
	}

	--current_selection;
	refresh();
	audio.play_sound(audio_rm.get_sound(app::resources::snd_default));
}

void menu::refresh() {

	std::vector<std::string> ids{
		"menu_start_game",
		"menu_how_to_play",
		"menu_options"
	};

	for(const auto& id : ids) {
		static_cast<ldv::ttf_representation*>(layout.get_by_id(id))->set_color(ldv::rgba8(255, 255, 255, 160));
	}

	static_cast<ldv::ttf_representation*>(layout.get_by_id(ids[current_selection]))->set_color(ldv::rgba8(255, 255, 255, 255));

}
