#include <app/game.h>
#include <app/space.h>
#include <app/order.h>
#include <app/world_reader.h>
#include <app/order_generator.h>
#include <app/log.h>
#include <app/order_checker.h>

#include <tools/number_generator.h>
#include <lm/sentry.h>
#include <iostream>
#include <vector>

using namespace app;

game::game(
	lm::logger& _logger
):
	log{_logger},
	world_instance{0,0},
	player_instance{0,0,0,0},
	bar_instance{0,0,0,0,0},
	trash_instance{0,0,0,0,0}
{

}

void game::init(
	const std::string& _path
) {

	tables.clear();
	stages.clear();

	app::world_reader wr;

	int score_consumable{0}, score_trash{0};

	wr.read(
		_path, 
		world_instance, 
		player_instance, 
		bar_instance, 
		trash_instance, 
		tables, 
		stages,
		game_seconds,
		score_consumable,
		score_trash
	);

	player_score.set_per_consumable(score_consumable);
	player_score.set_per_trash(score_trash);


	reset();
}

void game::tick(
	float _delta
) {

	if(current_mode != modes::game_over) {

		current_game_seconds+=_delta;

		if(current_game_seconds > game_seconds) {

			std::cout<<"game over"<<std::endl;
			game_over();
			return;
		}

		if(current_game_seconds > stages[current_stage].get_until() && current_stage != stages.size() -1) {

			log<<lm::debug<<"advancing to next stage "<<std::endl;
			advance_stage();
		}

		tick_tables(_delta);
	}

	switch(current_mode) {

		case modes::movement: tick_movement(_delta); break;
		case modes::fill_tray: tick_fill_tray(_delta); break;
		case modes::serve: tick_serve(_delta); break;
		case modes::take_order: tick_take_order(_delta); break;
		case modes::game_over: tick_game_over(_delta); break;
	}
}

void game::tick_tables(
	float _delta
) {

	for(auto& table : tables) {

		table.tick(_delta);
		//empty table, ready to roll chances...
		if(table.can_roll_for_customer()) {

			roll_table(table);
			continue;
		}

		//customer finally arrived at table.
		if(table.is_customer_arrived()) {

			log<<lm::debug<<table<<" will demand attention"<<std::endl;
			//TODO: should this value be fixed???
			table.set_to_demand_attention(20.f);
			continue;
		}

		//customer got tired of waiting for attention / order and leaves.
		if(table.is_customer_tired_of_waiting()) {

			//if the customer had something, the table is left dirty
			//TODO: fixed value??
			log<<lm::debug<<table<<" got tired of waiting and will leave"<<std::endl;
			table.set_to_customer_leaving(10.f);
			continue;
		}

		if(table.is_done_with_order()) {

			table_done_with_order(table);
			continue;
		}

		if(table.has_customer_left()) {

			log<<lm::debug<<table<<" is now free"<<std::endl;
			table.set_free();
			continue;
		}
	}
}

void game::table_done_with_order(
	app::table& _table
) {

	if(!_table.has_orders_left()) {

		log<<lm::debug<<_table<<" is done with order, will leave"<<std::endl;
		//TODO: fixed???
		_table.set_to_customer_leaving(4.f);
		return;
	}

	log<<lm::debug<<_table<<" is done with order, demands next one"<<std::endl;
	//TODO: fixed???
	_table.set_to_demand_attention(30.f);
}

void game::roll_table(
	app::table& _table
) {

	const auto& stage=stages[current_stage];

	//TODO: debug these!!!

	int occupied_count=std::count_if(
		std::begin(tables),
		std::end(tables),
		[](const table& _ctable) {

			return _ctable.is_consuming()
				|| _ctable.is_waiting_order()
				|| _ctable.is_demanding_attention()
				|| _ctable.is_customer_arriving();
		}
	);
	int dirty_count=std::count_if(
		std::begin(tables),
		std::end(tables),
		[](const table& _ctable) {
			return _ctable.is_free() && _ctable.is_dirty();
		}
	);

	auto chance=stage.calculate_chance(occupied_count, dirty_count);
	auto gen=tools::int_generator(0, 100);
	auto val=gen();

	log<<lm::debug<<_table<<" rolled "<<val<<"/"<<chance<<" (stage:"<<current_stage<<", dirty:"<<dirty_count<<", occupied:"<<occupied_count<<")"<<std::endl;

	if(val < chance) {

		log<<lm::debug<<_table<<" will get customer"<<std::endl;

		//calculate the order now...
		//first, a random timer for how much the customer takes to arrive,
		auto time_gen=tools::int_generator(3000, 8000);
		float customer_arrival_time=(float)time_gen() / 1000.f;
		log<<lm::debug<<"customer arrival time set at "<<customer_arrival_time<<std::endl;

		//We also need to set the orders for this table...
		//TODO: std::move this too?
		app::order_generator ordgen;
		auto orders=ordgen.generate(
			stage.get_min_orders(),
			stage.get_max_orders(),
			stage.get_min_consumables(),
			stage.get_min_consumables()
		);

		log<<lm::debug<<"will get "<<orders.size()<<" orders"<<std::endl;
		std::stringstream ss;
		for(const auto& o : orders) {

			log<<lm::debug<<o<<std::endl;
		}

		_table.set_to_customer_arrival(customer_arrival_time, std::move(orders));
		return;
	}

	//reset the timer...
	_table.set_timer(stages[current_stage].get_customer_cooloff());
}

