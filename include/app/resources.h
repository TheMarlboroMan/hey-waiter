#pragma once

#include <string>

namespace app {

struct resources {

	std::string	main_menu_ttf,
				game_hud_ttf,
				game_over_ttf;
	int			main_menu_font_size{16},
				game_hud_font_size{16},
				game_over_font_size{16};
};

}
