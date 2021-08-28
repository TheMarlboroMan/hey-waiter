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
	const app::resources& _resources,
	const ldtools::ttf_manager& _ttf_manager,
	const tools::i8n& _i8n,
	app::hi_score_manager& _hi_scores
)
	:log(_log),
	env{_env},
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

	const std::string layout_path=_env.build_data_path("layout/main_menu.json");
	auto document=tools::parse_json_string(
		tools::dump_file(layout_path)
	);
	layout.parse(document["main_menu"]);

	auto set_text=[this, &_i8n](const std::string& _id, const std::string& _key) {

		static_cast<ldv::ttf_representation*>(layout.get_by_id(_id))->set_text(_i8n.get(_key));
	};

	set_text("text_title", "menu-title");
	set_text("hi_scores_title", "menu-hi_scores");
}

void menu::awake(
	dfw::input& /*input*/
) {

	std::stringstream ss;
	for(const auto& score : hi_scores.get()) {

		ss<<std::setw(6)<<std::setfill('0')<<score.score<<"    "<<score.name<<std::endl;
	}

	static_cast<ldv::ttf_representation*>(layout.get_by_id("hi_scores"))->set_text(ss.str());

}

void menu::loop(
	dfw::input& _input, 
	const dfw::loop_iteration_data& /*lid*/
) {

	if(_input().is_exit_signal() || _input.is_input_down(input::escape)) {
		set_leave(true);
		return;
	}

	//TODO: perhaps this should be fixed values too, like space?
	if(_input.is_input_down(input::interact)) {

		set_state(t_states::state_game);
		return;
	}

}

void menu::draw(
	ldv::screen& _screen, 
	int /*fps*/
) {

	layout.draw(_screen);
}
