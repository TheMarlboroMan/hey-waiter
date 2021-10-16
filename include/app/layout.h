#pragma once

#include <app/env.h>

#include <ldtools/view_composer.h>
#include <rapidjson/document.h>
#include <string>

namespace app {

//service to retrieve controller layouts.
class layout {

	public:

								layout(const app::env&);

/**
*this is so because stuff can't be parsed if we don't assign fonts and stuff...
*/
	void						parse_into(const std::string&, ldtools::view_composer&) const;

	private:

	rapidjson::Document 		doc;

};

}
