
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

	BRZ::Window wnd;
	wnd.Construct(L"Berzerk InDev", 800, 600);

	logout << "window created." << std::endl;

	if (!DirectX::XMVerifyCPUSupport())
	{
		logout << "DirectXMath CPU operations not supported.  Failure..." << std::endl;
		return 0;
	}


	// Graphics testing:

	BRZ::D3DResource<::ID3D11Device> device;
	::ID3D11Device * ptr = device;


	while (wnd.Active())
	{
		MSG message;
		while((PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
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