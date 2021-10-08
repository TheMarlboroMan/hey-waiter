//I copied this file from the dfw-jumpstart project and just made the necessary
//changes. Long live rock and roll.

#include "controller/settings.h"

#include "input/input.h"

#include <tools/json.h>
#include <tools/file_utils.h>
#include <ldv/representation.h>
#include <ldv/ttf_representation.h>

#include <algorithm>
#include <numeric>

using namespace controller;

settings::settings(app::dependency_container& _dc) 
:
	log(_dc.get_log()),
	env(_dc.get_env()),
	audio{_dc.get_audio()},
	audio_rm{_dc.get_audio_resource_manager()},
	i8n{_dc.get_i8n()},
	input{_dc.get_input()},
	config{_dc.get_config()}
{

	auto json_document=tools::parse_json_string(
		tools::dump_file(env.build_user_path("settings.json"))
	);

	
	tools::options_menu_from_json(
		json_document["settings"],
		menu
	);

	layout.map_font(
		"settings", 
		_dc.get_ttf_manager().get(
			"settings", 
			_dc.get_resources().settings_font_size
		)
	);

	//TODO: This json file is being parsed everywhere!!! Just parse it once!!!
	const std::string layout_path=env.build_data_path("layout/layouts.json");
	auto document=tools::parse_json_string(
		tools::dump_file(layout_path)
	);
	layout.parse(document["settings"]);


	auto set_text=[this](const std::string& _id, const std::string& _key) {

		static_cast<ldv::ttf_representation*>(layout.get_by_id(_id))->set_text(i8n.get(_key));
	};

	name_keys={
		"10_sound_volume",
		"20_music_volume",
		"30_left",
		"40_right",
		"50_up",
		"60_down",
		"70_interact",
		"80_back"
	};

	value_keys={
		"10_sound_volume_value",
		"20_music_volume_value",
		"30_left_value",
		"40_right_value",
		"50_up_value",
		"60_down_value",
		"70_interact_value"
	};

	//Translate and align left side...
	std::map<std::string, std::string> translations={
		{"10_sound_volume", "settings-sound_volume"},
		{"20_music_volume", "settings-music_volume"},
		{"30_left", "settings-input_left"},
		{"40_right", "settings-input_right"},
		{"50_up", "settings-input_up"},
		{"60_down", "settings-input_down"},
		{"70_interact", "settings-input_interact"},
		{"80_back", "settings-back"}
	};

	auto align_rect=_dc.get_screen().get_rect();
	align_rect.w/=2;

	auto left_side_align=ldv::representation_alignment{
		ldv::representation_alignment::h::inner_right,
		ldv::representation_alignment::v::none,
		10,
		0
	};

	for(const auto& pair: translations) {

		set_text(pair.first, pair.second);
		layout.get_by_id(pair.first)->align(align_rect, left_side_align);
	}

	//Align right side...
	auto right_side_align=ldv::representation_alignment{
		ldv::representation_alignment::h::outer_right,
		ldv::representation_alignment::v::none,
		10,
		0
	};

	for(const auto& id : value_keys) {

		layout.get_by_id(id)->align(align_rect, right_side_align);
	}

	//Set current values, only for these, which are not toggle or free.
	menu.set("10_sound_volume", volume_internal_to_config(audio().get_main_sound_volume()));
	menu.set("20_music_volume", volume_internal_to_config(audio().get_main_music_volume()));
}

void settings::awake(
	dfw::input& /*_input*/
) {

	current_selection=menu.get_keys().front();
	sync_view();
	sync_selection();
}

void settings::slumber(
	dfw::input& /*_input*/
) {

	config.save();
}

void settings::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	if(!learning_mode) {

		loop_navigate(_input, _lid);
	}
	else {

		loop_learn(_input, _lid);
	}
}

void settings::loop_navigate(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input.is_input_down(input::escape)) {

		set_state(t_states::state_menu);
		return;
	}

	if(_input.is_input_down(input::up)) {

		prev();
	}
	else if(_input.is_input_down(input::down)) {

		next();
	}
	else if(_input.is_input_pressed(input::left)) {

		left(_input.is_input_down(input::left), _lid.delta);
	}
	else if(_input.is_input_pressed(input::right)) {

		right(_input.is_input_down(input::right), _lid.delta);
	}
	//TODO: or SDL space xD!
	else if(_input.is_input_down(input::interact)) {

		select();
	}
	else {

		press_timer=0.f;
	}
}

