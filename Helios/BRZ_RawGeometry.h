
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_RawGeometry.h" )
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class RawGeometry
	{
	public:
		RawGeometry();
		RawGeometry(const RawGeometry & ref);
		~RawGeometry();

	public:
		BRZ::RawElement *	elem;
		unsigned int		count;

	protected:
	private:
	};
}