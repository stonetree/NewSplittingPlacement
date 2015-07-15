#pragma once
#include "stdafx.h"
#include "cVMRequest.h"

class cServer
{
private:
	ID server_id;
	double server_weight;
	double server_capacity;
	double server_occupied;
	double server_residual;
	double total_resources_used;

public:
	map<TIME_T,double> server_time_weight;
	map<TIME_T,double> server_time_residual;

public:
	void setServID(ID _id) {server_id = _id;}
	ID   getServID(void) {return server_id;}

	void setServWeight(double _weight){server_weight = _weight;}
	double getServWeight(void){return server_weight;}

	void setServCapacity(double _capacity){server_capacity = _capacity;}
	double getServCapacity(void){return server_capacity;}

	void setServOccupied(double _occpied){server_occupied = _occpied;}
	double getServOccupied(void){return server_occupied;}

public:
	double getRemainingCapacity(void){return server_capacity - server_occupied;}
	bool isEmpty(void) {return server_residual == server_capacity;}
	bool enoughCapacity(TIME_T _arrival_time,TIME_T _departure_time,double _required);
	bool enoughResidual(double _required){return _required <= server_residual;}
	double getResidual(void){return server_residual;}
	void allocateResidual(double _required,TIME_T _arrival_time,TIME_T _duration_time);
	void releaseResidual(double _required);
	double getTimeResidualCapacity(TIME_T _time_slot);
	void setTimeResourceUsed(cVMRequest& _request,double _resource_used);
	void setTimeWeight(cVMRequest& _request,double _resource_used);
	double getTimeWeight(TIME_T _time,TIME_T _duration_time);
	double getTimeResidualCapacity(TIME_T _arrival_time,TIME_T _departure_time);
	double getTotalResUsed(void){return total_resources_used;}

public:
	cServer(void);
	cServer(ID _id,double _weight,double _capacity,double _occupied = 0,double _residual = 0):server_id(_id),server_weight(_weight),server_capacity(_capacity),server_residual(_capacity),server_occupied(_occupied),total_resources_used(0){}
	cServer(const cServer& _server);
	cServer& operator=(const cServer& _server);
	~cServer(void);
};

