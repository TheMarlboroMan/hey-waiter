#pragma once

#include <app/world.h>
#include <app/player.h>
#include <app/table.h>
#include <app/bar.h>
#include <app/trash.h>
#include <app/loop_stage.h>

#include <sstream>
#include <string>
#include <vector>

namespace app {

class world_reader {

	public:

	void			read(const std::string&, 
		world&, 
		player&, 
		bar&, 
		trash&, 
		std::vector<table>&,
		std::vector<loop_stage>&,
		int&
	);

	private:

	void			read_as_world(std::stringstream&, world&);
	void			read_as_table_margin(std::stringstream&);
	void			read_as_table_size(std::stringstream&);
	void			read_as_player_speed(std::stringstream&, player&);
	void			read_as_table(std::stringstream&, std::vector<table>&);
	void			read_as_player(std::stringstream&, player&);
	void			read_as_table(std::stringstream&);
	void			read_as_bar(std::stringstream&, bar&);
	void			read_as_trash(std::stringstream&, trash&);
	int				read_as_game_length(std::stringstream&);
	void			read_as_loop_stage(std::stringstream&, std::vector<loop_stage>&);

	int 			table_margin{-1},
					table_w{-1},
					table_h{-1};
};

}
