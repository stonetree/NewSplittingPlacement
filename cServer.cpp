#include "StdAfx.h"
#include "cServer.h"


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
	}

	return *this;
}
