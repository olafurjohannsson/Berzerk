
#include "BRZ_BindingModule.h"

#include "BRZ_InputCore.h"


BRZRESULT BRZ::BindingModule::Disengage()
{
	if (!active)
		return BRZ_FAILURE;

	for (unsigned int i = BRZ::IE_UNDEFINED; i < BRZ::IE_MAX_SIZE; ++i)
	{
		if (i == BRZ::IE_UNDEFINED)
			continue;

		if (listening[i] == true)
			core.Ignore(this, (BRZ::InputEvent)i);
	}

	active = false;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::BindingModule::Engage()
{
	if (active)
		return BRZ_FAILURE;

	for (unsigned int i = BRZ::IE_UNDEFINED; i < BRZ::IE_MAX_SIZE; ++i)
	{
		if (i == BRZ::IE_UNDEFINED)
			continue;

		if (listening[i] == true)
			core.Listen(this, (BRZ::InputEvent)i);
	}

	active = true;
	return BRZ_SUCCESS;
}


BRZRESULT BRZ::BindingModule::Ignore(BRZ::InputEvent A_event)
{
	if (active)
		return BRZ_FAILURE;

	if (A_event == BRZ::IE_UNDEFINED || A_event == BRZ::IE_MAX_SIZE)
		return BRZ_FAILURE;

	listening[A_event] = false;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::BindingModule::Listen(BRZ::InputEvent A_event)
{
	if (active)
		return BRZ_FAILURE;

	if (A_event == BRZ::IE_UNDEFINED || A_event == BRZ::IE_MAX_SIZE)
		return BRZ_FAILURE;

	listening[A_event] = true;

	return BRZ_SUCCESS;
}


BRZRESULT BRZ::BindingModule::Push(BRZ::InputEvent A_event)
{
	if (target == NULL)
		return BRZ_FAILURE;

	if (A_event == BRZ::IE_UNDEFINED || A_event == BRZ::IE_MAX_SIZE)
		return BRZ_FAILURE;

	return target->ResolveInput(A_event);
}


BRZRESULT BRZ::BindingModule::Link(BRZ::ActiveObject * A_obj)
{
	target = A_obj;

	return BRZ_SUCCESS;
}


BRZ::BindingModule::BindingModule(BRZ::InputCore & A_core) :
	core(A_core),
	target(NULL),
	active(false)
{
	for (unsigned int i = BRZ::IE_UNDEFINED; i < BRZ::IE_MAX_SIZE; ++i)
		listening[i] = false;
}


BRZ::BindingModule::~BindingModule()
{
}