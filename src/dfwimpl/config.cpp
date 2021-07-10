#include "../../include/dfwimpl/config.h"

using namespace dfwimpl;

config::config(
	const app::env& _env
): dfw::base_config(_env.build_user_path(get_file_path())) {
}


dfw::input_description dfwimpl::input_description_from_config_token(const rapidjson::Value& tok)
{
	if(!tok.IsArray()) {
		throw std::runtime_error("app::input_description_from_config_token, token is not array");
	}

	const auto& v=tok.GetArray();

	if(v.Size() != 3) {
		throw std::runtime_error("app::input_description_from_config_token, array has not size of 3");
	}

	return dfw::input_description{
		input_description_type_from_int(v[0].GetInt()), v[2].GetInt(), v[1].GetInt()
	};
}

std::vector<int> dfwimpl::config_token_from_input_description(const dfw::input_description& _id) {
	
	return std::vector<int>{
		input_description_int_from_type(_id.type),
		_id.device,
		_id.code
	};
}

dfw::input_description::types dfwimpl::input_description_type_from_int(int v) {

	switch(v) {
		case 0: return dfw::input_description::types::keyboard; break;
		case 1: return dfw::input_description::types::mouse; break;
		case 2: return dfw::input_description::types::joystick; break;
	}

	return dfw::input_description::types::none;
}

int dfwimpl::input_description_int_from_type(dfw::input_description::types t) {

	switch(t) {
		case dfw::input_description::types::keyboard: 	return 0; break;
		case dfw::input_description::types::mouse: 	return 1; break;
		case dfw::input_description::types::joystick: 	return 2; break;
		case dfw::input_description::types::none: 	return 3; break;
	}

	return 3;
}
