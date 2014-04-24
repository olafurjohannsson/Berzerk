
#include "BRZ_InputCore.h"

BRZRESULT BRZ::InputCore::Cycle()
{
	for (unsigned int i = BRZ::IE_UNDEFINED; i < BRZ::IE_MAX_SIZE; ++i)
	{
		if (i == BRZ::IE_UNDEFINED)
			continue;

		keyboard[binding[i]].Cycle();
	}

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::InputCore::Listen(BRZ::BindingModule * A_mod, BRZ::InputEvent A_event)
{
	// Not allowed to listen to 'undefined' or 'max size':
	if (A_event == BRZ::IE_UNDEFINED || A_event == BRZ::IE_MAX_SIZE)
		return BRZ_FAILURE;

	// If the event is not bound to a key, it cannot be listened for:
	if (binding[A_event] == BRZ::IK_UNDEFINED)
		return BRZ_FAILURE;

	keyboard[binding[A_event]].AddTarget(A_mod);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::InputCore::Ignore(BRZ::BindingModule * A_mod, BRZ::InputEvent A_event)
{
	// Not allowed to listen to 'undefined' or 'max size':
	if (A_event == BRZ::IE_UNDEFINED || A_event == BRZ::IE_MAX_SIZE)
		return BRZ_FAILURE;

	// If the event is not bound to a key, it cannot be listened for:
	if (binding[A_event] == BRZ::IK_UNDEFINED)
		return BRZ_FAILURE;

	keyboard[binding[A_event]].RemoveTarget(A_mod);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::InputCore::Bind(BRZ::InputKey A_key, BRZ::InputEvent A_event, BRZ::InputTrigger A_trig)
{
	// Input arguments ~ sanity check:
	if (A_key == BRZ::IK_UNDEFINED || A_key == BRZ::IK_MAX_SIZE || A_event == BRZ::IE_UNDEFINED || \
		A_event == BRZ::IE_MAX_SIZE || A_trig == BRZ::IT_UNDEFINED || A_trig == BRZ::IT_MAX_SIZE)
		return BRZ_FAILURE;

	if (binding[A_event] == BRZ::IK_UNDEFINED)
	{
		keyboard[A_key].Bind(A_event, A_trig);
		binding[A_event] = A_key;
		return BRZ_SUCCESS;
	}
	else
	{
		if (keyboard[binding[A_event]].CloneState(keyboard[A_key]) == BRZ_SUCCESS)
		{
			keyboard[binding[A_event]].ClearState();
			keyboard[A_key].Bind(A_event, A_trig);
			binding[A_event] = A_key;
			return BRZ_SUCCESS;
		}
		else
		{
			keyboard[binding[A_event]].ClearState();
			binding[A_event] = BRZ::IK_UNDEFINED;
			return BRZ_FAILURE;
		}
	}
}


BRZ::InputCore::InputCore()
{
	BRZ::ZeroMem(binding, BRZ::IE_MAX_SIZE);

	// Inititalize keyboard codes for asynchronous input polling:
	keyboard[BRZ::IK_A].Initialize(0x41);
	keyboard[BRZ::IK_B].Initialize(0x42);
	keyboard[BRZ::IK_C].Initialize(0x43);
	keyboard[BRZ::IK_D].Initialize(0x44);
	keyboard[BRZ::IK_E].Initialize(0x45);
	keyboard[BRZ::IK_F].Initialize(0x46);
	keyboard[BRZ::IK_G].Initialize(0x47);
	keyboard[BRZ::IK_H].Initialize(0x48);
	keyboard[BRZ::IK_I].Initialize(0x49);
	keyboard[BRZ::IK_J].Initialize(0x4A);
	keyboard[BRZ::IK_K].Initialize(0x4B);
	keyboard[BRZ::IK_L].Initialize(0x4C);
	keyboard[BRZ::IK_M].Initialize(0x4D);
	keyboard[BRZ::IK_N].Initialize(0x4E);
	keyboard[BRZ::IK_O].Initialize(0x4F);
	keyboard[BRZ::IK_P].Initialize(0x50);
	keyboard[BRZ::IK_Q].Initialize(0x51);
	keyboard[BRZ::IK_R].Initialize(0x52);
	keyboard[BRZ::IK_S].Initialize(0x53);
	keyboard[BRZ::IK_T].Initialize(0x54);
	keyboard[BRZ::IK_U].Initialize(0x55);
	keyboard[BRZ::IK_V].Initialize(0x56);
	keyboard[BRZ::IK_W].Initialize(0x57);
	keyboard[BRZ::IK_X].Initialize(0x58);
	keyboard[BRZ::IK_Y].Initialize(0x59);
	keyboard[BRZ::IK_Z].Initialize(0x5A);

	keyboard[BRZ::IK_L_SHIFT].Initialize(VK_LSHIFT);
	keyboard[BRZ::IK_R_SHIFT].Initialize(VK_RSHIFT);
	keyboard[BRZ::IK_L_CTRL].Initialize(VK_LCONTROL);
	keyboard[BRZ::IK_R_CTRL].Initialize(VK_RCONTROL);
	keyboard[BRZ::IK_L_ALT].Initialize(VK_LMENU);
	keyboard[BRZ::IK_R_ALT].Initialize(VK_RMENU);

	keyboard[BRZ::IK_ESC].Initialize(VK_ESCAPE);
	keyboard[BRZ::IK_ENTER].Initialize(VK_RETURN);
	keyboard[BRZ::IK_SPACE].Initialize(VK_SPACE);
	keyboard[BRZ::IK_BKSP].Initialize(VK_BACK);

	keyboard[BRZ::IK_PRIMARY].Initialize(VK_LBUTTON);
	keyboard[BRZ::IK_SECONDARY].Initialize(VK_RBUTTON);
//	keyboard[BRZ::IK_WHEELUP].Initialize(VK_MWHEELUP);
//	keyboard[BRZ::IK_WHEELDOWN].Initialize(VK_RBUTTON);

	keyboard[BRZ::IK_0].Initialize(0x30);
	keyboard[BRZ::IK_1].Initialize(0x31);
	keyboard[BRZ::IK_2].Initialize(0x32);
	keyboard[BRZ::IK_3].Initialize(0x33);
	keyboard[BRZ::IK_4].Initialize(0x34);
	keyboard[BRZ::IK_5].Initialize(0x35);
	keyboard[BRZ::IK_6].Initialize(0x36);
	keyboard[BRZ::IK_7].Initialize(0x37);
	keyboard[BRZ::IK_8].Initialize(0x38);
	keyboard[BRZ::IK_9].Initialize(0x39);


	keyboard[BRZ::IK_F1].Initialize(0x70);
	keyboard[BRZ::IK_F2].Initialize(0x71);
	keyboard[BRZ::IK_F3].Initialize(0x72);
	keyboard[BRZ::IK_F4].Initialize(0x73);
	keyboard[BRZ::IK_F5].Initialize(0x74);
	keyboard[BRZ::IK_F6].Initialize(0x75);
	keyboard[BRZ::IK_F7].Initialize(0x76);
	keyboard[BRZ::IK_F8].Initialize(0x77);
	keyboard[BRZ::IK_F9].Initialize(0x78);
	keyboard[BRZ::IK_F10].Initialize(0x79);
	keyboard[BRZ::IK_F11].Initialize(0x7A);
	keyboard[BRZ::IK_F12].Initialize(0x7B);


	keyboard[BRZ::IK_UP].Initialize(VK_UP);
	keyboard[BRZ::IK_LEFT].Initialize(VK_LEFT);
	keyboard[BRZ::IK_RIGHT].Initialize(VK_RIGHT);
	keyboard[BRZ::IK_DOWN].Initialize(VK_DOWN);
}


BRZ::InputCore::~InputCore()
{
}