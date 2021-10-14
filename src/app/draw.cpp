#include <app/draw.h>

#include <ldv/box_representation.h>
#include <ldv/ttf_representation.h>
#include <ldv/bitmap_representation.h>
#include <ldv/color.h>

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <map>

using namespace app;

draw::draw(
	const app::resources& _resources,
	const ldv::resource_manager& _vrm,
	const ldtools::ttf_manager& _ttf_manager,
	const ldtools::sprite_table& _sprite_table,
	const tools::i8n& _i8n
):
	resources{_resources},
	video_resource_manager{_vrm},
	ttf_manager{_ttf_manager},
	sprite_table{_sprite_table},
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

	std::sort(
		std::begin(sortable_components),
		std::end(sortable_components),
		[](
			const std::shared_ptr<draw_component> _a, 
			const std::shared_ptr<draw_component> _b
		) {

			return _a->origin().y <= _b->origin().y;
		}
	);

	_screen.clear(ldv::rgba8(0,0,0,255));
	//TODO: come on, just pass the rect.
	draw_background(_screen, _camera, _game);
	draw_score(_screen, _game.player_score);
	draw_timer(_screen, _game);

	for(const auto& dr : sortable_components) {

		dr->draw(_screen, _camera);
	}


	for(const auto& table : _game.tables) {

		draw_table(_screen, _camera, table);
	}

	if(debug) {

		for(const auto& obstacle : _game.obstacles) {

			draw_obstacle(_screen, _camera, obstacle);
		}
	}

	draw_bar(_screen, _camera, _game.bar_instance);
	draw_trash(_screen, _camera, _game.trash_instance);

	//If the game is over, we just want the world drawn.
	if(_game.is_game_over()) {

		return;
	}

	switch(_game.current_mode) {

		case game::modes::movement:

			draw_interactions(_screen, _game);
		break;
		case game::modes::fill_tray:

			draw_fill_tray(_screen, _game);
		break;
		case game::modes::serve:

			draw_serve(_screen, _game);
		break;
		case game::modes::take_order:

			draw_take_order(_screen, _game);
		break;
	}

	if(debug) {

		draw_level_number(_screen, _game);
	}
}

void draw::draw_obstacle(
	ldv::screen& _screen, 
	const ldv::camera& _camera, 
	const obstacle& _obstacle
) {

	auto color=ldv::rgba8(0, 0, 0, 32);

	ldv::box_representation collision_box(
		to_video(_obstacle.get_collision_box()),
		color
	);

	collision_box.set_blend(ldv::representation::blends::alpha);
	collision_box.draw(_screen, _camera);
}

void draw::draw_table(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	const table& _table
) {
	
	//TODO: lots of repetition...
	const auto sprite_box=sprite_table.get(app::resources::spr_table).box;
	auto origin=to_sprite_point(_table.get_collision_box(), sprite_box);

	ldv::bitmap_representation bmp(
		video_resource_manager.get_texture(app::resources::tex_sprites),
		{origin, sprite_box.w, sprite_box.h},
		sprite_box
	);

	bmp.set_blend(ldv::representation::blends::alpha);
	bmp.draw(_screen, _camera);

	if(debug) {

		auto table_color=ldv::rgba8(64, 64, 64, 128);

		if(_table.is_demanding_attention()) {
			//demanding table: blue,
			table_color=ldv::rgba8(0, 0, 192, 128);
		}
		else if(_table.is_waiting_order()) {
			//waiting table: yellow
			table_color=ldv::rgba8(192, 192, 0, 128);
		}
		else if(_table.is_dirty()) {
			//dirty table: red
			table_color=ldv::rgba8(192, 0, 0, 128);
		}
		else if(_table.is_consuming()) {
			//eating table: green
			table_color=ldv::rgba8(0, 192, 0, 128);
		}
		else if(_table.is_customer_arriving()) {

			//a customer is coming: white
			table_color=ldv::rgba8(255, 255, 255, 128);
		}
		else if(_table.is_customer_leaving()) {

			//a customer is coming: black
			table_color=ldv::rgba8(0, 0, 0, 128);
		}

		ldv::box_representation collision_box(
			to_video(_table.get_collision_box()),
			table_color
		);

		collision_box.set_blend(ldv::representation::blends::alpha);
		collision_box.draw(_screen, _camera);
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
		ttf_manager.get("hud", resources.game_hud_font_size),
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

			ss<<" [ "<<pair.second<<" ]\n";
		}
		else {

			ss<<pair.second<<"\n";
		}
	}
	
	ss<<"\n\n"<<"------------------"<<std::endl;

	//draw current tray contents...
	ss<<_game.player_tray.size()<<" / 8"<<std::endl;
	for(const auto& consumable : _game.player_tray.get()) {

		ss<<consumable_to_string(consumable)<<", ";
	}

	ldv::ttf_representation txt{
		ttf_manager.get("hud", resources.game_hud_font_size),
		ldv::rgba8(255, 255, 255, 255),
		ss.str()
	};

	txt.draw(_screen);
}

