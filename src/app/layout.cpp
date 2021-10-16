#include <app/layout.h>

#include <tools/json.h>
#include <tools/file_utils.h>

using namespace app;

layout::layout(const app::env& _env) {

	const std::string layout_path=_env.build_data_path("layout/layouts.json");
	doc=tools::parse_json_string(
		tools::dump_file(layout_path)
	);
}

void layout::parse_into(
	const std::string& _key,
	ldtools::view_composer& _view_composer
) const {

	_view_composer.parse(doc[_key.c_str()]);
}
