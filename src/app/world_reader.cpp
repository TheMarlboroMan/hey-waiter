#include <app/world_reader.h>

#include <tools/string_utils.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace app;

void world_reader::read(
	const std::string& _path,
	app::world& _world,
	app::player& _player,
	app::bar& _bar,
	app::trash& _trash,
	std::vector<table>& _tables,
	std::vector<loop_stage>& _stages,
	int& _game_length
) {

	std::ifstream infile(_path);

	if(!infile.is_open()) {

		throw std::runtime_error("could not open game config file");
	}

	std::string line, word;

	while(true) {

		std::getline(infile, line);

		if(infile.eof()) {

			break;
		}

		tools::trim(line);

		if(!line.size()) {

			continue;
		}

		if(line[0]=='#') {

			continue;
		}

		std::stringstream ss{line};
		ss>>word;

		if(word=="world") {

			read_as_world(ss, _world);
		}
		else if(word=="table_margin") {

			read_as_table_margin(ss);
		}
		else if(word=="player") {

			read_as_player(ss, _player);
		}
		else if(word=="player_speed") {

			read_as_player_speed(ss, _player);
		}
		else if(word=="table_size") {

			read_as_table_size(ss);
		}
		else if(word=="table") {

			read_as_table(ss, _tables);
		}
		else if(word=="bar") {

			read_as_bar(ss, _bar);
		}
		else if(word=="trash") {

			read_as_trash(ss, _trash);
		}
		else if(word=="game_length") {

			_game_length=read_as_game_length(ss);
		}
		else if(word=="loop") {

			read_as_loop_stage(ss, _stages);
		}
		else {

			throw std::runtime_error(std::string{"world_reader cannot interpret line '"}+line+"'");
		}
	}

	if(!_stages.size()) {

		throw std::runtime_error("no game loop entries!");
	}

	//sort loop entries.
	std::sort(
		std::begin(_stages), 
		std::end(_stages),
		[](const loop_stage& _a, const loop_stage& _b) {
			return _a.get_until() < _b.get_until();
		}
	);

	//make sure the times do not overlap.
	std::vector<int> untils;
	std::transform(
		std::begin(_stages),
		std::end(_stages),
		std::back_inserter(untils),
		[](const loop_stage& _item) {return _item.get_until();}
	);

	untils.erase(
		std::unique(std::begin(untils), std::end(untils)),
		std::end(untils)
	);

	if(untils.size() != _stages.size()) {

		throw std::runtime_error("repeated 'until' in loop entries!");
	}
}

//We don't actually need to pass the whole line, as the stringstream is still
//intact... in any case this seems more... testable.
void world_reader::read_as_world(
	std::stringstream& _ss,
	world& _world
) {

	int w{0}, h{0};
	_ss>>w>>h;
	
	if(_ss.fail()) {

		throw std::runtime_error("failed to read world line");
	}

	_world.set_size(w, h);
}

void world_reader::read_as_table_margin(
	std::stringstream& _ss
) {
	_ss>>table_margin;

	if(_ss.fail()) {

		throw std::runtime_error("failed to read table margin line");
	}
}

void world_reader::read_as_player(
	std::stringstream& _ss,
	app::player& _player
) {

	int x{0}, y{0}, w{0}, h{0};
	_ss>>x>>y>>w>>h;
	
	if(_ss.fail()) {

		throw std::runtime_error("failed to read world line");
	}

	_player.set_position(x, y);
	_player.set_size(w, h);
}

void world_reader::read_as_table_size(
	std::stringstream& _ss
) {

	_ss>>table_w>>table_h;

	if(_ss.fail()) {

		throw std::runtime_error("failed to read table size line");
	}
}

void world_reader::read_as_player_speed(
	std::stringstream& _ss,
	app::player& _player
) {

	double player_speed{};
	_ss>>player_speed;

	if(_ss.fail()) {

		throw std::runtime_error("failed to read player speed");
	}

	_player.set_speed(player_speed);
}

void world_reader::read_as_table(
	std::stringstream& _ss,
	std::vector<table>& _tables
) {

	if(-1==table_margin || -1==table_w || -1==table_h) {

		throw std::runtime_error("cannot read table without reading margin and size first");
	}

	int id=_tables.size()+1;
	int x{0}, y{0};
	char direction{0};

	_ss>>x>>y>>direction;

	if(_ss.fail()) {

		throw std::runtime_error("failed to read table position");
	}

	directions customer_direction{directions::left};
	
	switch(direction) {
		case 'L': customer_direction=directions::left; break;
		case 'R': customer_direction=directions::right; break;
		default:
			throw std::runtime_error("bad table direction");
	}

	_tables.push_back({id, x, y, table_w, table_h, table_margin, customer_direction});
}

void world_reader::read_as_bar(
	std::stringstream& _ss,
	app::bar& _bar
) {

	int x{0}, y{0}, w{0}, h{0}, margin{0};

	_ss>>x>>y>>w>>h>>margin;

	if(_ss.fail()) {

		throw std::runtime_error("failed to read bar position");
	}

	_bar=app::bar(x, y, w, h, margin);
}

void world_reader::read_as_trash(
	std::stringstream& _ss,
	app::trash& _trash
) {

	int x{0}, y{0}, w{0}, h{0}, margin{0};

	_ss>>x>>y>>w>>h>>margin;

	if(_ss.fail()) {

		throw std::runtime_error("failed to read trash position");
	}

	_trash=app::trash(x, y, w, h, margin);
}

int world_reader::read_as_game_length(
	std::stringstream& _ss
) {

	int length{0};

	_ss>>length;

	if(_ss.fail()) {

		throw std::runtime_error("failed to read game length");
	}

	if(length <= 0) {

		throw std::runtime_error("game length cannot be zero or negative");
	}

	return length;
}

void world_reader::read_as_loop_stage(
	std::stringstream& _ss, 
	std::vector<loop_stage>& _stages
) {

	int until{0}, chance{0}, cooloff{0}, min_orders{0},
		max_orders{0}, min_consumables{0}, max_consumables{0};

	_ss>>until>>chance>>cooloff>>min_orders>>max_orders>>min_consumables>>max_consumables;

	if(_ss.fail()) {

		throw std::runtime_error("failed to read game loop");
	}

	if(until <= 0) {

		throw std::runtime_error("game loop time cannot be zero or negative");
	}

	if(chance < 0 || chance > 100) {

		throw std::runtime_error("game loop chance must be between 0 and 100");
	}

	if(cooloff <= 0) {

		throw std::runtime_error("game loop cooloff cannot be zero or negative");
	}

	if(min_orders <= 0) {

		throw std::runtime_error("game loop min_orders cannot be zero or negative");
	}

	if(max_orders <= 0) {

		throw std::runtime_error("game loop max_orders cannot be zero or negative");
	}

	if(max_orders < min_orders) {

		throw std::runtime_error("game loop max_orders cannot be lesser than min_orders");
	}

	if(min_consumables <= 0) {

		throw std::runtime_error("game loop min_consumables cannot be zero or negative");
	}

	if(max_consumables <= 0) {

		throw std::runtime_error("game loop max_consumables cannot be zero or negative");
	}

	if(max_consumables < min_consumables) {

		throw std::runtime_error("game loop max_consumables cannot be lesser than min_consumables");
	}

	_stages.push_back({
		until, 
		chance, 
		cooloff, 
		min_orders,
		max_orders, 
		min_consumables, 
		max_consumables
	});
}