void draw::draw_serve(
	ldv::screen& _screen,
	const app::game& _game
) {

	std::stringstream ss;
	
	std::size_t index=0;

	ss<<"-- IN TRAY --\n";
	for(const auto& carried : _game.player_tray.get()) {
	
		if(index++==_game.player_tray.get_current_index()) {

			ss<<" [ "<<consumable_to_string(carried)<<" ] ";
		}
		else {

			ss<<consumable_to_string(carried)<<" ";
		}
	}

	ss<<"\n\n-- IN TABLE --"<<std::endl;
	for(const auto& served : _game.table_serving.get()) {

		ss<<consumable_to_string(served)<<" ";
	}

	ss<<"\n";

	ldv::ttf_representation txt{
		ttf_manager.get("hud", resources.game_hud_font_size),
		ldv::rgba8(255, 255, 255, 255),
		ss.str()
	};

	txt.draw(_screen);
}

void draw::draw_take_order(
	ldv::screen& _screen, 
	const app::game& _game
) {

	const auto& order=_game.current_table->get_current_order();

	std::stringstream ss;

	ss<<"just get me ";
	for(const auto& consumable : order.products) {

		ss<<consumable_to_string(consumable)<<", ";
	}

	ldv::ttf_representation txt{
		ttf_manager.get("hud", resources.game_hud_font_size),
		ldv::rgba8(255, 255, 255, 255),
		ss.str()
	};

	txt.draw(_screen);
}

std::string draw::consumable_to_string(
	const app::consumable& _consumable
) const {

	switch(_consumable.type) {

		case consumable::types::water_bottle: return "water";;
		case consumable::types::soda_bottle: return "soda";;
		case consumable::types::beer_bottle: return "beer";;
		case consumable::types::beer_jar: return "beer jar";;
		case consumable::types::cocktail: return "cocktail";;
		case consumable::types::pinneaple: return "pineapple";
		case consumable::types::melon: return "melon";
		case consumable::types::watermelon: return "watermelon";
	}

	return "???";
}

void draw::draw_score(
	ldv::screen& _screen, 
	const app::score& _score
) {

	std::stringstream ss;
	ss<<std::setw(8)<<std::setfill('0')<<std::to_string(_score.get());

	ldv::ttf_representation txt{
		ttf_manager.get("hud", resources.game_hud_font_size),
		ldv::rgba8(255, 255, 255, 255),
		ss.str()
	};

	//TODO: it would be great if there was a layout for this and others,
	//so we can do this shit better.
	txt.go_to({278, 17});
	txt.draw(_screen);
}

void draw::draw_timer(
	ldv::screen& _screen, 
	const app::game& _game
) {

	int timer=_game.game_seconds - std::floor(_game.current_game_seconds);
	
	std::stringstream ss;
	ss<<std::setw(3)<<std::setfill('0')<<std::to_string(timer);

	ldv::ttf_representation txt{
		ttf_manager.get("hud", resources.game_hud_font_size),
		ldv::rgba8(255, 255, 255, 255),
		ss.str()
	};

	txt.go_to({160, 52});
	txt.draw(_screen);
}

