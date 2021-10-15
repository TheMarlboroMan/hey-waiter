#include <app/draw_component.h>
#include <app/resources.h>


#include <ldv/box_representation.h>

using namespace app;

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
	tray_ptr{&_tray}
{

}

void draw_player::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {

	sprite_draw.draw(
		app::resources::spr_table, 
		player_ptr->get_collision_box(), 
		app::resources::tex_sprites, 
		_screen,
		_camera
	);

/*
	auto player_color=ldv::rgba8(255, 255, 255, 255);
	if(tray_ptr->has_trash()) {

		player_color=ldv::rgba8(255, 0, 0, 255);
	}
	else if(!tray_ptr->is_empty()) {

		player_color=ldv::rgba8(0, 255, 0, 255);
	}

TODO: There's no "to video".
	ldv::box_representation player_box(
		to_video(player_ptr->get_collision_box()),
		player_color
	);
	player_box.draw(_screen, _camera);
*/
}

