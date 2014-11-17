#pragma once
#include "stdafx.h"

class cServer;

class cVMRequest
{
private:
	ID request_id;
	double original_res_request;
	int  num_svm;
	double svm_res_request;

public:
	vector<cServer*> host_server_vec;

public:
	void setRequestID(ID _id){request_id = _id;}
	ID   getReqeustID(void){return request_id;}

	void setOriginalResRequest(double _request){original_res_request = _request;}
	double getOriginalResRequest(void){return original_res_request;}

	void setSVMNumber(int _num){num_svm = _num;}
	double getSVMNumber(void){return num_svm;}

	void setSVMResRequest(double _request){svm_res_request = _request;}
	double getSVMResRequest(void){return svm_res_request;}

public:
	cVMRequest(void);
	cVMRequest(ID _id,double _original_request,int _num_svm = 0,int _svm_res_request = 0):request_id(_id),original_res_request(_original_request),\
		num_svm(_num_svm),svm_res_request(_svm_res_request){}
	cVMRequest(const cVMRequest& _vm_request);
	cVMRequest& operator=(const cVMRequest& _vm_request);
	~cVMRequest(void);
};

