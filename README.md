Berzerk game.

~ Added gitignore whitelist;
~ Barebones renderer added, D3D11 w/ shaders.
~ Added functionality:
	- Direct3D 11 line object rendering, load from ASE files or created in-code.
	- Input module, allowing binding of actions to keys, and listening for actions
	- Configuration module, loading text configuration files to allow variables to be changed without re-compiling.
	- Working timer class.
~ See Startup.cpp for usage of various bits.
~ Note that Display class still contains testing functionality, requires limited refactoring for clarity.
~ Note that AsteroidGen class is WIP, just messing around a bit there.