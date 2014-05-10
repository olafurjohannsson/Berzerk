Berzerk game.


Changelog 09/07/2014 [Cathode]
~ Added functionality:
	- Sample implementation of Actor base class
	- RigidBody class for physical interactions, collision detection
	- LineObject interfaces for manipulating specific elements
	- Skeleton class allowing synchronization of animation between physics and graphical representations
	- Asset class capable of importing from ASE format, exporting to and importing from HeliosAssetFile format
	- Environment class encapsulating the game's execution environment
	- Craft class providing an example of a derived Actor, implementing pilot controls and showcasing usage of sub-objects
	- DiskStream class providing a uniform interface for packing/unpacking objects and structures to and from files on disk.
	- Additional functionality for BRZ::Vec2 class.
	- Updated game geometry resources, changed the ASE Importer class to parse two new commands (which need to be added by hand to .ASE files), *HELIOS_COLLIDES and *HELIOS_COLOUR r g b a
	- Configuration file now contains the variables for the craft's thruster powers and rates for dampening of velocity and angular velocity.  Mess around with them!
	- Note that Scene class is still UNIMPLEMENTED.  See ENVIRONMENT class for main game-loop logic.



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

