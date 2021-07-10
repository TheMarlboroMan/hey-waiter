#pragma once

#include <app/game.h>
#include <app/definitions.h>

#include <ldv/screen.h>
#include <ldv/camera.h>
#include <ldtools/ttf_manager.h>
#include <tools/i8n.h>


namespace app {

class draw {

	public:

						draw(const ldtools::ttf_manager&, const tools::i8n&);
	void				do_draw(ldv::screen&, const ldv::camera&, const app::game&);

	private:

	void				draw_interactions(ldv::screen&, const app::game&);
	void				draw_fill_tray(ldv::screen&, const app::game&);
	ldv::rect			to_video(const box&) const;

	const ldtools::ttf_manager&	ttf_manager;
	const tools::i8n&			i8n;


};

}