void settings::loop_learn(
	dfw::input& /*_input*/,
	const dfw::loop_iteration_data& /*_lid*/
) {

	if(!learning_input_entered) {

		return;
	}

	auto descriptions=input.locate_descriptions(learning_input_id);

	//Remove the last, add the new at the first place...
	descriptions.pop_back();
	descriptions.insert(std::begin(descriptions), learned_input);

	input.clear(learning_input_id);
	for(const auto desc : descriptions) {
		input.configure({desc, learning_input_id});
	}

	auto type_to_int=[](dfw::input_description::types _type) {

		switch(_type) {
			case dfw::input_description::types::keyboard: return 0;
			case dfw::input_description::types::mouse: return 1;
			case dfw::input_description::types::joystick: return 2;
			case dfw::input_description::types::none: return 3;
		}

		return 0;
	};

	const auto& main=descriptions[0];
	config.set_vector(int_to_config_name(learning_input_id), std::vector<int>{type_to_int(main.type), main.device, main.code});

	if(descriptions.size() > 1) {

		const auto& alt=descriptions[1];
		config.set_vector(int_to_config_name(learning_input_id)+"_alt", std::vector<int>{type_to_int(alt.type), alt.device, alt.code});
	}

	end_learning_mode();
}

void settings::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

	_screen.clear(ldv::rgba8(0, 0, 0, 255));
	layout.draw(_screen);
}


void settings::prev() {

	current_selection=menu.adjacent_key(current_selection, tools::options_menu<std::string>::browse_dir::previous);
	audio.play_sound(audio_rm.get_sound(app::resources::snd_default));
	sync_selection();
}

void settings::next() {

	current_selection=menu.adjacent_key(current_selection, tools::options_menu<std::string>::browse_dir::next);
	audio.play_sound(audio_rm.get_sound(app::resources::snd_default));
	sync_selection();
}

void settings::left(
	bool _down,
	float _delta
) {

	press_timer+=_delta;

	if(current_selection=="10_sound_volume") {

		if(press_timer < 0.3f && !_down) {

			return;
		}

		menu.browse(current_selection, tools::options_menu<std::string>::browse_dir::previous);
		int new_value=volume_config_to_internal(menu.get_int(current_selection));
		audio().set_main_sound_volume(new_value);
		config.set("audio:sound_volume", new_value);
	}
	else if(current_selection=="20_music_volume") {

		if(press_timer < 0.3f && !_down) {

			return;
		}

		menu.browse(current_selection, tools::options_menu<std::string>::browse_dir::previous);
		int new_value=volume_config_to_internal(menu.get_int(current_selection));
		audio().set_main_music_volume(new_value);
		config.set("audio:music_volume", new_value);
	}
	else {

		return;
	}

	if(_down) {

		audio.play_sound(audio_rm.get_sound(app::resources::snd_default));
	}

	sync_view();
}

void settings::right(
	bool _down,
	float _delta
) {

	press_timer+=_delta;

	if(current_selection=="10_sound_volume") {

		if(press_timer < 0.3f && !_down) {

			return;
		}

		menu.browse(current_selection, tools::options_menu<std::string>::browse_dir::next);
		int new_value=volume_config_to_internal(menu.get_int(current_selection));
		audio().set_main_sound_volume(new_value);
		config.set("audio:sound_volume", new_value);
	}
	else if(current_selection=="20_music_volume") {

		if(press_timer < 0.3f && !_down) {

			return;
		}

		menu.browse(current_selection, tools::options_menu<std::string>::browse_dir::next);
		int new_value=volume_config_to_internal(menu.get_int(current_selection));
		audio().set_main_music_volume(new_value);
		config.set("audio:music_volume", new_value);
	}
	else {

		return;
	}

	if(_down) {

		audio.play_sound(audio_rm.get_sound(app::resources::snd_default));
	}

	sync_view();
}

void settings::select() {

	if(current_selection=="80_back") {

		set_state(t_states::state_menu);
		return;
	}
	else if(current_selection=="10_sound_volume" || current_selection=="20_sound_volume") {

		return;
	}
	else {

		start_learning_mode(current_selection);
	}

	audio.play_sound(audio_rm.get_sound(app::resources::snd_default));
	sync_view();
}

