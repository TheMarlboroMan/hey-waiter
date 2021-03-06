#pragma once

#include <app/game.h>
#include <app/definitions.h>
#include <app/resources.h>
#include <app/draw_component.h>
#include <app/layout.h>

#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldv/resource_manager.h>
#include <ldtools/ttf_manager.h>
#include <ldtools/view_composer.h>
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
							const ldtools::sprite_table&,
							const ldtools::animation_table&,  
							const tools::i8n&,
							const app::draw_sprite&,
							const app::layout&
						);
	void				do_draw(ldv::screen&, const ldv::camera&, const app::game&);
	void				set_debug(bool _val) {debug=_val;}
	bool				get_debug() const {return debug;}
	void				populate(const app::game&);
	void				tick(float);

	private:

	void				setup_bar();
	void				update_tray(const app::game&);
	void				update_table(const app::game&);
	void				update_bar_selector_position(const app::game&);
	void				update_tray_selector_position(const app::game&);
	void				toggle_selector(ldtools::view_composer&, bool);
	void				set_view_selector(ldtools::view_composer&, int);
	void				fill_view_with_consumables(ldtools::view_composer&, const std::vector<consumable>);
	void				draw_background(ldv::screen&, const ldv::camera&, const app::game&);
	void				draw_fill_tray(ldv::screen&);
	void				draw_serve(ldv::screen&);
	void				draw_score(ldv::screen&, const app::score&);

	void				debug_draw_bar(ldv::screen&, const ldv::camera&, const app::bar&);
	void				debug_draw_trash(ldv::screen&, const ldv::camera&, const trash&);
	void				debug_draw_interactions(ldv::screen&, const app::game&);
	void				debug_draw_take_order(ldv::screen&, const app::game&);
	std::string			debug_consumable_to_string(const app::consumable&) const;
	void				debug_draw_player(ldv::screen&, const ldv::camera&, const player&);
	void				debug_draw_table(ldv::screen&, const ldv::camera&, const table&);
	void				debug_draw_obstacle(ldv::screen&, const ldv::camera&, const obstacle&);
	void				debug_draw_level_number(ldv::screen&, const app::game&);
	ldv::rect			to_video(const box&) const;

	const app::resources& 						resources;
	const ldv::resource_manager& 				video_resource_manager;
	const ldtools::ttf_manager&					ttf_manager;
	const ldtools::sprite_table&				sprite_table;
	const ldtools::animation_table&				animation_table;
	const tools::i8n&							i8n;
	const app::draw_sprite&						draw_sprite;
	ldtools::view_composer						consumable_selector,
												tray_list,
												table_list;
	std::vector<std::shared_ptr<draw_component>>	sortable_components;
	std::shared_ptr<draw_player>				draw_player_instance;
//	std::vector<ldv::bitmap_representation*>	tray_items;
	std::vector<consumable>		 				bar_item_order;
	app::draw_info								draw_info;
	bool										debug{false};
};

}
