#pragma once

#include <app/env.h>

#include <rapidjson/document.h>
#include <string>

namespace app {

//service to retrieve controller layouts.
class layout {

	public:

								layout(const app::env&);

	const rapidjson::Value& 	get(const std::string&) const;

	private:

	rapidjson::Document 		doc;

};

}
