
#include <Windows.h>
#include <fstream>

#include "BZK_MemService.h"
#include "BZK_Utility.h"
#include "BZK_FileSystem.h"

namespace BZK
{
	unsigned int counter = 0;

	class TestClass
	{
	public:
		TestClass(std::ofstream * out = NULL) : out(out), data(BZK::counter)	{		BZK::counter++;		}
		~TestClass()	{	*out << "Destroying data value: " << data << std::endl;	}

		void Print()
		{
			*out << "This class has the data value: " << data << "!" << std::endl;
		}

		void TestFileSystem()
		{
			std::wstring path = L"C:\\Users\\Lenovo\\Dropbox\\root\\Projects\\Berzerk\\Ascension";
			BZK::FileSystem *fs = BZK::Alloc<BZK::FileSystem>(path);
			fs->CreateDir(L"asdf");
			fs->CreateFilez(L"asdf\\test.txt");
		}

		std::ofstream * out;
		unsigned int data;
	};
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// BEGIN MemService testing:
	BZK::InitMemService();





	std::ofstream output("asc_log.txt");
	output << "Some logging text?" << std::endl;

	BZK::TestClass * ptr = BZK::Alloc<BZK::TestClass>(&output);
	BZK::TestClass * ptr2 = BZK::Alloc<BZK::TestClass>(&output);
	BZK::TestClass * ptr3 = BZK::ArrayAlloc<BZK::TestClass>(4);

	for (int i = 0; i < 4; ++i)
		ptr3[i].out = &output;

	char * raw = BZK::RawAlloc(64);

	BZK::Free(ptr3);
	BZK::Free(ptr2);

	BZK::EndMemService();
	// END MemService testing.
	// Modified comment!

	output.close();

	return 0;
}