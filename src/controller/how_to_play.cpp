#include <controller/how_to_play.h>
#include <input/input.h>
#include <app/layout.h>

#include <ldv/ttf_representation.h>

using namespace controller;

how_to_play::how_to_play(app::dependency_container& _dc) 
	:log{_dc.get_log()},
	env{_dc.get_env()},
	audio{_dc.get_audio()},
	audio_rm{_dc.get_audio_resource_manager()},
	i8n{_dc.get_i8n()} {

	layout.map_font(
		"how_to_play", 
		_dc.get_ttf_manager().get(
			"how_to_play", 
			_dc.get_resources().settings_font_size
		)
	);

	layout.parse(_dc.get_layout().get("how_to_play"));


	auto set_text=[this](const std::string& _id, const std::string& _key) {

		static_cast<ldv::ttf_representation*>(layout.get_by_id(_id))->set_text(i8n.get(_key));
	};

	set_text("text", "how_to_play-placeholder");
}

void how_to_play::loop(
	dfw::input& _input, 
	const dfw::loop_iteration_data& /*lid*/
) {

	if(_input().is_exit_signal()) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(input::escape)) {

		set_state(t_states::state_menu);
		return;
	}
}

void how_to_play::draw(
	ldv::screen& _screen, 
	int /*fps*/
) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));
	layout.draw(_screen);
}
