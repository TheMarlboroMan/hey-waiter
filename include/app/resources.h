#pragma once

#include <string>

namespace app {

struct resources {

	std::string	main_menu_ttf,
				game_hud_ttf,
				game_over_ttf,
				settings_ttf,
				how_to_play_ttf,
				consumable_selector_ttf;
	int			main_menu_font_size{16},
				game_hud_font_size{16},
				game_over_font_size{16},
				settings_font_size{16},
				how_to_play_font_size{16},
				consumable_selector_font_size{16};

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
		spr_trash_outline=6,
		spr_hud_bar_top_left=7,
		spr_hud_bar_top_right=8,
		spr_hud_bar_bottom_right=9,
		spr_hud_bar_bottom_left=10,
		spr_hud_consumable_water=11,
		spr_hud_consumable_soda=12,
		spr_hud_consumable_beer_bottle=13,
		spr_hud_consumable_beer_jar=14,
		spr_hud_consumable_cocktail=15,
		spr_hud_consumable_pineapple=16,
		spr_hud_consumable_melon=17,
		spr_hud_consumable_watermelon=18,
		spr_hud_marker_red=19,
		spr_hud_marker_gray=20,
		spr_player_south=21,
		spr_player_north=22,
		spr_player_east=23,
		spr_player_west=24,
	};
};

}
