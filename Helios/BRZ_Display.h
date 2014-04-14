
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message("Header Trace: BRZ_Display.h")
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	template <class T_type>
	class D3DResource
	{
	public:
		D3DResource() : ptr(NULL)					{	 }
		~D3DResource()								{ if (ptr != NULL) { ptr->Release(); }	}

		inline operator const T_type *() const		{ return ptr;		}
		inline operator T_type *()					{ return ptr;		}

		inline T_type ** operator&()				{ return &ptr;		}
		inline const T_type ** operator&() const	{ return &ptr;		}

	private:
		T_type * ptr;
	};


	class Display
	{
	public:
		
	};
}
