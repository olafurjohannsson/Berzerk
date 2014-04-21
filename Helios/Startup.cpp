
#include <Windows.h>
#include <fstream>
#include <sstream>

#include "BRZ_Define.h"
#include "BRZ_Display.h"
#include "BRZ_Window.h"

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
	



	while (wnd.Active())
	{
		
		MSG message;
		while((PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		disp->TestQueue();
		disp->Render();
	}


	logout << "shmee.  Window destroyed." << std::endl;
	delete disp;

	return 0;
}