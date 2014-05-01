
#include <Windows.h>
// #include <ctime>
// #include <cstdlib>

#include "BRZ_Define.h"

#include "BRZ_BindingModule.h"
#include "BRZ_Config.h"
#include "BRZ_Display.h"
#include "BRZ_InputCore.h"
#include "BRZ_LineObject.h"
#include "BRZ_Time.h"
#include "BRZ_Vec2.h"
#include "BRZ_Window.h"

namespace BRZ
{
	class Environment : public BRZ::ActiveObject
	{
	public:
		Environment() : quit(false)	{ }

	public:
		virtual BRZRESULT ResolveInput(BRZ::InputEvent action);

	public:
		bool	quit;
	};

	class TimedActor : public BRZ::ActiveObject
	{
	public:
		TimedActor() : rot(0), vel(25), active(false)	{ }

	public:
		virtual BRZRESULT ResolveInput(BRZ::InputEvent action);
		BRZRESULT Update(unsigned int timeDelta);

	public:
		BRZ::LineObject		obj;
		BRZ::Vec2			pos;
		float				rot;
		float				vel;	// in pixels / second
		bool				active;
	};
}


BRZRESULT BRZ::TimedActor::ResolveInput(BRZ::InputEvent A_event)
{
	if (A_event == BRZ::IE_TRACTOR_BEAM)
	{
		active = !active;
		return BRZ_SUCCESS;
	}
	if (A_event == BRZ::IE_MAIN_THRUSTER)
	{
		pos = BRZ::Vec2(0, 0);
		return BRZ_SUCCESS;
	}

	return BRZ_FAILURE;
}


BRZRESULT BRZ::TimedActor::Update(unsigned int A_delta)
{
	if (!active)
		return BRZ_SUCCESS;

	float dt = A_delta;
	dt /= 1000.0f;
	dt *= vel;
	pos.x += dt;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Environment::ResolveInput(BRZ::InputEvent A_event)
{
	if (A_event == BRZ::IE_QUIT)
	{
		quit = true;
		return BRZ_SUCCESS;
	}

	return BRZ_FAILURE;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	srand(::GetTickCount());

	std::ofstream logout("Log_Helios.txt");

	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	// Get number of cores, check whether DirectX Math is supported by the processor:
	unsigned int numCPU = sysinfo.dwNumberOfProcessors;
	logout << "Initializing Helios: " << numCPU << " processer cores." << std::endl;
	if (!DirectX::XMVerifyCPUSupport())
	{
		logout << "DirectXMath CPU operations not supported.  Failure..." << std::endl;
		return 0;
	}

	BRZ::Window		wnd;
	wnd.Construct(L"Berzerk InDev", 800, 600);
	logout << "Window created." << std::endl;


	BRZ::Config		cfg(logout);
	cfg.LoadFile(L"HeliosConfig.cfg");
	logout << "Config string: " << BRZ::Narrow(cfg.ReadString(L"string")) << std::endl;
	logout << "Bad string: " << BRZ::Narrow(cfg.ReadString(L"varThatDoesNotExist")) << std::endl;

	

	BRZ::Display * disp = new BRZ::Display(logout);
	
	if (disp->Initialize(wnd.Handle()) != BRZ_SUCCESS)
	{
		logout << "Could not initialized display.  Exiting..." << std::endl;
		return 0;
	}

	// Test ASEIMPORT functionality:
	disp->SetDataDirectory(L"..\\Helios\\Data\\");
	disp->LoadGeometry(L"test_obj.ase", L"ase");


	// disp->GenerateGeometry();
	disp->GenerateGrid();
	disp->GenerateAsteroids();
 	disp->LockGeometry();



	// INPUT MODULE TESTING:
	BRZ::InputCore *	input = new BRZ::InputCore();
	
	input->Bind(BRZ::IK_UP, BRZ::IE_MAIN_THRUSTER, BRZ::IT_BEGIN);
	// input->Bind(BRZ::IK_LEFT, BRZ::IE_LEFT_THRUSTER, BRZ::IT_WHILE);
	// input->Bind(BRZ::IK_RIGHT, BRZ::IE_RIGHT_THRUSTER, BRZ::IT_WHILE);
	input->Bind(BRZ::IK_DOWN, BRZ::IE_TRACTOR_BEAM, BRZ::IT_BEGIN);
	input->Bind(BRZ::IK_ESC, BRZ::IE_QUIT, BRZ::IT_END);

	BRZ::Environment	game;
	BRZ::TimedActor		actor;
	disp->Link(actor.obj, L"ase");
	BRZ::BindingModule	moduleA(*input);
	BRZ::BindingModule	moduleG(*input);
	moduleA.Link(&actor);
	moduleA.Listen(BRZ::IE_MAIN_THRUSTER);
	moduleA.Listen(BRZ::IE_TRACTOR_BEAM);
	moduleA.Engage();

	moduleG.Link(&game);
	moduleG.Listen(BRZ::IE_QUIT);
	moduleG.Engage();

	// Test Grid:
	BRZ::LineObject	grid;
	disp->Link(grid, L"grid");
	BRZ::LineObject rock;
	disp->Link(rock, L"rock");

	// Test Asteroid Gen:


	BRZ::Time *	localTime = new BRZ::Time();
	while (wnd.Active())
	{
		if (game.quit)
			::SendMessage(wnd.Handle(), WM_CLOSE, 0, 0);
		
		MSG message;
		while((PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		actor.Update(localTime->LastFrame());
		actor.obj.Render(actor.pos, actor.rot);
		rock.Render(BRZ::Vec2(0.0f, 50.0f), 0);
		grid.Render(BRZ::Vec2(), 0);

		input->Cycle();
		disp->Render();
		localTime->Cycle();
	}

	moduleA.Disengage();
	moduleG.Disengage();
	delete input;


	logout << "Exiting..." << std::endl;
	delete disp;

	return 0;
}