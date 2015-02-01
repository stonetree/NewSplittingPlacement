#include "StdAfx.h"
#include "cVMRequest.h"


cVMRequest::cVMRequest(void)
{
}


cVMRequest::~cVMRequest(void)
{
}

cVMRequest::cVMRequest(const cVMRequest& _vm_request)
{
	this->operator=(_vm_request);
}

cVMRequest& cVMRequest::operator=(const cVMRequest& _vm_request)
{
	if (this != &_vm_request)
	{
		request_id = _vm_request.request_id;
		isServed = _vm_request.isServed;
		original_res_request = _vm_request.original_res_request;
		num_svm = _vm_request.num_svm;
		svm_res_request = _vm_request.svm_res_request;
		splittable = _vm_request.splittable;
		lambda = _vm_request.lambda;

		arrival_time = _vm_request.arrival_time;
		departure_time = _vm_request.departure_time;
		duration_time = _vm_request.duration_time;
	}

	return *this;
}
