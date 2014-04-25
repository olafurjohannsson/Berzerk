
#include "BRZ_Time.h"

#include <Windows.h>

/*
	class Time
	{
	public:
		struct Package
		{
			Package() : poke(false), cores(1), freq(1), stamp(0), frame(0), turn(0)	{    }

			bool	 			poke;
			unsigned int		cores;
			unsigned __int64	freq;
			unsigned __int64	stamp;
			unsigned __int64	frame;
			unsigned int		turn;
		};

	public:
		Time();
		~Time();

	public:
		BRZRESULT		Cycle();
		unsigned int	LastFrame() const;
		unsigned int	TotalFrames() const;

	public:
		static DWORD WINAPI WorkerProc(LPVOID param);

	private:
		HANDLE		worker;
		Package *	data;
	};
*/


BRZ::Time::Time() :
	cores(1),
	freq(1),
	stamp(0),
	frame(0),
	turn(0)
{
	LARGE_INTEGER holder;

	if (!::QueryPerformanceFrequency(&holder))
		::MessageBox(NULL, L"Performance Timer unavailable!", L"Fatal Error", MB_OK);
	else
		this->freq = holder.QuadPart;

	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	this->cores = sysinfo.dwNumberOfProcessors;

	this->Cycle();
}


BRZ::Time::~Time()
{
}




unsigned int BRZ::Time::TotalFrames() const
{
	return turn;
}


unsigned int BRZ::Time::LastFrame() const
{
	// Check if the cycle is definitely done:

	unsigned __int64	ms = frame / (freq / 1000);
	LARGE_INTEGER		holder;

	holder.QuadPart = ms;
	return holder.LowPart;
}


BRZRESULT BRZ::Time::Cycle()
{
	unsigned __int64	ticks = 0;
	LARGE_INTEGER		holder;

	::QueryPerformanceCounter(&holder);
	ticks = holder.QuadPart;

	ticks = ticks - stamp;

	if (stamp == 0)
		frame = 0;
	else
		frame = ticks;

	stamp += ticks;
	++turn;

	return BRZ_SUCCESS;
}




