
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
}