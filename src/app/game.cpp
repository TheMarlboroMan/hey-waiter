#include <app/game.h>
#include <app/space.h>
#include <app/world_reader.h>

#include <iostream>

using namespace app;

game::game():
	world_instance{0,0},
	player_instance{0,0,0,0},
	bar_instance{0,0,0,0,0},
	trash_instance{0,0,0,0,0}
{

}

void game::init(
	const std::string& _path
) {

	app::world_reader wr;
	wr.read(
		_path, 
		world_instance, 
		player_instance, 
		bar_instance, 
		trash_instance, 
		tables, 
		stages,
		game_seconds
	);
}

void game::tick(
	float _delta
) {

	//TODO: Tick the tables, of course.

	switch(current_mode) {

		case modes::movement: tick_movement(_delta); break;
		case modes::fill_tray: tick_fill_tray(_delta); break;
		case modes::serve: tick_serve(_delta); break;
	}
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

		current_mode=modes::movement;
		return;
	}
	
	if(game_input.select_up) {

		player_tray.pop();
		return;
	}


	if(game_input.interact) {

		if(player_tray.is_full()) {

			return;
		}

		player_tray.add({bar_selector_instance.get()});
	}
}

void game::tick_serve(
	float
) {
	//TODO:
}
void game::evaluate_interactions() {

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

		for(const auto& table : tables) {

			if(!table.can_be_interacted_with()) {

				continue;
			}

			if(table.get_interaction_box().collides_with(player_box)) {
	
				if(table.is_demanding_attention() && player_tray.is_empty()) {

					current_interaction_type=interaction_types::take_order;
					return;
				}

				if(table.is_waiting_order() && !player_tray.is_empty()) {

					current_interaction_type=interaction_types::serve_order;
					return;
				}

				if(table.is_dirty() && player_tray.is_empty()) {

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

			//TODO: 

		break;
		case game::interaction_types::serve_order:

			//TODO: 
		break;
		case game::interaction_types::clean_table:

			player_tray.fill_with_trash();
			return;
		break;
		case game::interaction_types::drop_trash:

			player_tray.drop_trash();
			return;
		break;
	}
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
