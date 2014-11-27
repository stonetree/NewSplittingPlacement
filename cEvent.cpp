#include "StdAfx.h"
#include "cEvent.h"


cEvent::cEvent(void)
{
}


cEvent::~cEvent(void)
{
}

cEvent::cEvent(cEvent& _event)
{
	this->operator=(_event);
}

cEvent& cEvent::operator=(cEvent& _event)
{
	if (this != &_event)
	{
		type = _event.type;
		request = _event.request;
	}
	
	return *this;
}