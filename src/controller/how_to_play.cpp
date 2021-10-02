#include <controller/how_to_play.h>
#include <input/input.h>

#include <tools/json.h>
#include <tools/file_utils.h>
#include <ldv/ttf_representation.h>

using namespace controller;

how_to_play::how_to_play(
	lm::logger& _logger,
	const app::env& _env,
	dfw::audio& _audio,
	const lda::resource_manager& _arm,
	const app::resources& _resources,
	const tools::i8n& _i8n,
	const ldtools::ttf_manager& _ttf_manager
)
	:log{_logger},
	env{_env},
	audio{_audio},
	audio_rm{_arm},
	resources{_resources},
	i8n{_i8n} {

	layout.map_font(
		"how_to_play", 
		_ttf_manager.get(
			"how_to_play", 
			resources.settings_font_size
		)
	);

	//TODO: This json file is being parsed everywhere!!! Just parse it once!!!
	const std::string layout_path=_env.build_data_path("layout/layouts.json");
	auto document=tools::parse_json_string(
		tools::dump_file(layout_path)
	);
	layout.parse(document["how_to_play"]);


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
