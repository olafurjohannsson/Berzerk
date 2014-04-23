
#include <Windows.h>
#include <fstream>
#include <sstream>

#include "BRZ_Define.h"
#include "BRZ_Display.h"
#include "BRZ_Window.h"
#include "BRZ_LineObject.h"
#include "BRZ_Vec2.h"

// #include <d3dcompiler.h>

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
	

	

	BRZ::LineObject obj;
	BRZ::Vec2		pos(0.0f, 0.0f);
	BRZ::Vec2		lPos(0.0f, 0.0f);
	float			rot = 0.0f;
	float			mod = 1.0f;
	unsigned int	max = 100;
	unsigned int	now = 0;

	float			lMod = 1.0f;
	unsigned int	lMax = 30;
	unsigned int	lNow = 0;


	disp->Link(obj, L"craft");
	obj.elements[1].standard = false;


	while (wnd.Active())
	{
		
		MSG message;
		while((PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		
		// Update object position:
		if (now > max)
		{
			mod *= -1.0f;
			now -= max;
		}
		else
			++now;

		if (lNow > lMax)
		{
			lMod *= -1.0f;
			lNow -= lMax;
		}
		else
			++lNow;

		pos.x += mod;
		lPos.x += lMod;

		obj.elements[1].offset = lPos;
		obj.Render(pos, rot);

		disp->Render();
	}


	logout << "shmee.  Window destroyed." << std::endl;
	delete disp;

	return 0;
}