void game::tick_movement(
	float _delta
) {

	player_get_input();
	player_instance.tick(_delta);
	player_motion_phase(_delta);

	//Can the player interact with anything?
	evaluate_interactions();

	if(game_input.interact) {

		process_interactions();
	}
}

void game::tick_game_over(
	float _delta
) {

	state_time_counter-=_delta;

	if(game_input.interact && state_time_counter <= 0.f) {

		reset();
		return;
	}
}

void game::tick_fill_tray(
	float /*_delta*/
) {

	if(game_input.select_left) {

		bar_selector_instance.prev();
		return;
	}
	
	if(game_input.select_right) {

		bar_selector_instance.next();
		return;
	}

	if(game_input.select_down) {

		if(player_tray.is_full()) {

			return;
		}

		player_tray.add({bar_selector_instance.get()});
	}
	
	if(game_input.select_up) {

		player_tray.pop();
		return;
	}

	if(game_input.interact) {

		current_mode=modes::movement;
		return;
	}
}

void game::tick_take_order(
	float _delta
) {

	state_time_counter-=_delta;

	//The state time counter must run out before we can go again.
	if(game_input.interact && state_time_counter <= 0.f) {

		log<<lm::debug<<*current_table<<" wait for the order to arrive"<<std::endl;

		//TODO: Should this be fixed???
		current_table->set_to_wait_for_order(30.f);
		current_mode=modes::movement;
	}
}

void game::tick_serve(
	float
) {
	//Change selection
	if(game_input.select_left) {

		player_tray.prev();
		return;
	}
	
	//Change selection
	if(game_input.select_right) {

		player_tray.next();
		return;
	}

	//Retrieve all...
	if(game_input.select_up) {

		for(const auto& prod : table_serving.get()) {

			player_tray.add(prod);
		}

		table_serving.reset();
		return;
	}
	
	//Serve current...
	if(game_input.select_down && player_tray.size()) {

		auto prod=player_tray.remove_current();
		table_serving.add(prod);
		return;
	}

	if(game_input.interact) {
	
		!table_serving.size()
			? cancel_serve()
			: confirm_serve();
		return;
	}
}

void game::cancel_serve() {

	log<<lm::debug<<*current_table<<" serving cancelled, returning to movement mode"<<std::endl;
	current_mode=modes::movement;
}

void game::confirm_serve() {

	log<<lm::debug<<*current_table<<" confirming serve, a comparison will take place"<<std::endl;

	order_checker checker;
	auto check_result=checker.check(
		table_serving.get(),
		current_table->get_current_order().products
	);

	log<<lm::debug<<"ok: "<<check_result.ok
		<<", missing: "<<check_result.missing
		<<", failed: "<<check_result.failed<<std::endl;

	//if the order had at least no good products...
	if(!check_result.ok) {

		log<<lm::debug<<*current_table<<" wrong order, will skip or leave, will lose time"<<std::endl;
		//TODO: should be a fixed value????
		current_game_seconds+=15.f;
		table_done_with_order(*current_table);
	}
	else {

		current_table->serve();
		player_score.score_consumables(check_result.ok, check_result.missing, check_result.failed);
		log<<lm::debug<<*current_table<<" will start consuming"<<std::endl;
		current_table->set_to_consuming(30.f);
	}

	//Whatever happens, reset stuff.
	table_serving.reset();
//	current_table->get_current_order().reset();

	log<<lm::debug<<"returning to movement order"<<std::endl;
	current_mode=modes::movement;
}

void game::evaluate_interactions() {

	current_table=nullptr;

	const auto& player_box=player_instance.get_collision_box();
	current_interaction_type=interaction_types::none;

	if(
		bar_instance.get_interaction_box().collides_with(player_box)
		&& !player_tray.has_trash()
	) {

		current_interaction_type=player_tray.is_empty()
			? interaction_types::pick_consumables
			: interaction_types::empty_tray;

		return;
	}

	if(
		trash_instance.get_interaction_box().collides_with(player_box)
		&& player_tray.has_trash()
	) {

		current_interaction_type=interaction_types::drop_trash;
		return;
	}
	

	if(!player_tray.has_trash()) {

		for(auto& table : tables) {

			if(!table.can_be_interacted_with()) {

				continue;
			}

			if(table.get_interaction_box().collides_with(player_box)) {
	
				if(table.is_demanding_attention() && player_tray.is_empty()) {

					current_table=&table;
					current_interaction_type=interaction_types::take_order;
					return;
				}

				if(table.is_waiting_order() && !player_tray.is_empty()) {

					current_table=&table;
					current_interaction_type=interaction_types::serve_order;
					return;
				}

				if(table.is_dirty() && player_tray.is_empty()) {

					current_table=&table;
					current_interaction_type=interaction_types::clean_table;
					return;
				}
			}
		}
	}
}

