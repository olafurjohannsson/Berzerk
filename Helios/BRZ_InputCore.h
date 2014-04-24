
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_InputCore.h" )
#endif

#include "BRZ_Define.h"
#include "BRZ_Input.h"

#include "BRZ_KeyState.h"

namespace BRZ
{
	class InputCore
	{
	public:
		InputCore();
		~InputCore();

	public:
		BRZRESULT Bind(BRZ::InputKey key, BRZ::InputEvent action, BRZ::InputTrigger trigger);
		BRZRESULT Listen(BRZ::BindingModule * module, BRZ::InputEvent action);
		BRZRESULT Ignore(BRZ::BindingModule * module, BRZ::InputEvent action);

		BRZRESULT Cycle();

	private:
		BRZ::KeyState	keyboard[BRZ::IK_MAX_SIZE];
		BRZ::InputKey	binding[BRZ::IE_MAX_SIZE];
	};
}