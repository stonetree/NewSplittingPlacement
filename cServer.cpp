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
		server_residual = _server.server_residual;
		total_resources_used = _server.total_resources_used;

		server_time_weight.clear();
		server_time_weight.insert(_server.server_time_weight.begin(),_server.server_time_weight.end());

		server_time_residual.clear();
		server_time_residual.insert(_server.server_time_residual.begin(),_server.server_time_residual.end());
	}

	return *this;
}

double cServer::getTimeResidualCapacity(TIME_T _time_slot)
{
	map<double,double>::iterator iter_time_residual_capacity;
	iter_time_residual_capacity = server_time_residual.find(_time_slot);
	if (iter_time_residual_capacity != server_time_residual.end())
	{
		return iter_time_residual_capacity->second;
	}
	else
		return server_capacity;
}

double cServer::getTimeResidualCapacity(TIME_T _arrival_time,TIME_T _departure_time)
{
	double residual_capacity = 0;
	map<TIME_T,double>::iterator iter_time_residual_capacity;
	for (;_arrival_time<_departure_time;_arrival_time++)
	{
		residual_capacity += getTimeResidualCapacity(_arrival_time);	
	}
	
	return residual_capacity;
}

void cServer::setTimeResourceUsed(cVMRequest& _request,double _resource_used)
{
	map<TIME_T,double>::iterator iter_time_resource_used;
	server_weight = 0;
	TIME_T time_slot = 0;
	TIME_T arrival_time = _request.getArrivalTime();
	TIME_T duration_time = _request.getDurationTime();
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
	
	TIME_T arrival_time = _request.getArrivalTime();
	TIME_T duration_time = _request.getDurationTime();
	TIME_T time_slot;

	map<TIME_T,double>::iterator iter_time_weight;
	map<TIME_T,double>::iterator iter_time_residual;

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

double cServer::getTimeWeight(TIME_T _time,TIME_T _duration_time)
{
	double tem_serv_weight = 0;
	TIME_T time_slot = 0;
	map<TIME_T,double>::iterator iter_time_weight;
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

bool cServer::enoughCapacity(TIME_T _arrival_time,TIME_T _departure_time,double _required)
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

void cServer::allocateResidual(double _required,TIME_T _arrival_time,TIME_T _duration_time)
{
	server_residual -= _required;
	
	if (_arrival_time + _duration_time > total_running_time)
	{
		total_resources_used += _required * (total_running_time - _arrival_time);
	}
	else
	{
		total_resources_used += _required * _duration_time;
	}
	return;
}
