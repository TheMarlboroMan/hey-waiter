#pragma once

#include <app/table.h>
#include <app/bar.h>
#include <app/trash.h>

#include <ldv/screen.h>
#include <ldv/camera.h>

namespace app {

/**
*A way to pass down the current interactions, for the shiny border.
*/

struct draw_info {

	bool	interact_bar{false},
			interact_trash{false},
			interact_table{false};

	table const * current_table{nullptr};
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

								draw_bar(const bar&);
	virtual point				origin() const {return bar_ptr->get_collision_box().origin;}
	virtual void				draw(ldv::screen&, const ldv::camera&) const;
	virtual void				tick(double) {}

	private:

	bar const *			bar_ptr;

};

class draw_trash
	: public draw_component {

	public:

								draw_trash(const trash&);
	virtual point				origin() const {return trash_ptr->get_collision_box().origin;}
	virtual void				draw(ldv::screen&, const ldv::camera&) const;
	virtual void				tick(double) {}

	private:

	trash const *		trash_ptr;

};

class draw_table
	: public draw_component {

	public:

								draw_table(const table&);
	virtual point				origin() const {return table_ptr->get_collision_box().origin;}
	virtual void				draw(ldv::screen&, const ldv::camera&) const;
	virtual void				tick(double) {}

	private:

	table const *		table_ptr;

};

}
