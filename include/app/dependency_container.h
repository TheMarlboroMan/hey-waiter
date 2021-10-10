#pragma once

#include <memory>

namespace lm {class logger;}
namespace dfw {class audio; class input;}
namespace dfwimpl {class config;}
namespace ldtools {class ttf_manager;}
namespace lda {class resource_manager;}
namespace tools {class i8n;}
namespace ldv {class screen; class resource_manager;}
namespace app {

class env; 
class resources; 
class hi_score_manager;
class score; 
class layout;

/**
*exactly what it sounds like. An easy way to grab dependencies.
*/

class dependency_container {

	public:

	app::resources&							get_resources();
	tools::i8n&								get_i8n();
	ldtools::ttf_manager&					get_ttf_manager();
	app::hi_score_manager&					get_hi_score_manager();
	app::score&								get_score();
	app::layout&							get_layout();

	const app::env&							get_env() const {return *env;}
	const ldv::screen&						get_screen() const {return *screen;}
	dfw::input&								get_input() {return *input;}
	dfwimpl::config&						get_config() {return *config;}
	dfw::audio&								get_audio() {return *audio;}
	lda::resource_manager&					get_audio_resource_manager() {return *audio_resource_manager;}
	ldv::resource_manager&					get_video_resource_manager() {return *video_resource_manager;}
	lm::logger&								get_log() {return *logger;}

	void									set_env(const app::env& _val) {env=&_val;}
	void 									set_input(dfw::input& _val) {input=&_val;}
	void 									set_config(dfwimpl::config& _val) {config=&_val;}
	void 									set_audio(dfw::audio& _val) {audio=&_val;}
	void 									set_audio_resource_manager(lda::resource_manager& _val) {audio_resource_manager=&_val;}
	void 									set_video_resource_manager(ldv::resource_manager& _val) {video_resource_manager=&_val;}
	void 									set_logger(lm::logger& _val) {logger=&_val;}
	void 									set_screen(ldv::screen& _val) {screen=&_val;}

	private:

	//managed deps. These should not be initialized to nullptr, that would call
	//reset, which would be forbidden because the type is incomplete.
	std::unique_ptr<app::resources>			resources;
	std::unique_ptr<tools::i8n>				i8n;
	std::unique_ptr<ldtools::ttf_manager>	ttf_manager;
	std::unique_ptr<app::hi_score_manager>	hi_score_manager;
	std::unique_ptr<app::score>				score;
	std::unique_ptr<app::layout>			layout;

	//injected deps.
	app::env const *						env{nullptr};
	dfw::input*								input{nullptr};
	dfwimpl::config*						config{nullptr};
	dfw::audio*								audio{nullptr};
	lda::resource_manager*					audio_resource_manager{nullptr};
	ldv::resource_manager*					video_resource_manager{nullptr};
	lm::logger*								logger{nullptr};
	ldv::screen const *						screen{nullptr};
};

}

