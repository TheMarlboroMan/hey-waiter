#pragma once

#include <app/env.h>

#include <string>
#include <vector>

namespace app {

struct hi_score {

					hi_score(int, const std::string&);
					hi_score() {}

	int				score{0};
	std::string		name;
	bool			operator<(const hi_score& _other) const {

		return _other.score < score;
	}
};

class hi_score_manager {

	public:

								hi_score_manager(const app::env&);
	void						submit(const hi_score&);
	bool						can_be_submitted(int) const;
	const std::vector<hi_score>& get() const {return current_scores;}

	private:

	void						save();
	void						load();
	std::string					get_filename() const;

	const app::env&				env;
	std::vector<hi_score>		current_scores;
};

}