void settings::sync_view() {

	auto set_value=[this](const std::string& _id, const std::string& _value) {

		static_cast<ldv::ttf_representation*>(layout.get_by_id(_id))->set_text(_value);
	};

	auto translate_input=[this](auto _input_type, auto _id) -> std::string {

		if(learning_mode && current_selection==_id) {

			return i8n.get("settings-learn");
		}

		auto descriptions=input.locate_descriptions(_input_type);
		if(!descriptions.size()) {

			return "???";
		}

		auto uniqit=std::unique(
			std::begin(descriptions),
			std::end(descriptions),
			[](const dfw::input_description& _a, const dfw::input_description& _b) {
				return _a.type==_b.type && _a.code==_b.code && _a.device==_b.device;
			}
		);

		descriptions.erase(uniqit, std::end(descriptions));

		std::vector<std::string> result;
		for(const auto& l : descriptions) {

			switch(l.type) {
				case dfw::input_description::types::keyboard:
					result.push_back(SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)l.code)));
				break;
				case dfw::input_description::types::mouse:
					result.push_back(i8n.get("settings-mouse_button")+" "+std::to_string(l.code));
				break;
		 		case dfw::input_description::types::joystick:
					result.push_back(i8n.get("settings-joystick")+" "+std::to_string(l.device)+" "+i8n.get("settings-button")+" "+std::to_string(l.code));
				break;
				case dfw::input_description::types::none:
				break;
			}
		}

		return std::accumulate(
			std::begin(result),
			std::end(result),
			std::string(),
			[](std::string &ss, std::string &s) {
				return ss.empty() ? s : ss + ", " + s;
			}
		);
	};

	int audio_volume=volume_internal_to_config(audio().get_main_sound_volume());
	int music_volume=volume_internal_to_config(audio().get_main_music_volume());

	set_value("10_sound_volume_value", std::to_string(audio_volume)+"%");
	set_value("20_music_volume_value", std::to_string(music_volume)+"%");
	set_value("30_left_value", translate_input(input::left, "30_left"));
	set_value("40_right_value", translate_input(input::right, "40_right"));
	set_value("50_up_value", translate_input(input::up, "50_up"));
	set_value("60_down_value", translate_input(input::down, "60_down"));
	set_value("70_interact_value", translate_input(input::interact, "70_interact"));
}

void settings::sync_selection() {

	for(const auto& id : name_keys) {
		layout.get_by_id(id)->set_alpha(160);
	}

	layout.get_by_id(current_selection)->set_alpha(255);
}

int settings::volume_internal_to_config(
	int _value
) const {

	return (_value*100) / 127;
}

int settings::volume_config_to_internal(
	int _value
) const {

	return (_value*127) / 100;
}

void settings::start_learning_mode(
	const std::string& _key
) {

	learning_input_id=input_key_to_int(_key);
	learning_mode=true;

	auto f=[this](SDL_Event& e, ldi::sdl_input::tf_default& df) {

		dfw::input_description::types t=dfw::input_description::types::none;
		int code=0, device=0;

		if(e.type==SDL_KEYDOWN)
		{
			t=dfw::input_description::types::keyboard;
			code=e.key.keysym.scancode;

			if(code==SDL_SCANCODE_ESCAPE) {
				end_learning_mode();
			}
		}
		else if(e.type==SDL_MOUSEBUTTONDOWN)
		{
			t=dfw::input_description::types::mouse;
			code=e.button.button;
		}
		else if(e.type==SDL_JOYBUTTONDOWN)
		{
			t=dfw::input_description::types::joystick;
			code=e.jbutton.button;
			device=input().get_joystick_index_from_id(e.jbutton.which);
		}
		else //Any other thing and we resort to the default treatment.
		{
			df(e);
			learned_input={t, code, device};
			return true;
		}

		learning_input_entered=true;
		learned_input={t, code, device};
		return true;
	};

	//Begin the "learn" mode.
	input().set_event_processing_function(f);
}

void settings::end_learning_mode() {

	learning_mode=false;
	learning_input_entered=false;
	learning_input_id=0;
	sync_view();
	input().reset_event_processing_function();
}

int settings::input_key_to_int(
	const std::string _key
) const {
	if(_key=="30_left") return input::left;
	else if(_key=="40_right") return input::right;
	else if(_key=="50_up") return input::up;
	else if(_key=="60_down") return input::down;
	else if(_key=="70_interact") return input::interact;
	return 0;
}

std::string settings::int_to_config_name(
	int _value
) const {

	switch(_value) {
		case input::left: return "input:left";
		case input::right: return "input:right";
		case input::up: return "input:up";
		case input::down: return "input:down";
		case input::interact: return "input:interact";
	}

	return "";
}
