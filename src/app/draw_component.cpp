#include <app/draw_component.h>
#include <app/resources.h>

#include <ldv/box_representation.h>
#include <sstream>

using namespace app;

/////
//draw timer
draw_timer::draw_timer(
	const game& _game, 
	const ldtools::ttf_manager& _ttf_manager, 
	const resources& _resources
):
	txt_rep{
		_ttf_manager.get("hud", _resources.game_hud_font_size),
		ldv::rgba8(255, 255, 255, 255),
		"..."
	}
 {

	timer_fn=[&]() {

		return _game.get_remaining_seconds();
	};
}

void draw_timer::draw(
	ldv::screen& _screen, 
	const ldv::camera& /*_camera*/
) const {

	txt_rep.set_text(std::to_string(timer_fn()));
	//TODO: no magic, please.
	txt_rep.go_to({150, 56});
	txt_rep.draw(_screen);
}

/////
//draw bar

draw_bar::draw_bar(
	const draw_sprite& _draw_sprite,
	const app::draw_info& _draw_info,
	const bar& _bar
):
	sprite_draw{_draw_sprite},
	draw_info{_draw_info},
	bar_ptr{&_bar}
{

}

void draw_bar::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {

	sprite_draw.draw(
		app::resources::spr_bar, 
		bar_ptr->get_collision_box(),
		app::resources::tex_sprites,
		_screen,
		_camera
	);

	if(draw_info.interact_bar) {

		auto box=bar_ptr->get_collision_box();
		box.origin.x-=1;
		box.origin.y-=1;

		sprite_draw.draw(
			app::resources::spr_bar_outline, 
			box,
			app::resources::tex_sprites,
			_screen,
			_camera
		);
	}
}

/////
//draw trash

draw_trash::draw_trash(
	const draw_sprite& _draw_sprite,
	const app::draw_info& _draw_info,
	const trash& _trash
):
	sprite_draw{_draw_sprite},
	draw_info{_draw_info},
	trash_ptr{&_trash}
{

}


void draw_trash::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {

	sprite_draw.draw(
		app::resources::spr_trash, 
		trash_ptr->get_collision_box(),
		app::resources::tex_sprites,
		_screen,
		_camera
	);

	if(draw_info.interact_trash) {

		auto box=trash_ptr->get_collision_box();
		box.origin.x-=1;
		box.origin.y-=1;

		sprite_draw.draw(
			app::resources::spr_trash_outline, 
			box,
			app::resources::tex_sprites,
			_screen,
			_camera
		);
	}
}

/////
//draw table

draw_table::draw_table(
	const draw_sprite& _draw_sprite,
	const app::draw_info& _draw_info,
	const table& _table
):
	sprite_draw{_draw_sprite},
	draw_info{_draw_info},
	table_ptr{&_table}
{

}

void draw_table::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {

	sprite_draw.draw(
		app::resources::spr_table, 
		table_ptr->get_collision_box(), 
		app::resources::tex_sprites, 
		_screen,
		_camera
	);

	if(draw_info.interact_table && draw_info.current_table==table_ptr) {

		auto box=table_ptr->get_collision_box();
		box.origin.x-=1;
		box.origin.y-=1;

		sprite_draw.draw(
			app::resources::spr_table_outline, 
			box,
			app::resources::tex_sprites,
			_screen,
			_camera
		);
	}
}

//////////////
// draw_player
///

draw_player::draw_player(
	const draw_sprite& _draw_sprite,
	const app::draw_info& _draw_info,
	const player& _player,
	const tray& _tray
):
	sprite_draw{_draw_sprite},
	draw_info{_draw_info},
	player_ptr{&_player},
	tray_ptr{&_tray},
	time{0.0f}
{

}

void draw_player::tick(
	double _delta
) {

	const auto& motion=player_ptr->get_motion();
	if(!motion.x && !motion.y) {
	
		time=0.f;
	}
	else {

		time+=_delta;
	}
}

void draw_player::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {

	int index=0;

	//Static player...
	if(!time) {

		switch(player_ptr->get_current_face()) {

			case player::facing::north: index=app::resources::spr_player_north; break;
			case player::facing::south: index=app::resources::spr_player_south; break;
			case player::facing::east: index=app::resources::spr_player_east; break;
			case player::facing::west: index=app::resources::spr_player_west; break;
		}

		sprite_draw.draw(
			index,
			player_ptr->get_collision_box(), 
			app::resources::tex_sprites, 
			_screen,
			_camera
		);
	}
	else{

		switch(player_ptr->get_current_face()) {

			case player::facing::north: index=app::resources::anm_player_north; break;
			case player::facing::south: index=app::resources::anm_player_south; break;
			case player::facing::east: index=app::resources::anm_player_east; break;
			case player::facing::west: index=app::resources::anm_player_west; break;
		}

		sprite_draw.anim(
			index,
			time,
			player_ptr->get_collision_box(), 
			app::resources::tex_sprites, 
			_screen,
			_camera
		);
	}
}

