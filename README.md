# hey-waiter

A table-waiting game.

# TODO:
- Design HUD for serving and taking shit from the bar.
	- Complete the layout for this.
	- Make the view composer be able to render stuff from a different "origin",
		as in draw(screen, point origin),
		draw(screen, camera, origin);
		auto old_pos=item->get_position();
		auto new_pos=old_pos+=origin;
		item->go_to(new_pos)
		item->draw(...)
		item->go_to(old_pos)

- Placeholder Graphics and sprite tables
	- Player
	- Customers
	- HUD consumables
	- Consumables in player tray and table

- Definitive graphics for
	- Bar
	- Table
	- Trash
	- Player
	- Customers
	- HUD consumables
	- Consumables in player tray and table
- Add music
- Add sounds
- Help controller explaining the sytems

# BUGS:

- Some bug shows up on serving and sigsevs

# Credits:

Font Origami Mommy by Tepid Monkey Fonts (https://www.1001fonts.com/users/tepidmonkey/)
The font was modified so all characters had the same width.
Music is "Happy Adventure" by TinyWorlds at opengameart.org
