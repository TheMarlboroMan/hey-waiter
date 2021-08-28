#pragma once

#include "world.h"
#include "player.h"
#include "table.h"
#include "bar.h"
#include "trash.h"
#include "tray.h"
#include "serving.h"
#include "bar_selector.h"
#include "input.h"
#include "loop_stage.h"
#include "score.h"

#include <lm/logger.h>
#include <vector>
#include <optional>

namespace app {

class draw;

class game {

	public:

					game(lm::logger&);
	void			init(const std::string&);
	void			tick(float _delta);
	void			set_input(app::input& _i) {game_input=std::move(_i);}
	const world&	get_world() const {return world_instance;}
	void			reset();

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
		take_order,
		fill_tray,
		serve,
		game_over
	};

	void			tick_tables(float);
	void			tick_movement(float);
	void			tick_fill_tray(float);
	void			tick_serve(float);
	void			tick_take_order(float);
	void			tick_game_over(float);
	void			roll_table(app::table&);
	void			player_get_input();
	void			player_motion_phase(float);
	std::optional<app::box>	collision_detection();
	void			collision_response(app::axes, const app::box&);
	void			evaluate_interactions();
	void			process_interactions();
	void			advance_stage();
	void			game_over();
	void			confirm_serve();
	void			cancel_serve();
	void			drop_trash();
	void			table_done_with_order(app::table&);

	lm::logger&		log;

	modes			current_mode{modes::movement};
	app::world		world_instance;
	app::player		player_instance;
	app::bar		bar_instance;
	app::trash		trash_instance;
	std::vector<table>	tables;
	std::vector<loop_stage>	stages;
	app::tray		player_tray;
	app::serving	table_serving;
	app::input		game_input;
	app::bar_selector bar_selector_instance;
	app::score		player_score;
	interaction_types current_interaction_type{interaction_types::none};
	app::table *	current_table{nullptr};
	int				game_seconds{0}; //<!Max allowed time.
	std::size_t		current_stage{0};	
	float			current_game_seconds{0.f}, //<!Current time
					state_time_counter{0.f}; //<! Counter for time-bound states.

	friend class 	draw; //Yeah that's right!
};

}
