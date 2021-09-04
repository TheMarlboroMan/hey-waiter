#include <app/enter_name.h>

using namespace app;

enter_name::enter_name(

)
	:acceptable{" ABCDEFGHIJKLMNOPQRSTUVWXYZ-_!?"}, //I would love to include a space and symbols, but it is not monospaced
	player_name("          ")
{

}

void enter_name::reset() {

	player_name="          ";
	current_position=0;
	current_index=0;
	finished=false;
}

void enter_name::tick(
	float /*_delta*/
) {

	auto recalculate_current_index=[this]() {

		auto curchar=player_name[current_position];
		current_index=acceptable.find(curchar, 0);
	};

	if(game_input.select_left) {

		if(0==current_position) {

			return;
		}

		--current_position;
		recalculate_current_index();
		return;
	}
	
	if(game_input.select_right) {

		if(current_position==player_name.size()-1) {
			return;
		}

		++current_position;
		recalculate_current_index();
		return;
	}

	if(game_input.select_down) {

		if(0==current_index) {
			current_index=acceptable.size()-1;
		}
		else {
			--current_index;
		}
		player_name[current_position]=acceptable[current_index];
		return;
	}
	
	if(game_input.select_up) {

		++current_index;
		if(current_index==acceptable.size()) {
			current_index=0;
		}
		player_name[current_position]=acceptable[current_index];
		return;
	}

	if(game_input.interact) {

		finished=true;
		return;
	}
}
