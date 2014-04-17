
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
	worker(NULL),
	workerID(0),
	data(NULL)
{
	data = new BRZ::Time::Package();
	LARGE_INTEGER holder;

	if (!::QueryPerformanceFrequency(&holder))
		::MessageBox(NULL, L"Performance Timer unavailable!", L"Fatal Error", MB_OK);
	else
		this->data->freq = holder.QuadPart;

	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	this->data->cores = sysinfo.dwNumberOfProcessors;

	worker = ::CreateThread(NULL, 0, &BRZ::Time::WorkerProc, (LPVOID)data, 0, (LPDWORD)&workerID);
	// Set the thread affinity mask for only core 2 (Note that one should implement behaviour based
	//	on how many cores the processor has; different behaviour will be needed on single core machines
	// ~ a non-threaded solution would work just fine there.
	if (::SetThreadAffinityMask(worker, 1<<1) == 0)
	{
		::MessageBox(NULL, L"Performance Timer Thread Affinity Error!", L"Fatal Error", MB_OK);
	}

	this->Cycle();
}


BRZ::Time::~Time()
{
	*data->exit = true;
	delete data;
}




unsigned int BRZ::Time::TotalFrames() const
{
	return data->turn;
}


unsigned int BRZ::Time::LastFrame() const
{
	// Check if the cycle is definitely done:
	if (data->poke)
		return -1;

	unsigned __int64	ms = data->frame / (data->freq / 1000);
	LARGE_INTEGER		holder;

	holder.QuadPart = ms;
	return holder.LowPart;
}


DWORD WINAPI BRZ::Time::WorkerProc(LPVOID A_arg)
{
	BRZ::Time::Package * pak = (BRZ::Time::Package *)A_arg;
	bool * exit = new bool(false);
	pak->exit = exit;

	while (!*exit)
	{
		if (!pak->poke)
			continue;
		else
		{
			// The timer has been poked to a new frame, update the frame number (turn), the timing of 
			//	the last frame (frame) and the total timestamp (stamp).
			unsigned __int64	ticks = 0;
			LARGE_INTEGER		holder;

			::QueryPerformanceCounter(&holder);
			ticks = holder.QuadPart;

			ticks = ticks - pak->stamp;

			if (pak->stamp == 0)
				pak->frame = 0;
			else
				pak->frame = ticks;

			pak->stamp += ticks;
			++pak->turn;
			pak->poke = false;
		}
	}
	delete exit;

	return 0;
}


BRZRESULT BRZ::Time::Cycle()
{
	data->poke = true;

	return BRZ_SUCCESS;
}




