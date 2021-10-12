# hey-waiter
A table-waiting game.

# TODO:

- Bar / Table / Trash bitmap lights up to indicate interaction!!!
	- Choose:
		- it lights up
		- or a border appears
			- I kind of like the border thing. which would be a separate 
			graphic.
- Graphics
	- Bar
	- Table
	- Trash
	- Player
- Draw world with depth:
	- This is going to be... problematic, I guess, unless we create a separate
		entity with separate pointers to the table, player, trash and shit...
		maybe they all share the same base, but I'd rather do it through
		composition, if possible... Not sure though, table info (like the
		customer type and shit) should be stored in this other entity, right???
- Add music
- Add sounds
- Help controller explaining the sytems

# BUGS:

- Some bug shows up on serving and sigsevs

# Credits:

Font Origami Mommy by Tepid Monkey Fonts (https://www.1001fonts.com/users/tepidmonkey/)
The font was modified so all characters had the same width.
Music is "Happy Adventure" by TinyWorlds at opengameart.org
