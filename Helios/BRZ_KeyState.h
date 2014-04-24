
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_KeyState.h" )
#endif

#include "BRZ_Define.h"
#include "BRZ_Input.h"

namespace BRZ
{
	class KeyState
	{
	private:
		typedef std::vector<BRZ::BindingModule *> BindVector;

	public:
		KeyState();
		~KeyState();

	public:
		void		Initialize(unsigned int code);
		BRZRESULT	Bind(BRZ::InputEvent action, BRZ::InputTrigger trigger);
		BRZRESULT	AddTarget(BRZ::BindingModule * module);
		BRZRESULT	RemoveTarget(BRZ::BindingModule * module);
		BRZRESULT	Cycle();

		bool		IsBound() const;
		BRZRESULT	CloneState(BRZ::KeyState & target);
		void		ClearState();

	private:
		BindVector			targets;
		unsigned int		code;
		BRZ::InputEvent		action;
		BRZ::InputTrigger	trigger;
		bool				on;
	};
}