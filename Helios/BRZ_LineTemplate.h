
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_LineTemplate.h" )
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class LineTemplate
	{
	public:
		LineTemplate();
		LineTemplate(const LineTemplate & ref);
		~LineTemplate();

	public:
		void AllocElems(unsigned int count);

	public:
		unsigned int	numElements;
		unsigned int *	idElements;
		BRZSTRING		name;
	};
}