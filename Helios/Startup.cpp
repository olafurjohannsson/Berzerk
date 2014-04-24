
#include <Windows.h>
#include <fstream>
#include <sstream>

#include "BRZ_Define.h"

#include "BRZ_BindingModule.h"
#include "BRZ_Display.h"
#include "BRZ_InputCore.h"
#include "BRZ_LineObject.h"
#include "BRZ_Vec2.h"
#include "BRZ_Window.h"

namespace BRZ
{
	class TestActor : public BRZ::ActiveObject
	{
	public:
		TestActor() : rot(0)	{ }

	public:
		virtual BRZRESULT ResolveInput(BRZ::InputEvent action);

	public:
		BRZ::LineObject		obj;
		BRZ::Vec2			pos;
		float				rot;
	};

	class Environment : public BRZ::ActiveObject
	{
	public:
		Environment() : quit(false)	{ }

	public:
		virtual BRZRESULT ResolveInput(BRZ::InputEvent action);

	public:
		bool	quit;
	};
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


BRZRESULT BRZ::TestActor::ResolveInput(BRZ::InputEvent A_event)
{
	switch (A_event)
	{
	case BRZ::IE_MAIN_THRUSTER:
		pos.y += 1.0f;
		return BRZ_SUCCESS;

	case BRZ::IE_LEFT_THRUSTER:
		pos.x -= 1.0f;
		return BRZ_SUCCESS;

	case BRZ::IE_RIGHT_THRUSTER:
		pos.x += 1.0f;
		return BRZ_SUCCESS;

	case BRZ::IE_TRACTOR_BEAM:
		pos.y -= 1.0f;
		return BRZ_SUCCESS;
	}

	return BRZ_FAILURE;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	std::ofstream logout("Log_Helios.txt");

	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	unsigned int numCPU = sysinfo.dwNumberOfProcessors;
	logout << "Initializing Helios: " << numCPU << " processer cores." << std::endl;


	BRZ::Window wnd;
	wnd.Construct(L"Berzerk InDev", 800, 600);

	logout << "window created." << std::endl;

	if (!DirectX::XMVerifyCPUSupport())
	{
		logout << "DirectXMath CPU operations not supported.  Failure..." << std::endl;
		return 0;
	}

	BRZ::Display * disp = new BRZ::Display(logout);
	
	if (disp->Initialize(wnd.Handle()) != BRZ_SUCCESS)
	{
		logout << "Could not initialized display.  Exiting..." << std::endl;
		return 0;
	}
	disp->GenerateGeometry();
 	disp->LockGeometry();


	// INPUT MODULE TESTING:
	BRZ::InputCore *	input = new BRZ::InputCore();
	
	input->Bind(BRZ::IK_UP, BRZ::IE_MAIN_THRUSTER, BRZ::IT_WHILE);
	input->Bind(BRZ::IK_LEFT, BRZ::IE_LEFT_THRUSTER, BRZ::IT_WHILE);
	input->Bind(BRZ::IK_RIGHT, BRZ::IE_RIGHT_THRUSTER, BRZ::IT_WHILE);
	input->Bind(BRZ::IK_DOWN, BRZ::IE_TRACTOR_BEAM, BRZ::IT_WHILE);
	input->Bind(BRZ::IK_ESC, BRZ::IE_QUIT, BRZ::IT_END);

	BRZ::Environment	game;
	BRZ::TestActor		actor;
	disp->Link(actor.obj, L"craft");
	BRZ::BindingModule	moduleA(*input);
	BRZ::BindingModule	moduleG(*input);
	moduleA.Link(&actor);
	moduleA.Listen(BRZ::IE_MAIN_THRUSTER);
	moduleA.Listen(BRZ::IE_LEFT_THRUSTER);
	moduleA.Listen(BRZ::IE_RIGHT_THRUSTER);
	moduleA.Listen(BRZ::IE_TRACTOR_BEAM);
	moduleA.Engage();

	moduleG.Link(&game);
	moduleG.Listen(BRZ::IE_QUIT);
	moduleG.Engage();


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

		actor.obj.Render(actor.pos, actor.rot);

		input->Cycle();
		disp->Render();
	}

	moduleA.Disengage();
	moduleG.Disengage();
	delete input;


	logout << "shmee.  Window destroyed." << std::endl;
	delete disp;

	return 0;
}