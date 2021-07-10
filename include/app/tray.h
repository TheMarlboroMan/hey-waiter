#pragma once

#include <app/consumable.h>

#include <vector>

namespace app {

/** 
* the tray is whatever is carried on the player's inventory
**/
class tray {

	public:

								tray();
	bool						is_empty() const {return contents.empty();}
	bool						has_trash() const {return trash;}
	void						fill_with_trash() {trash=true;}
	void						drop_trash() {trash=false;}
	void						empty() {contents.clear();}
	void						pop();
	bool						is_full() const {return size()==max_size;}
	void						add(consumable _val) {contents.push_back(_val);}
	const std::vector<consumable> get() const {return contents;}
	std::size_t					size() const {return contents.size();}

	private:

	static const std::size_t max_size{8};
	std::vector<consumable>		contents;
	bool						trash{false};
};

}
