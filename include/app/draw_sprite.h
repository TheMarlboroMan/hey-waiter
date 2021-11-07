#pragma once

#include <app/definitions.h>

#include <ldtools/sprite_table.h>
#include <ldtools/animation_table.h>
#include <ldv/resource_manager.h>
#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/bitmap_representation.h>

namespace app {

class draw_sprite {

	public:

										draw_sprite(
											const ldtools::sprite_table&,
											const ldtools::animation_table&,
											const ldv::resource_manager&
										);

	/**
	*draws sprite index at box position, with texture.
	*/
	void								draw(int, const box&, int, ldv::screen&, const ldv::camera&) const;

	/**
	*draws animation frame for the given time, at the box position, with texture.
	*/
	void								anim(int, float, const box&, int, ldv::screen&, const ldv::camera&) const;


	private:

	ldv::point							to_sprite_point(const box&, const ldv::rect&) const;

	const ldtools::sprite_table&		sprite_table;
	const ldtools::animation_table&		animation_table;
	const ldv::resource_manager&		vrm;

	//TODO: instance a new bmp to get rid of the mutable thing, if need be.
	mutable ldv::bitmap_representation	bmp;

};

}
