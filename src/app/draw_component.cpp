#include <app/draw_component.h>

using namespace app;

/////
//draw bar

draw_bar::draw_bar(
	const bar& _bar
):
	bar_ptr{&_bar}
{

}

void draw_bar::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {

/*
	//TODO: needs sprite table
	//TODO: needs to sprite point
	//TODO: needs VRM
	//TODO: make a service: sprite_draw, which contains such shit.

	const auto sprite_box=sprite_table.get(app::resources::spr_bar).box;
	auto origin=to_sprite_point(_bar.get_collision_box(), sprite_box);

	ldv::bitmap_representation bmp(
		video_resource_manager.get_texture(app::resources::tex_sprites),
		{origin, sprite_box.w, sprite_box.h},
		sprite_box
	);

	bmp.set_blend(ldv::representation::blends::alpha);
	bmp.draw(_screen, _camera);
*/


}

/////
//draw trash

draw_trash::draw_trash(
	const trash& _trash
):
	trash_ptr{&_trash}
{
}


void draw_trash::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {

/*

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
*/

}

/////
//draw table

draw_table::draw_table(
	const table& _table
):
	table_ptr{&_table}
{

}

void draw_table::draw(
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {

/*
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
*/

}

