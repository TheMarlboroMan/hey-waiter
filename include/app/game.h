#pragma once

#include "world.h"
#include "player.h"
#include "table.h"
#include "bar.h"
#include "trash.h"
#include "tray.h"
#include "bar_selector.h"
#include "input.h"
#include "loop_stage.h"

#include <vector>
#include <optional>

namespace app {

class draw;

class game {

	public:

					game();
	void			init(const std::string&);
	void			tick(float _delta);
	void			set_input(app::input& _i) {game_input=std::move(_i);}
	const world&	get_world() const {return world_instance;}

	private:

	enum class interaction_types {
		none,
		pick_consumables, //empty tray, pick stuff at bar
		empty_tray, //full tray, empty it at bar
		take_order, //empty tray, take customer order at a table
		serve_order, //full tray, serve customer order at a table,
		clean_table, //empty tray, clean messy table
		drop_trash //drop table refuse at trash
	};

	enum class modes {
		movement,
		fill_tray,
		serve
	};

	void			tick_movement(float _delta);
	void			tick_fill_tray(float _delta);
	void			tick_serve(float _delta);
	void			player_get_input();
	void			player_motion_phase(float);
	std::optional<app::box>	collision_detection();
	void			collision_response(app::axes, const app::box&);
	void			evaluate_interactions();
	void			process_interactions();
	
	modes			current_mode{modes::movement};
	app::world		world_instance;
	app::player		player_instance;
	app::bar		bar_instance;
	app::trash		trash_instance;
	std::vector<table>	tables;
	std::vector<loop_stage>	stages;
	app::tray		player_tray;
	app::input		game_input;
	app::bar_selector bar_selector_instance;
	interaction_types current_interaction_type{interaction_types::none};
	app::table *	current_table{nullptr};
	int				game_seconds{0};

	friend class 	draw; //Yeah that's right!
};

}