void game::process_interactions() {

	switch(current_interaction_type) {

		case game::interaction_types::none:

			return;
		break;
		case game::interaction_types::pick_consumables:

			current_mode=modes::fill_tray;
			bar_selector_instance.reset();
			return;
		break;
		case game::interaction_types::empty_tray:

			player_tray.empty();
			return;
		break;
		case game::interaction_types::take_order: 

			log<<lm::debug<<"popping order of table "<<*current_table<<std::endl;
			current_table->pop_order();
			log<<lm::debug<<"will enter take order mode"<<std::endl;
			current_mode=modes::take_order;
			//TODO: Should be fixed const?
			state_time_counter=0.5f;
			return;
		break;
		case game::interaction_types::serve_order:

			log<<lm::debug<<*current_table<<" will enter serve mode"<<std::endl;
			current_mode=modes::serve;
			table_serving.reset();
			return;
		break;
		case game::interaction_types::clean_table:

			player_tray.fill_with_trash();
			current_table->clean();
			return;
		break;
		case game::interaction_types::drop_trash:

			drop_trash();
			return;
		break;
	}
}

void game::drop_trash() {

	player_tray.drop_trash();
	player_score.score_trash();
}

std::optional<app::box> game::collision_detection() {

	const auto& player_box=player_instance.get_collision_box();

	//Check the player is inside the world borders...
	if(!player_box.inside(world_instance.get_collision_box())) {

		return {world_instance.get_collision_box()};
	}

	//Check the player does not collide with any tables...
	for(const auto& table : tables) {

		if(table.get_collision_box().collides_with(player_box)) {

			return {table.get_collision_box()};
		}
	}

	//Check the player does not collide with the bar or trash
	if(bar_instance.get_collision_box().collides_with(player_box)) {

			return {bar_instance.get_collision_box()};
	}

	if(trash_instance.get_collision_box().collides_with(player_box)) {

			return {trash_instance.get_collision_box()};
	}

	return {};
}

void game::player_get_input() {

	double motion_x=0.;
	if(game_input.walk_left) {

		motion_x=-1.;
	}
	else if(game_input.walk_right) {

		motion_x=1.;
	}

	double motion_y=0.;
	if(game_input.walk_down) {

		motion_y=-1.;
	}
	else if(game_input.walk_up) {

		motion_y=1.;
	}

	player_instance.set_motion(motion_x, motion_y);
}

void game::player_motion_phase(
	float _delta
) {

	player_instance.backup_position();

	player_instance.apply_motion(app::axes::x, _delta);
	if(const auto box=collision_detection()) {

		collision_response(app::axes::x, box.value());
	}

	player_instance.apply_motion(app::axes::y, _delta);
	if(const auto box=collision_detection()) {

		collision_response(app::axes::y, box.value());
	}
}

void game::collision_response(
	app::axes _axis, 
	const app::box& _box
) {

	//TODO: Clean this up with some function pointer...

	//Special case...
	if(_box==world_instance.get_collision_box()) {

		auto& box=player_instance.ref_collision_box();
		auto edge=ldt::box_edges::right; //Any value will do.
	
		switch(_axis) {

			case app::axes::x:

				edge=box.origin.x+box.w >= _box.origin.x+_box.w 
					? ldt::box_edges::right
					: ldt::box_edges::left;
			break;
			case app::axes::y:

				edge=box.origin.y+box.h >= _box.origin.y+_box.h 
					? ldt::box_edges::top
					: ldt::box_edges::bottom;
			break;
		}

		ldt::match_edge(box, _box, edge);
		return;
	}

	switch(_axis) {

		case app::axes::x:
			ldt::snap_to_edge(
				player_instance.ref_collision_box(),
				_box,
				app::is_left_of(player_instance.get_previous_box(), _box)
					? ldt::box_edges::right
					: ldt::box_edges::left
			);

		break;
		case app::axes::y:

			ldt::snap_to_edge(
				player_instance.ref_collision_box(),
				_box,
				app::is_below(player_instance.get_previous_box(), _box)
					? ldt::box_edges::top
					: ldt::box_edges::bottom
			);
		break;
	}
}

void game::advance_stage() {

	//TODO:
	++current_stage;
}

void game::reset() {

	current_stage=0;
	current_game_seconds=0.f;
	current_table=nullptr;
	current_mode=modes::movement;

	//reset tables...
	auto table_time=stages[current_stage].get_customer_cooloff();
	for(auto& table : tables) {

		table.reset();
		table.set_timer(table_time);
	}
	
	table_serving.reset();
	player_instance.reset();
	player_tray.reset();
	player_score.reset();
	bar_selector_instance.reset();
	current_interaction_type=interaction_types::none;
}

void game::game_over() {

	state_time_counter=5.f;
	current_mode=modes::game_over;
}
