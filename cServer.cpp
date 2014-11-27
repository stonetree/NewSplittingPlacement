#include "StdAfx.h"
#include "cServer.h"
#include "common.h"


cServer::cServer(void)
{
}


cServer::~cServer(void)
{
}

cServer::cServer(const cServer& _server)
{
	this->operator=(_server);
}

cServer& cServer::operator=(const cServer& _server)
{
	if (this != &_server)
	{
		server_id = _server.server_id;
		server_weight = _server.server_weight;
		server_capacity = _server.server_capacity;
		server_occupied = _server.server_occupied;

		server_time_weight.clear();
		server_time_weight.insert(_server.server_time_weight.begin(),_server.server_time_weight.end());

		server_time_residual.clear();
		server_time_residual.insert(_server.server_time_residual.begin(),_server.server_time_residual.end());
	}

	return *this;
}

double cServer::getTimeResidualCapacity(uint _time_slot)
{
	map<uint,double>::iterator iter_time_residual_capacity;
	iter_time_residual_capacity = server_time_residual.find(_time_slot);
	if (iter_time_residual_capacity != server_time_residual.end())
	{
		return iter_time_residual_capacity->second;
	}
	else
		return server_capacity;
}

void cServer::setTimeResourceUsed(cVMRequest& _request,double _resource_used)
{
	map<uint,double>::iterator iter_time_resource_used;
	server_weight = 0;
	uint time_slot = 0;
	uint arrival_time = _request.getArrivalTime();
	uint duration_time = _request.getDurationTime();
	for (time_slot = 0;time_slot<duration_time;time_slot++)
	{
		iter_time_resource_used = server_time_residual.find(arrival_time + time_slot);
		if (iter_time_resource_used != server_time_residual.end())
		{
			iter_time_resource_used->second = iter_time_resource_used->second - _resource_used;
			//server_weight += 1/(server_capacity*exp(iter_time_resource_used->second/server_capacity));
		}
		else
		{
			server_time_residual.insert(make_pair(arrival_time + time_slot,server_capacity - _resource_used));
			//server_weight += 1/(server_capacity*exp(_resource_used/server_capacity));
		}
	}


	return;
}

void cServer::setTimeWeight(cVMRequest& _request,double _resource_used)
{
	//It should be called after cServer::setTimeResourceUsed() function!!!!
	
	uint arrival_time = _request.getArrivalTime();
	uint duration_time = _request.getDurationTime();
	uint time_slot;

	map<uint,double>::iterator iter_time_weight;
	map<uint,double>::iterator iter_time_residual;

	for (time_slot = 0;time_slot<duration_time;time_slot++)
	{
		iter_time_weight = server_time_weight.find(arrival_time + time_slot);
		iter_time_residual = server_time_residual.find(arrival_time + time_slot);

		if (iter_time_weight != server_time_weight.end())
		{
			iter_time_weight->second = 1/(server_capacity*exp((server_capacity - iter_time_residual->second)/server_capacity));
		}
		else
		{
			server_time_weight.insert(make_pair(arrival_time + time_slot,1/(server_capacity*exp(_resource_used/server_capacity))));
		}
	}
	return;
}

double cServer::getTimeWeight(uint _time,uint _duration_time)
{
	double tem_serv_weight = 0;
	uint time_slot = 0;
	map<uint,double>::iterator iter_time_weight;
	for (time_slot = 0;time_slot<_duration_time;time_slot++)
	{
		iter_time_weight = server_time_weight.find(_time + time_slot);
		if (iter_time_weight != server_time_weight.end())
		{
			tem_serv_weight += iter_time_weight->second;
		}
		else
		{
			tem_serv_weight += 1/server_capacity;
		}
	}
	
	return tem_serv_weight;
}

bool cServer::enoughCapacity(uint _arrival_time,uint _departure_time,double _required)
{
	bool enough_capacity = true;
	for (;_arrival_time<_departure_time;_arrival_time++)
	{
		if (_required > getTimeResidualCapacity(_arrival_time))
		{
			enough_capacity = false;
		}
	}
	
	return enough_capacity;
}
