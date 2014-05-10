
#include <Windows.h>

#include "BRZ_Environment.h"

#include "BRZ_Craft.h"
#include "BRZ_RawGeometry.h"


BRZRESULT BRZ::Environment::Engage()
{
	if (!loaded || running)
		return BRZ_FAILURE;

	BRZ::Craft	player(cfg);
	player.Construct(as_craft, vid, input, BRZ::Vec2(), BRZ::HALF_PI / -2);

	BRZ::Actor	rock;
	rock.Construct(as_g_centre, vid, input, BRZ::Vec2(100, 100), BRZ::HALF_PI / 2);

	Note(L"Engaging Helios....");
	running = true;
	while (wnd.Active())
	{
		// Wrap the window message pump with checks for exiting, such that if the exit
		//	condition is detected, the window is destroyed naturally before the game loop
		//	breaks.
		if (exit)
			::SendMessage(wnd.Handle(), WM_CLOSE, 0, 0);

		MSG		msg;
		while (::PeekMessage(&msg, wnd.Handle(), 0, 0, PM_REMOVE) > 0)
		{
			// Note that if the program is exited via closing the window, it must be handled here:
			if (msg.message == WM_CLOSE)
				exit = true;

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		if (exit)
			break;

		// Collect and resolve input:
		input.Cycle();

		// Update the game world
		player.Cycle(time.LastFrame());

		if (player.TestCollision(rock))
			player.Reset();


		// Render as necessary, with the grid overlay on top:
		player.Render();
		rock.Render();
		// grid.Render(BRZ::Vec2(), 0);
		vid.Render();

		// Update the frame time:
		time.Cycle();
	}
	running = false;
	Note(L"Terminating Helios....");
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Environment::LoadBindings()
{
	input.Bind(BRZ::IK_W, BRZ::IE_MAIN_THRUSTER, BRZ::IT_WHILE);
	input.Bind(BRZ::IK_A, BRZ::IE_RIGHT_THRUSTER, BRZ::IT_WHILE);
	input.Bind(BRZ::IK_D, BRZ::IE_LEFT_THRUSTER, BRZ::IT_WHILE);
	input.Bind(BRZ::IK_S, BRZ::IE_RCS_THRUSTER, BRZ::IT_WHILE);

	input.Bind(BRZ::IK_SPACE, BRZ::IE_TRACTOR_BEAM, BRZ::IT_BEGIN);

	input.Bind(BRZ::IK_ESC, BRZ::IE_QUIT, BRZ::IT_END);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Environment::LoadAssets()
{
	vid.SetDataDirectory(cfg.ReadString(L"DataDirectory"));

	vid.GenerateGrid();

	// Test the Asset process:
	BRZSTRING in_Craft = L"craft.ASE";
	BRZSTRING in_GravityCentre = L"gravity_centre.ASE";

	BRZSTRING fileIn_Craft = cfg.ReadString(L"DataDirectory") + in_Craft;
	BRZSTRING fileIn_GravityCentre = cfg.ReadString(L"DataDirectory") + in_GravityCentre;
	as_craft.ImportASEObject(fileIn_Craft);
	as_g_centre.ImportASEObject(fileIn_GravityCentre);

	BRZ::RawGeometry	geoA;
	as_craft.ExtractGeometry(geoA);
	vid.BakeGeometry(geoA, as_craft.Name());

	BRZ::RawGeometry	geoB;
	as_g_centre.ExtractGeometry(geoB);
	vid.BakeGeometry(geoB, as_g_centre.Name());
	
	// vid.LoadGeometry(L"craft.ASE", L"craft");

	vid.LockGeometry();

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Environment::Initialize()
{
	if (loaded)
		return BRZ_FAILURE;

	// Log the number of processors:
	SYSTEM_INFO		sysInfo;
	::GetSystemInfo(&sysInfo);
	BRZOSTREAM		msg;
	msg << L"Initializing Helios -- " << sysInfo.dwNumberOfProcessors << L" processors detected.";
	Note(msg.str());



	if (DirectX::XMVerifyCPUSupport())
		Note(L"DirectXMath CPU operations supported.");
	else
	{
		Note(L"DirectXMath CPU operations NOT supported..  Exiting..");
		return BRZ_FAILURE;
	}


	// Initialize the window:
	if (wnd.Construct(L"Berzerk: HELIOS", 800, 600) != BRZ_SUCCESS)
	{
		Note(L"Could not construct Helios window..  Exiting..");
		return BRZ_FAILURE;
	}
	else
		Note(L"Successfully constructed Helios window.");


	// Load up the configuration file:
	if (cfg.LoadFile(L"HeliosConfig.cfg") != BRZ_SUCCESS)
	{
		Note(L"Could not load Helios configuration file..  Exiting..");
		return BRZ_FAILURE;
	}


	// Initialize the graphics module, loading all requisite data files:
	if (vid.Initialize(wnd.Handle()) != BRZ_SUCCESS)
	{
		Note(L"Could not initialize display..  Exiting..");
		return BRZ_FAILURE;
	}

	if (LoadAssets() != BRZ_SUCCESS)
	{
		Note(L"Could not load Helios assets..  Exiting..");
		return BRZ_FAILURE;
	}
	vid.Link(grid, L"grid");
	

	// Initialize the input module:
	LoadBindings();
	envBinds.Link(this);
	envBinds.Listen(BRZ::IE_QUIT);
	envBinds.Engage();

	Note(L"Helios initialization successful.");
	loaded = true;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Environment::ResolveInput(BRZ::InputEvent A_event)
{
	if (!running)
		return BRZ_SUCCESS;

	if (A_event == BRZ::IE_QUIT)
		exit = true;

	return BRZ_SUCCESS;
}


void BRZ::Environment::Note(const BRZSTRING & A_msg)
{
	log << BRZ::Narrow(BRZSTRING(L"[Environment]:\t") + A_msg) << std::endl;
	log.flush();
}


BRZ::Environment::Environment(std::ofstream & A_log) :
	log(A_log),
	cfg(log),
	vid(log),
	envBinds(input),
	exit(false),
	loaded(false),
	running(false),
	world(NULL),
	as_craft(log),
	as_g_centre(log)
{
	Note(L"Helios Environment created.");
}


BRZ::Environment::~Environment()
{
	if (world != NULL)
		delete world;


}