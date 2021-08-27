#include <app/score.h>

#include <cmath>

using namespace app;

int score::score_consumables(
	int _quantity,
	int /*_missing*/,
	int _failed
) {

	//base: given products * quantity.
	double base=_quantity * per_consumable;

	//for each product that was not in the order, remove a %.
	//TODO: should this be a fixed quantity?

	double percent_removed_per_failure=5.;
	double total_percent=percent_removed_per_failure * _failed;

	double score_removal=base * (total_percent / 100);
	int result=ceil(base - score_removal);

	score+=result;
	return result;
}

int score::score_trash() {

	score+=per_trash;
	return per_trash;
}

void score::reset() {

	score=0;
}


