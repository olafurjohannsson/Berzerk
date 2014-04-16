
#include <Windows.h>
#include <fstream>
#include <sstream>

#include "BRZ_Define.h"
#include "BRZ_Display.h"
#include "BRZ_Window.h"
#include "BRZ_Time.h"

// #include <d3dcompiler.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	std::ofstream logout("Log_Helios.txt");

	BRZ::Time timer;

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
		logout << "Could not initialize display.  Exiting..." << std::endl;
		return 0;
	}

	if (disp->GenerateGeo() != BRZ_SUCCESS)
	{
		logout << "Could not generate geometry.  Exiting..." << std::endl;
		return 0;
	}

	if (disp->LoadShaders() != BRZ_SUCCESS)
	{
		logout << "Could not initialize shaders.  Exiting..." << std::endl;
		return 0;
	}

	// Graphics testing:

	/*
	BRZ::D3DResource<::ID3D11Device> device;
	::ID3D11Device * ptr = device;
	*/


	float objX = 0.0f;
	float objY = 0.0f;
	float objR = 0.0f;

	while (wnd.Active())
	{
		timer.Cycle();
		MSG message;
		while((PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		/*
		float rateT = 5;
		float rateR = BRZ::PI / 4;

		float xOff = timer.LastFrame();
		xOff /= 1000.0f;
		xOff *= rateT;
		objX += xOff;


		float rOff = timer.LastFrame();
		rOff /= 1000.0f;
		rOff *= rateR;
		objR += rOff;
		*/

		disp->Render(objX, objY, objR);
	}

	/*
	float base[4] = {888, 696.7, 1463.3, 480.7};
	float unk[10] = {482, 718, 1370, 1505, 1838, 1985, 2243, 2385, 3008, 3263};
	float err = 60;

	for (int i = 0; i < 10; ++i)
	{
		float target = unk[i];
		std::ostringstream out("");
		for (int j = 0; j < 10; ++j)
		{
			for (int k = 0; k < 10; ++k)
			{
				for (int l = 0; l < 10; ++l)
				{
					for (int n = 0; n < 10; ++n)
					{
						float test = (j * base[0]) + (k * base[1]) + (l * base[2]) + (n * base[3]);
						float margin = abs(test - target);
						if (margin < err)
						{
							out << "For target [" << i << "] - (" << target << ") : (" << j << ", " << k << ", " << l << ", " << n << ") = " << test << ", err = " << margin;
							logout << out.str() << std::endl;
							out = std::ostringstream();
						}
					}
				}
			}
		}
	}
	*/




	logout << "shmee.  Window destroyed." << std::endl;

	return 0;
}