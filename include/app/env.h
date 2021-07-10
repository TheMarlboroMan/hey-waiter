#pragma once

#include <string>

namespace app {

class env {

	public:

	std::string			build_data_path(const std::string& _str) const {return std::string{"../resources/data/"}+_str;}
	std::string			build_user_path(const std::string& _str) const {return std::string{"../resources/data/"}+_str;}
};

}
