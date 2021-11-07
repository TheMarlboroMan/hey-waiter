#include <app/draw_sprite.h>

#include <iostream>

using namespace app;

draw_sprite::draw_sprite(
	const ldtools::sprite_table& _sprite_table,
	const ldtools::animation_table& _animation_table,
	const ldv::resource_manager& _vrm
):
	sprite_table{_sprite_table},
	animation_table{_animation_table},
	vrm{_vrm}
{

	bmp.set_blend(ldv::representation::blends::alpha);
}

void draw_sprite::draw(
	int _sprite, 
	const box& _position, 
	int _tex, 
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {


	const auto& sprite=sprite_table.get(_sprite);
	const auto sprite_box=sprite.box;
	auto origin=to_sprite_point(_position, sprite_box);
	origin.x-=sprite.disp_x;

	bmp.set_texture(vrm.get_texture(_tex));
	bmp.set_location({origin, sprite_box.w, sprite_box.h});
	bmp.set_clip(sprite_box);
	bmp.draw(_screen, _camera);
}

void draw_sprite::anim(
	int _index, 
	float _time, 
	const box& _position, 
	int _tex, 
	ldv::screen& _screen, 
	const ldv::camera& _camera
) const {

	const auto& anim=animation_table.get(_index);
	const auto& line=anim.get_for_time(_time);
	const auto& sprite=line.frame;
	const auto sprite_box=sprite.box;
	auto origin=to_sprite_point(_position, sprite_box);
	origin.x-=sprite.disp_x;

	bmp.set_texture(vrm.get_texture(_tex));
	bmp.set_location({origin, sprite_box.w, sprite_box.h});
	bmp.set_clip(sprite_box);
	bmp.draw(_screen, _camera);
}

ldv::point draw_sprite::to_sprite_point(
	const box& _position_box,
	const ldv::rect& _sprite_box
) const {

	int x=_position_box.origin.x,
		y=-(_position_box.origin.y+_sprite_box.h);

	return {x, y};
}
