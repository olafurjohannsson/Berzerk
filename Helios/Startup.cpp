
#include <Windows.h>
// #include <ctime>
// #include <cstdlib>

#include "BRZ_Define.h"

#include "BRZ_Environment.h"



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	srand(::GetTickCount());

	std::ofstream log("Log_Helios.txt");
	BRZSTRING cmdLine(pCmdLine);
	log << BRZ::Narrow(cmdLine) << std::endl;
	log.flush();


	BRZ::Environment *	helios = new BRZ::Environment(log);
	helios->Initialize();
	helios->Engage();

	delete helios;


	log << "Exiting...." << std::endl;
	log.flush();
	log.close();

	return 0;
}