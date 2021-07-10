#include <app/world_reader.h>

#include <tools/string_utils.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace app;

void world_reader::read(
	const std::string& _path,
	app::world& _world,
	app::player& _player,
	app::bar& _bar,
	app::trash& _trash,
	std::vector<table>& _tables
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
		else {

			throw std::runtime_error(std::string{"world_reader cannot interpret line '"}+line+"'");
		}
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

	int x{0}, y{0};

	_ss>>x>>y;

	if(_ss.fail()) {

		throw std::runtime_error("failed to read table position");
	}

	_tables.push_back({x, y, table_w, table_h, table_margin});
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
