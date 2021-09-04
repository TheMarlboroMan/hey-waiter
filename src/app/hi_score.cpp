#include <app/hi_score.h>

#include <fstream>
#include <sstream>
#include <algorithm>

using namespace app;

hi_score::hi_score(
	int _score,
	const std::string& _name
):
	score{_score},
	name{_name} {

}

hi_score_manager::hi_score_manager(
	const app::env& _env
):
	env{_env} {

	load();
}

void hi_score_manager::submit(const hi_score& _item) {

	current_scores.push_back(_item);
	std::sort(std::begin(current_scores), std::end(current_scores));

	if(current_scores.size() > max_size) {

		current_scores.resize(max_size);
	}

	save();
}

void hi_score_manager::save() {

	std::ofstream file(get_filename());

	for(const auto& item : current_scores) {

		file<<item.score<<" "<<item.name<<std::endl;
	}
}

void hi_score_manager::load() {

	current_scores.clear();

	std::ifstream file(get_filename());

	if(!file) {

		throw std::runtime_error("no hi score table found");
	}

	std::string buff;

	while(true) {

		std::getline(file, buff);
		if(file.eof()) {

			break;
		}

		if(!buff.size()) {
	
			continue;
		}

		int score{0};
		std::string name;

		std::stringstream ss{buff};
		ss>>score>>name;

		if(ss.fail()) {

			throw std::runtime_error("failed to read hi score table");
		}

		current_scores.push_back({score, name});
	}

	if(current_scores.size() > 10) {

		current_scores.resize(10);
	}
}

std::string hi_score_manager::get_filename() const {

	return env.build_user_path("hi_scores.dat");
}

bool hi_score_manager::can_be_submitted(
	int _score
) const {

	if(_score==0) {

		return false;
	}

	if(current_scores.size() < 10) {

		return true;
	}

	for(const auto& score : current_scores) {

		if(_score > score.score) {

			return true;
		}
	}

	return false;
}
