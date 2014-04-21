
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Time.h" )
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class Time
	{
	public:
		struct Package
		{
			Package() : poke(false), exit(NULL), cores(1), freq(1), stamp(0), frame(0), turn(0)	{    }

			volatile bool				poke;
			volatile bool *				exit;
			volatile unsigned int		cores;
			volatile unsigned __int64	freq;
			volatile unsigned __int64	stamp;
			volatile unsigned __int64	frame;
			volatile unsigned int		turn;
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
		HANDLE			worker;
		unsigned int	workerID;
		Package *		data;
	};
}