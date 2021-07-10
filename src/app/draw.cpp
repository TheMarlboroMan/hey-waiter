#include <app/draw.h>

#include <ldv/box_representation.h>
#include <ldv/ttf_representation.h>
#include <ldv/color.h>


#include <iostream>
#include <sstream>
#include <map>

using namespace app;

draw::draw(
	const ldtools::ttf_manager& _ttf_manager,
	const tools::i8n& _i8n
):
	ttf_manager{_ttf_manager},
	i8n{_i8n}
{

}

ldv::rect draw::to_video(
	const box& _box
) const {

	auto rect=ldv::rect(_box.origin.x, -(_box.origin.y+_box.h), _box.w, _box.h);

//	std::cout<<"<<"<<_box.origin.x<<", "<<_box.origin.y<<" "<<_box.w<<"x"<<_box.h<<std::endl;
//	std::cout<<">>"<<rect.origin.x<<", "<<rect.origin.y<<" "<<rect.w<<"x"<<rect.h<<std::endl;

	return rect;
}

void draw::do_draw(

	ldv::screen& _screen,
	const ldv::camera& _camera,
	const app::game& _game
) {

	_screen.clear(ldv::rgba8(0,0,255,255));

	const auto wbox=_game.world_instance.get_collision_box();
	
	ldv::box_representation bg_box(
		to_video(wbox),
		ldv::rgba8(32, 32, 32, 255)
	);

	bg_box.draw(_screen, _camera);

	for(const auto& table : _game.tables) {

		ldv::box_representation interaction_box(
			to_video(table.get_interaction_box()),
			ldv::rgba8(128, 255, 32, 64)
		);

		ldv::box_representation collision_box(
			to_video(table.get_collision_box()),
			ldv::rgba8(255, 32, 32, 255)
		);

		interaction_box.draw(_screen, _camera);
		collision_box.draw(_screen, _camera);
	}

	ldv::box_representation bar_box(
		to_video(_game.bar_instance.get_collision_box()),
		ldv::rgba8(0, 255, 255, 255)
	);
	bar_box.draw(_screen, _camera);

	ldv::box_representation trash_box(
		to_video(_game.trash_instance.get_collision_box()),
		ldv::rgba8(128, 128, 128, 255)
	);
	trash_box.draw(_screen, _camera);

	auto player_color=ldv::rgba8(255, 255, 255, 255);
	if(_game.player_tray.has_trash()) {

		player_color=ldv::rgba8(255, 0, 0, 255);
	}
	else if(!_game.player_tray.is_empty()) {

		player_color=ldv::rgba8(0, 255, 0, 255);
	}

	ldv::box_representation player_box(
		to_video(_game.player_instance.get_collision_box()),
		player_color
	);
	player_box.draw(_screen, _camera);

	switch(_game.current_mode) {

		case game::modes::movement:

			draw_interactions(_screen, _game);
		break;
		case game::modes::fill_tray:

			draw_fill_tray(_screen, _game);
			//TODO: draw selection and tray and shit.
		break;
		case game::modes::serve:

			//TODO:
		break;
	}
}

void draw::draw_interactions(
	ldv::screen& _screen,
	const app::game& _game
) {

	if(_game.current_interaction_type==game::interaction_types::none) {

		return;
	}

	ldv::ttf_representation txt{
		ttf_manager.get("main", 16),
		ldv::rgba8(255, 255, 255, 255),
		"..."
	};

	switch(_game.current_interaction_type) {

		case game::interaction_types::none:
			return;
		break;
		case game::interaction_types::pick_consumables:
			txt.set_text(i8n.get("game-pick_consumables"));
		break;
		case game::interaction_types::empty_tray:
			txt.set_text(i8n.get("game-empty_tray"));
		break;
		case game::interaction_types::take_order:
			txt.set_text(i8n.get("game-take_order"));
		break;
		case game::interaction_types::serve_order:
			txt.set_text(i8n.get("game-serve_order"));
		break;
		case game::interaction_types::clean_table:
			txt.set_text(i8n.get("game-clean_table"));
		break;
		case game::interaction_types::drop_trash:
			txt.set_text(i8n.get("game-drop_trash"));
		break;
	}

	txt.draw(_screen);
}

void draw::draw_fill_tray(
	ldv::screen& _screen,
	const app::game& _game
) {

	//draw bar selector...
	std::map<consumable::types, std::string> bar{
{consumable::types::water_bottle, "water"},
{consumable::types::soda_bottle, "soda"},
{consumable::types::beer_bottle, "beer"},
{consumable::types::beer_jar, "beer jar"},
{consumable::types::cocktail, "cocktail"},
{consumable::types::pinneaple, "pineapple"},
{consumable::types::melon, "melon"},
{consumable::types::watermelon, "watermelon"},
	};

	std::stringstream ss;
	for(const auto& pair : bar) {

		if(pair.first==_game.bar_selector_instance.get()) {

			ss<<">>";
		}

		ss<<pair.second<<std::endl;
	}

	ss<<std::endl<<"------------------"<<std::endl;

	//draw current tray contents...
	ss<<_game.player_tray.size()<<" / 8"<<std::endl;
	for(const auto& consumable : _game.player_tray.get()) {

		switch(consumable.type) {

			case consumable::types::water_bottle: ss<<"water, "; break;
			case consumable::types::soda_bottle: ss<<"soda, "; break;
			case consumable::types::beer_bottle: ss<<"beer, "; break;
			case consumable::types::beer_jar: ss<<"beer jar, "; break;
			case consumable::types::cocktail: ss<<"cocktail, "; break;
			case consumable::types::pinneaple: ss<<"pineapple, "; break;
			case consumable::types::melon: ss<<"melon, "; break;
			case consumable::types::watermelon: ss<<"watermelon, "; break;
		}
	}

	ldv::ttf_representation txt{
		ttf_manager.get("main", 16),
		ldv::rgba8(255, 255, 255, 255),
		ss.str()
	};

	txt.draw(_screen);
}
