
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_BindingModule.h" )
#endif

#include "BRZ_Define.h"
#include "BRZ_Input.h"

namespace BRZ
{
	class BindingModule
	{
	public:
		BindingModule(BRZ::InputCore & core);
		~BindingModule();

	public:
		BRZRESULT Link(BRZ::ActiveObject * object);
		BRZRESULT Push(BRZ::InputEvent action);

		BRZRESULT Listen(BRZ::InputEvent action);
		BRZRESULT Ignore(BRZ::InputEvent action);

		BRZRESULT Engage();
		BRZRESULT Disengage();

	private:
		BRZ::InputCore &		core;
		BRZ::ActiveObject *		target;
		bool					listening[BRZ::IE_MAX_SIZE];
		bool					active;
	};
}