
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Input.h" )
#endif

namespace BRZ
{
	class InputCore;
	class KeyState;

	class BindingModule;

	enum InputEvent;
	enum InputKey;
	enum InputTrigger;
	

	class ActiveObject
	{
	public:
		virtual BRZRESULT ResolveInput(BRZ::InputEvent action) = 0;
	};
	

	enum InputEvent
	{
		IE_UNDEFINED			= 0,

		IE_QUIT,

		IE_MAIN_THRUSTER,
		IE_LEFT_THRUSTER,
		IE_RIGHT_THRUSTER,

		IE_TRACTOR_BEAM,

		IE_MAX_SIZE
	};

	enum InputTrigger
	{
		IT_UNDEFINED	= 0,

		IT_BEGIN,
		IT_END,
		IT_WHILE,
		IT_FLIP,

		IT_MAX_SIZE
	};

	enum InputKey
	{
		IK_UNDEFINED		= 0,

		IK_A,
		IK_B,
		IK_C,
		IK_D,
		IK_E,
		IK_F,
		IK_G,
		IK_H,
		IK_I,
		IK_J,
		IK_K,
		IK_L,
		IK_M,
		IK_N,
		IK_O,
		IK_P,
		IK_Q,
		IK_R,
		IK_S,
		IK_T,
		IK_U,
		IK_V,
		IK_W,
		IK_X,
		IK_Y,
		IK_Z,
		
		IK_0,
		IK_1,
		IK_2,
		IK_3,
		IK_4,
		IK_5,
		IK_6,
		IK_7,
		IK_8,
		IK_9,
		
		IK_F1,
		IK_F2,
		IK_F3,
		IK_F4,
		IK_F5,
		IK_F6,
		IK_F7,
		IK_F8,
		IK_F9,
		IK_F10,
		IK_F11,
		IK_F12,

		IK_SPACE,
		IK_ENTER,
		IK_BKSP,
		IK_DEL,
		IK_L_ALT,
		IK_L_CTRL,
		IK_L_SHIFT,
		IK_R_ALT,
		IK_R_CTRL,
		IK_R_SHIFT,

		IK_ESC,

		IK_UP,
		IK_DOWN,
		IK_LEFT,
		IK_RIGHT,

		IK_PRIMARY,
		IK_SECONDARY,
		IK_WHEELUP,
		IK_WHEELDOWN,

		IK_MAX_SIZE
	};
}