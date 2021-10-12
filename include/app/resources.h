#pragma once

#include <string>

namespace app {

struct resources {

	std::string	main_menu_ttf,
				game_hud_ttf,
				game_over_ttf,
				settings_ttf,
				how_to_play_ttf;
	int			main_menu_font_size{16},
				game_hud_font_size{16},
				game_over_font_size{16},
				settings_font_size{16},
				how_to_play_font_size{16};

	enum sounds {
		snd_default=1
	};

	enum music {
		mus_default=1
	};

	enum textures {
		tex_background=1,
		tex_sprites=2
	};

	enum sprites {
		spr_bar=1,
		spr_table=2,
		spr_trash=3,
		spr_bar_outline=4,
		spr_table_outline=5,
		spr_trash_outline=6
	};
};

}
