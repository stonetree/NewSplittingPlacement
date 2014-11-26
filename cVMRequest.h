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
	bool splittable;
	double lambda;

	uint arrival_time;
	uint departure_time;
	uint duration_time;

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

	void setVMRequestSplittable(bool _splittable){splittable = _splittable;}
	bool getVMRequestSplittable(void){return splittable;}

	void setLambda(double _lambda){lambda = _lambda;}
	double getLambda(void){return lambda;}

	void setArrivalTime(uint _time){arrival_time = _time;}
	uint getArrivalTime(void) {return arrival_time;}

	void setDepartureTime(uint _time){departure_time = _time;}
	uint getDepartureTime(void){return departure_time;}

	void setDurationTime(uint _time){duration_time = _time;}
	uint getDurationTime(void){return duration_time;}

public:
	cVMRequest(void);
	cVMRequest(ID _id,double _original_request,double _lambda,uint _arrival_time,uint _duration_time,bool _splittable = false,int _num_svm = 0,int _svm_res_request = 0):\
		request_id(_id),original_res_request(_original_request),\
		arrival_time(_arrival_time),duration_time(_duration_time),\
		lambda(_lambda),num_svm(_num_svm),svm_res_request(_svm_res_request),splittable(_splittable){departure_time = arrival_time + duration_time;}
	cVMRequest(const cVMRequest& _vm_request);
	cVMRequest& operator=(const cVMRequest& _vm_request);
	~cVMRequest(void);
};

