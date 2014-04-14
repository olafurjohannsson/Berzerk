
#include "BRZ_Time.h"

#include <Windows.h>

/*
	class Time
	{
	public:
		Time();

	public:
		BRZRESULT		Cycle();
		unsigned int	LastFrame() const;
		unsigned int	TotalFrames() const;

		BRZRESULT		BeginCapture();
		BRZRESULT		EndCapture();
		unsigned int	CapAverage() const;
		unsigned int	CapDuration() const;
		unsigned int	CapFrames() const;

	private:
		unsigned __int64	freq;
		unsigned __int64	stamp;
		unsigned __int64	frame;

		unsigned int		turn;

		bool				capturing;
		unsigned int		capFrameInit;
		unsigned int		capFrameEnd;
		unsigned __int64	capInit;
		unsigned __int64	capEnd;
	};
*/


unsigned int BRZ::Time::CapFrames() const
{
	if (capturing)
		return 0;

	return capFrameEnd - capFrameInit;
}


unsigned int BRZ::Time::CapDuration() const
{
	if (capturing)
		return 0;

	unsigned __int64	ms = (capEnd - capInit) / (freq / 1000);
	LARGE_INTEGER		holder;
	holder.QuadPart = ms;

	return holder.LowPart;
}


unsigned int BRZ::Time::CapAverage() const
{
	if (capturing)
		return 0;

	unsigned __int64	ms = (capEnd - capInit) / (freq / 1000);
	LARGE_INTEGER		holder;
	holder.QuadPart = ms;

	return holder.LowPart / (capFrameEnd - capFrameInit);
}


BRZRESULT BRZ::Time::EndCapture()
{
	if (!capturing)
		return BRZ_FAILURE;

	capFrameEnd = turn;
	capEnd = stamp;
	capturing = false;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::Time::BeginCapture()
{
	if (capturing)
		return BRZ_FAILURE;
	else
	{
		capFrameInit = 0;
		capFrameEnd = 0;
		capInit = 0;
		capEnd = 0;
	}

	capFrameInit = turn;
	capInit = stamp;
	capturing = true;

	return BRZ_SUCCESS;
}


unsigned int BRZ::Time::TotalFrames() const
{
	return turn;
}


unsigned int BRZ::Time::LastFrame() const
{
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


BRZ::Time::Time() :
	freq(1),
	stamp(0),
	frame(0),
	turn(0),
	capturing(false),
	capFrameInit(0),
	capFrameEnd(0),
	capInit(0),
	capEnd(0)
{
	LARGE_INTEGER holder;

	if (!::QueryPerformanceFrequency(&holder))
		::MessageBox(NULL, L"Performance Timer unavailable!", L"Fatal Error", MB_OK);
	else
		this->freq = holder.QuadPart;
}

