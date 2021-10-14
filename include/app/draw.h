#pragma once

#include <app/game.h>
#include <app/definitions.h>
#include <app/resources.h>
#include <app/draw_component.h>

#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/resource_manager.h>
#include <ldtools/ttf_manager.h>
#include <tools/i8n.h>
#include <string>
#include <vector>
#include <memory>

namespace app {

class draw {

	public:

						draw(
							const app::resources&, 
							const ldv::resource_manager&,
							const ldtools::ttf_manager&, 
							const tools::i8n&,
							const app::draw_sprite&
						);
	void				do_draw(ldv::screen&, const ldv::camera&, const app::game&);
	void				set_debug(bool _val) {debug=_val;}
	void				populate(const app::game&);

	private:

	void				draw_background(ldv::screen&, const ldv::camera&, const app::game&);
	void				draw_table(ldv::screen&, const ldv::camera&, const table&);
	void				draw_bar(ldv::screen&, const ldv::camera&, const app::bar&);
	void				draw_obstacle(ldv::screen&, const ldv::camera&, const obstacle&);
	void				draw_trash(ldv::screen&, const ldv::camera&, const trash&);
	void				draw_interactions(ldv::screen&, const app::game&);
	void				draw_fill_tray(ldv::screen&, const app::game&);
	void				draw_serve(ldv::screen&, const app::game&);
	void				draw_take_order(ldv::screen&, const app::game&);
	void				draw_score(ldv::screen&, const app::score&);
	void				draw_timer(ldv::screen&, const app::game&);
	void				draw_level_number(ldv::screen&, const app::game&);
	ldv::rect			to_video(const box&) const;
	std::string			consumable_to_string(const app::consumable&) const;

	const app::resources& 		resources;
	const ldv::resource_manager& video_resource_manager;
	const ldtools::ttf_manager&	ttf_manager;
	const tools::i8n&			i8n;
	const app::draw_sprite&		draw_sprite;
	std::vector<std::shared_ptr<draw_component>>	sortable_components;
	bool						debug{false};



};

}
