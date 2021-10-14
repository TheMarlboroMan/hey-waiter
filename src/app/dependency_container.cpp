#include <app/dependency_container.h>

#include <app/env.h>
#include <app/resources.h>
#include <app/hi_score.h>
#include <app/score.h>
#include <app/layout.h>
#include <app/draw_sprite.h>

#include <lda/resource_manager.h>
#include <ldv/screen.h>
#include <dfw/audio.h>
#include <dfw/input.h>
#include <dfwimpl/config.h>
#include <lm/logger.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/sprite_table.h>
#include <tools/i8n.h>

#include <fstream>
#include <string>
#include <sstream>

using namespace app;

app::resources& dependency_container::get_resources() {

	if(nullptr==resources.get()) {

		resources.reset(new app::resources{});

		std::ifstream resfile(get_env().build_data_path("fonts.txt"));
		if(!resfile.is_open()) {

			throw std::runtime_error("could not open font files");
		}

		std::string line, title;

		while(true) {

			std::getline(resfile, line);

			if(resfile.eof()) {

				break;
			}

			if(!line.size()) {
			
				continue;
			}

			std::stringstream ss{line};
			ss>>title;

			if(title=="game_hud_ttf") {

				ss>>resources->main_menu_ttf;
			}
			else if(title=="game_hud_font_size") {

				ss>>resources->main_menu_font_size;
			}
			else if(title=="main_menu_ttf") {
				
				ss>>resources->game_hud_ttf;
			}
			else if(title=="settings_ttf") {
				
				ss>>resources->settings_ttf;
			}
			else if(title=="how_to_play_ttf") {
				
				ss>>resources->how_to_play_ttf;
			}
			else if(title=="main_menu_font_size") {

				ss>>resources->game_hud_font_size;
			}
			else if(title=="game_over_ttf") {
				
				ss>>resources->game_over_ttf;
			}
			else if(title=="game_over_font_size") {

				ss>>resources->game_over_font_size;
			}
			else if(title=="settings_font_size") {

				ss>>resources->settings_font_size;
			}
			else if(title=="how_to_play_font_size") {

				ss>>resources->how_to_play_font_size;
			}
			else {

				std::string error{"straneous data in resources file '"};
				error+=title+"'";
				throw std::runtime_error(error);
			}
		}
	}

	return *resources;
}

tools::i8n& dependency_container::get_i8n() {

	if(nullptr==i8n.get()) {

		i8n.reset(new tools::i8n{get_env().build_data_path("i8n"), "en", {"game.txt"}});
	}

	return *i8n;
}

ldtools::sprite_table& dependency_container::get_sprite_table() {

	if(nullptr==sprite_table.get()) {

		sprite_table.reset(new ldtools::sprite_table{get_env().build_data_path("sprites.txt")});
	}

	return *sprite_table;
}

ldtools::ttf_manager& dependency_container::get_ttf_manager() {

	if(nullptr==ttf_manager.get()) {

		ttf_manager.reset(new ldtools::ttf_manager{});

		ttf_manager->insert(
			"menu",
			get_resources().main_menu_font_size,
			get_env().build_data_path(std::string{"fonts/"}+get_resources().main_menu_ttf)
		);

		ttf_manager->insert(
			"hud",
			get_resources().game_hud_font_size,
			get_env().build_data_path(std::string{"fonts/"}+get_resources().game_hud_ttf)
		);

		ttf_manager->insert(
			"game_over",
			get_resources().game_over_font_size,
			get_env().build_data_path(std::string{"fonts/"}+get_resources().game_over_ttf)
		);

		ttf_manager->insert(
			"settings",
			get_resources().settings_font_size,
			get_env().build_data_path(std::string{"fonts/"}+get_resources().settings_ttf)
		);

		ttf_manager->insert(
			"how_to_play",
			get_resources().how_to_play_font_size,
			get_env().build_data_path(std::string{"fonts/"}+get_resources().how_to_play_ttf)
		);
	}

	return *ttf_manager;
}

app::layout& dependency_container::get_layout() {

	if(nullptr==layout.get()) {

		layout.reset(new app::layout{get_env()});
	}

	return *layout;
}

app::hi_score_manager& dependency_container::get_hi_score_manager() {

	if(nullptr==hi_score_manager.get()) {

		hi_score_manager.reset(new app::hi_score_manager{get_env()});
	}

	return *hi_score_manager;
}

app::score& dependency_container::get_score() {

	if(nullptr==score.get()) {

		score.reset(new app::score{});
	}

	return *score;
}

app::draw_sprite& dependency_container::get_draw_sprite() {

	if(nullptr==draw_sprite.get()) {

		draw_sprite.reset(new app::draw_sprite{
			get_sprite_table(),
			get_video_resource_manager()
		});
	}

	return *draw_sprite;
}

