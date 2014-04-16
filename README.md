Berzerk game.

~ Added gitignore whitelist;		- DF
~ Barebones renderer added, D3D11 w/ shaders.  Note:
	- modify the input variables to the Display::Render function to change the location and rotation
		of the box testing object
	- note that the .fx files comprise the shaders, they are compiled to .cso (compiled shader object)
		files as part of the build process, integrated into Visual Studio 2012.
	- note that Display::fullScreen variable (initialized in constructer) determines fullScreen or not,
		this cannot be changed yet on the fly, ALT+F4 to exit fullscreen game.
	- note: consider either ::GetAsyncKeyState (Win32 API) or WM_KEYDOWN messages to capture keyboard input.