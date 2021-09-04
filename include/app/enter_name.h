#pragma once
#include <app/input.h>
#include <string>

namespace app {

class enter_name {

	public:

								enter_name();
	void						set_input(app::input& _i) {game_input=std::move(_i);}
	void						tick(float);
	void						reset();
	bool						is_finished() const {return finished;}
	const std::string			get_player_name() const {return player_name;}
	int							get_current_position() const {return current_position;}

	private:

	const std::string			acceptable;
	std::string					player_name;
	std::size_t					current_position{0},
								current_index{0};
	app::input					game_input;
	bool						finished{false};
};

}