void draw::draw_level_number(
	ldv::screen& _screen, 
	const app::game& _game
) {

	//TODO: only in debug mode!
	

	std::stringstream ss;
	ss<<"lvl "<<(_game.current_stage+1);

	ldv::ttf_representation txt{
		ttf_manager.get("hud", resources.game_hud_font_size),
		ldv::rgba8(255, 255, 255, 255),
		ss.str()
	};

	txt.go_to({30, 14});
	txt.draw(_screen);
}

void draw::draw_background(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	const app::game& _game
) {

	const auto wbox=_game.world_instance.get_collision_box();
	ldv::bitmap_representation bg(
		video_resource_manager.get_texture(app::resources::tex_background),
		to_video(wbox),
		{0,0, (unsigned)wbox.w, (unsigned)wbox.h}
	);

	bg.set_blend(ldv::representation::blends::alpha);
	bg.draw(_screen, _camera);
}

void draw::draw_bar(
	ldv::screen& _screen,
	const ldv::camera& _camera,
	const app::bar& _bar
) {

	//TODO: A shitload of repetition...
	const auto sprite_box=sprite_table.get(app::resources::spr_bar).box;
	auto origin=to_sprite_point(_bar.get_collision_box(), sprite_box);

	ldv::bitmap_representation bmp(
		video_resource_manager.get_texture(app::resources::tex_sprites),
		{origin, sprite_box.w, sprite_box.h},
		sprite_box
	);

	bmp.set_blend(ldv::representation::blends::alpha);
	bmp.draw(_screen, _camera);

	if(debug) {

		ldv::box_representation bar_box(
			to_video(_bar.get_interaction_box()),
			ldv::rgba8(255, 0, 0, 128)
		);
		bar_box.set_blend(ldv::representation::blends::alpha);
		bar_box.draw(_screen, _camera);

		ldv::box_representation collision_box(
			to_video(_bar.get_collision_box()),
			ldv::rgba8(0, 0, 0, 128)
		);
		collision_box.set_blend(ldv::representation::blends::alpha);
		collision_box.draw(_screen, _camera);
	}
}

void draw::draw_trash(
	ldv::screen& _screen, 
	const ldv::camera& _camera, 
	const trash& _trash
) {

	//TODO: lots of repetition...
	const auto sprite_box=sprite_table.get(app::resources::spr_trash).box;
	auto origin=to_sprite_point(_trash.get_collision_box(), sprite_box);

	ldv::bitmap_representation bmp(
		video_resource_manager.get_texture(app::resources::tex_sprites),
		{origin, sprite_box.w, sprite_box.h},
		sprite_box
	);

	bmp.set_blend(ldv::representation::blends::alpha);
	bmp.draw(_screen, _camera);

	if(debug) {

		ldv::box_representation trash_box(
			to_video(_trash.get_collision_box()),
			ldv::rgba8(128, 0, 0, 128)
		);
		trash_box.set_blend(ldv::representation::blends::alpha);
		trash_box.draw(_screen, _camera);

		ldv::box_representation collision_box(
			to_video(_trash.get_collision_box()),
			ldv::rgba8(0, 0, 0, 128)
		);
		collision_box.set_blend(ldv::representation::blends::alpha);
		collision_box.draw(_screen, _camera);

	}
}

ldv::point draw::to_sprite_point(
	const box& _position_box,
	const ldv::rect& _sprite_box
) const {

	int x=_position_box.origin.x,
		y=-(_position_box.origin.y+_sprite_box.h);

	return {x, y};
}

void draw::populate(
	const app::game& _game
) {

	sortable_components.clear();

	auto player=new app::draw_player(_game.player_instance, _game.player_tray);
	sortable_components.push_back(
		std::shared_ptr<draw_component>(player)
	);

	auto bar=new app::draw_bar{_game.bar_instance};
	sortable_components.push_back(
		std::shared_ptr<draw_component>(bar)
	);

	auto trash=new app::draw_trash{_game.trash_instance};
	sortable_components.push_back(
		std::shared_ptr<draw_component>(trash)
	);

	for(const auto& table_instance : _game.tables) {

		auto table=new app::draw_table{table_instance};
		sortable_components.push_back(
			std::shared_ptr<draw_component>(table)
		);
	}
}

