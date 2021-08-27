#pragma once

#include <app/game.h>
#include <app/definitions.h>

#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldtools/ttf_manager.h>
#include <tools/i8n.h>
#include <string>

namespace app {

class draw {

	public:

						draw(const ldtools::ttf_manager&, const tools::i8n&);
	void				do_draw(ldv::screen&, const ldv::camera&, const app::game&);

	private:

	void				draw_table(ldv::screen&, const ldv::camera&, const table&);
	void				draw_interactions(ldv::screen&, const app::game&);
	void				draw_fill_tray(ldv::screen&, const app::game&);
	void				draw_serve(ldv::screen&, const app::game&);
	void				draw_take_order(ldv::screen&, const app::game&);
	void				draw_score(ldv::screen&, const app::score&);
	void				draw_timer(ldv::screen&, const app::game&);
	ldv::rect			to_video(const box&) const;
	std::string			consumable_to_string(const app::consumable&) const;

	const ldtools::ttf_manager&	ttf_manager;
	const tools::i8n&			i8n;


};

}
