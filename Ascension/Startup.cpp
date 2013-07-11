
#include <Windows.h>
#include <fstream>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	std::ofstream output("asc_log.txt");
	output << "Some logging text?";

	return 0;
}