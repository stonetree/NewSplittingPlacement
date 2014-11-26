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
	bool enoughCapacity(double _required){return _required <= (server_capacity - server_occupied);}

public:
	cServer(void);
	cServer(ID _id,double _weight,double _capacity,double _occupied = 0):server_id(_id),server_weight(_weight),server_capacity(_capacity),server_occupied(_occupied){}
	cServer(const cServer& _server);
	cServer& operator=(const cServer& _server);
	~cServer(void);
};

