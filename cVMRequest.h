#pragma once
#include "stdafx.h"

class cServer;

class cVMRequest
{
private:
	ID request_id;
	double original_res_request;
	uint  num_svm;
	double svm_res_request;
	bool splittable;
	double lambda;
	bool isServed;

	TIME_T arrival_time;
	TIME_T departure_time;
	TIME_T duration_time;

public:
	vector<cServer*> host_server_vec;

public:
	void setRequestID(ID _id){request_id = _id;}
	ID   getReqeustID(void){return request_id;}

	void setServed(bool _served){isServed = _served;}
	bool getServed(void){return isServed;}

	void setOriginalResRequest(double _request){original_res_request = _request;}
	double getOriginalResRequest(void){return original_res_request;}

	void setSVMNumber(uint _num){num_svm = _num;}
	uint getSVMNumber(void){return num_svm;}

	void setSVMResRequest(double _request){svm_res_request = _request;}
	double getSVMResRequest(void){return svm_res_request;}

	void setVMRequestSplittable(bool _splittable){splittable = _splittable;}
	bool getVMRequestSplittable(void){return splittable;}

	void setLambda(double _lambda){lambda = _lambda;}
	double getLambda(void){return lambda;}

	void setArrivalTime(TIME_T _time){arrival_time = _time;}
	TIME_T getArrivalTime(void) {return arrival_time;}

	void setDepartureTime(TIME_T _time){departure_time = _time;}
	TIME_T getDepartureTime(void){return departure_time;}

	void setDurationTime(TIME_T _time){duration_time = _time;}
	TIME_T getDurationTime(void){return duration_time;}

public:
	cVMRequest(void);
	cVMRequest(ID _id,double _original_request,double _lambda,TIME_T _arrival_time,TIME_T _duration_time,bool _splittable = false,uint _num_svm = 0,int _svm_res_request = 0,bool _isServed = false):\
		request_id(_id),original_res_request(_original_request),\
		arrival_time(_arrival_time),duration_time(_duration_time),\
		lambda(_lambda),num_svm(_num_svm),isServed(_isServed),svm_res_request(_svm_res_request),splittable(_splittable){departure_time = arrival_time + duration_time;}
	cVMRequest(const cVMRequest& _vm_request);
	cVMRequest& operator=(const cVMRequest& _vm_request);
	~cVMRequest(void);
};

