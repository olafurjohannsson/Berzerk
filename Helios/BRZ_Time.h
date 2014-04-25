
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
		Time();
		~Time();

	public:
		BRZRESULT		Cycle();
		unsigned int	LastFrame() const;
		unsigned int	TotalFrames() const;

	public:

	private:
		unsigned int		cores;
		unsigned __int64	freq;
		unsigned __int64	stamp;
		unsigned __int64	frame;
		unsigned int		turn;
	};
}