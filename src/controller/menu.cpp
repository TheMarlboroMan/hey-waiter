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
	app::dependency_container& _dependency_container
)
	:log(_dependency_container.get_log()),
	env{_dependency_container.get_env()},
	audio{_dependency_container.get_audio()},
	audio_rm{_dependency_container.get_audio_resource_manager()},
	resources{_dependency_container.get_resources()},
	i8n{_dependency_container.get_i8n()},
	hi_scores{_dependency_container.get_hi_score_manager()}
{

	layout.map_font(
		"main_menu_font", 
		_dependency_container.get_ttf_manager().get(
			"menu", 
			resources.main_menu_font_size
		)
	);

	const std::string layout_path=env.build_data_path("layout/layouts.json");
	auto document=tools::parse_json_string(
		tools::dump_file(layout_path)
	);
	layout.parse(document["main_menu"]);

	auto set_text=[this](const std::string& _id, const std::string& _key) {

		static_cast<ldv::ttf_representation*>(layout.get_by_id(_id))->set_text(i8n.get(_key));
	};

	set_text("text_title", "menu-title");
	set_text("hi_scores_title", "menu-hi_scores");
	set_text("menu_start_game", "menu-start_game");
	set_text("menu_how_to_play", "menu-how_to_play");
	set_text("menu_options", "menu-options");

	audio.play_music(audio_rm.get_music(app::resources::mus_default));
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
				set_state(t_states::state_how_to_play);
				return;
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
