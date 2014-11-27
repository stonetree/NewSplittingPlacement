#pragma once
#include "stdafx.h"
#include "common.h"
#include "cVMRequest.h"


class cEvent
{
private:
	event_type type;
	cVMRequest *request;

public:
	event_type getEventType(void){return type;}
	cVMRequest* getRequestPoint(void){return request;}

public:
	cEvent(void);
	cEvent(event_type _type,cVMRequest* _request):type(_type),request(_request){}
	cEvent(cEvent& _event);
	cEvent& operator=(cEvent& _event);
	~cEvent(void);
};

