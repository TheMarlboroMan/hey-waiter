#pragma once

#include <app/table.h>
#include <app/bar.h>
#include <app/trash.h>
#include <app/player.h>
#include <app/tray.h>
#include <app/draw_sprite.h>

#include <ldv/screen.h>
#include <ldv/camera.h>

namespace app {

/**
*A way to pass down the current interactions, for the shiny border.
*/

struct draw_info {

	bool						interact_bar{false},
								interact_trash{false},
								interact_table{false};

	table const * 				current_table{nullptr};

	void 						reset() {

		interact_bar=false;
		interact_trash=false;
		interact_table=false;
		current_table=nullptr;
	}
};

/**
*base for all of them.
*/
class draw_component {

	public:

	virtual 					~draw_component() {}
	virtual point				origin() const=0;
	virtual void				draw(ldv::screen&, const ldv::camera&) const=0;
	virtual void				tick(double)=0;
};

class draw_bar
	: public draw_component {

	public:

								draw_bar(const draw_sprite&, const draw_info&, const bar&);
	virtual point				origin() const {return bar_ptr->get_collision_box().origin;}
	virtual void				draw(ldv::screen&, const ldv::camera&) const;
	virtual void				tick(double) {}

	private:

	const app::draw_sprite		sprite_draw;
	const app::draw_info&		draw_info;
	bar const *					bar_ptr;
};

class draw_trash
	: public draw_component {

	public:

								draw_trash(const draw_sprite&, const draw_info&, const trash&);
	virtual point				origin() const {return trash_ptr->get_collision_box().origin;}
	virtual void				draw(ldv::screen&, const ldv::camera&) const;
	virtual void				tick(double) {}

	private:

	const app::draw_sprite&		sprite_draw;
	const app::draw_info&		draw_info;
	trash const *				trash_ptr;
};

class draw_table
	: public draw_component {

	public:

								draw_table(const draw_sprite&, const draw_info&, const table&);
	virtual point				origin() const {return table_ptr->get_collision_box().origin;}
	virtual void				draw(ldv::screen&, const ldv::camera&) const;
	virtual void				tick(double) {}

	private:

	const app::draw_sprite&		sprite_draw;
	const app::draw_info&		draw_info;
	table const *				table_ptr;
};

class draw_player
	: public draw_component {

	public:

								draw_player(const draw_sprite&, const draw_info&, const player&, const tray&);
	virtual point				origin() const {return player_ptr->get_collision_box().origin;}
	virtual void				draw(ldv::screen&, const ldv::camera&) const;
	virtual void				tick(double) {}

	private:

	const app::draw_sprite&		sprite_draw; 
	const app::draw_info&		draw_info;
	player const *				player_ptr;
	tray const *				tray_ptr;
};

}
