
#include "BRZ_KeyState.h"
#include "BRZ_BindingModule.h"


BRZRESULT BRZ::KeyState::Cycle()
{
	if (code == 0 || trigger == BRZ::IT_UNDEFINED || trigger == BRZ::IT_MAX_SIZE || action == BRZ::IE_UNDEFINED || \
		action == BRZ::IE_MAX_SIZE)
		return BRZ_FAILURE;

	if (targets.empty())
		return BRZ_SUCCESS;

	unsigned short	msb = 0x8000;
	bool			down = ((::GetAsyncKeyState(code) & msb) > 0);

	BRZ::BindingModule & module = *targets.back();

	switch (trigger)
	{
	case BRZ::IT_BEGIN:
		if (down && !on)
		{
			on = true;
			return module.Push(action);
		}
		else if (!down && on)
		{
			on = false;
			return BRZ_SUCCESS;
		}
		else
			return BRZ_SUCCESS;

	case BRZ::IT_END:
		if (down && !on)
		{
			on = true;
			return BRZ_SUCCESS;
		}
		else if (!down && on)
		{
			on = false;
			return module.Push(action);
		}
		else
			return BRZ_SUCCESS;

	case BRZ::IT_WHILE:
		if (down)
		{
			on = true;
			return module.Push(action);
		}
		else
		{
			on = false;
			return BRZ_SUCCESS;
		}

	case BRZ::IT_FLIP:
		if (down && !on)
		{
			on = true;
			return module.Push(action);
		}
		else if (!down && on)
		{
			on = false;
			return module.Push(action);
		}
		else
			return BRZ_SUCCESS;
	}

	// Should never get here.
	return BRZ_FAILURE;
}


BRZRESULT BRZ::KeyState::CloneState(BRZ::KeyState & A_target)
{
	if (A_target.IsBound())
		return BRZ_FAILURE;

	A_target.ClearState();
	A_target.action = this->action;
	A_target.trigger = this->trigger;
	A_target.on = this->on;

	for (auto i = targets.begin(); i != targets.end(); ++i)
	{
		A_target.targets.push_back(*i);
	}

	return BRZ_SUCCESS;
}


bool BRZ::KeyState::IsBound() const
{
	return (action != BRZ::IE_UNDEFINED);
}


void BRZ::KeyState::ClearState()
{
	targets.clear();
	action = BRZ::IE_UNDEFINED;
	trigger = BRZ::IT_UNDEFINED;
	on = false;
}


BRZRESULT BRZ::KeyState::RemoveTarget(BRZ::BindingModule * A_mod)
{
	if (A_mod == NULL)
		return BRZ_FAILURE;

	if (targets.empty())
		return BRZ_FAILURE;

	for (auto i = targets.begin(); i != targets.end(); ++i)
	{
		if (*i == A_mod)
		{
			targets.erase(i);
			return BRZ_SUCCESS;
		}
	}

	return BRZ_FAILURE;
}


BRZRESULT BRZ::KeyState::AddTarget(BRZ::BindingModule * A_mod)
{
	if (A_mod == NULL)
		return BRZ_FAILURE;

	for (auto i = targets.cbegin(); i != targets.cend(); ++i)
	{
		if (*i == A_mod)
			return BRZ_FAILURE;
	}

	targets.push_back(A_mod);

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::KeyState::Bind(BRZ::InputEvent A_event, BRZ::InputTrigger A_trig)
{
	// input sanity check:
	if (A_event == BRZ::IE_UNDEFINED || A_event == BRZ::IE_MAX_SIZE || A_trig == BRZ::IT_UNDEFINED || \
		A_trig == BRZ::IT_MAX_SIZE)
	{
		return BRZ_FAILURE;
	}

	action = A_event;
	trigger = A_trig;
	
	return BRZ_SUCCESS;
}


void BRZ::KeyState::Initialize(unsigned int A_code)
{
	code = A_code;
}


BRZ::KeyState::KeyState() :
	code(0),
	action(BRZ::IE_UNDEFINED),
	trigger(BRZ::IT_UNDEFINED),
	on(false)
{
}


BRZ::KeyState::~KeyState()
{
}