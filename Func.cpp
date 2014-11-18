#include "stdafx.h"
#include "common.h"
#include "cServer.h"
#include "cVMRequest.h"
#include "cCplexRuntime.h"


void initializeServers(vector<cServer>& _server_vec)
{
	uint server_index;
	_server_vec.clear();

	for (server_index = 1;server_index <= total_num_profile_55 + total_num_profile_70;server_index++)
	{
		_server_vec.push_back(cServer((ID)server_index,1/total_server_capacity,total_server_capacity));
	}

	return;
}

void initializeVMRequests(vector<cVMRequest>& _vmrequests_vec)
{
	uint request_index;
	unsigned long int random_num;
	const gsl_rng_type * T;
	gsl_rng * r;

	_vmrequests_vec.clear();

	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);
	
	for (request_index = 1;request_index <= total_num_profile_55 + total_num_profile_70;request_index++)
	{
		random_num = gsl_rng_uniform_int (r, 2);
		
		if (random_num == 0)
		{
			_vmrequests_vec.push_back(cVMRequest((ID)request_index,55));
		}
		else
		{
			_vmrequests_vec.push_back(cVMRequest((ID)request_index,55));
		}
	}

	gsl_rng_free(r);
	
	
	
	//for (request_index = 1;request_index <= total_num_profile_55;request_index++)
	//{
	//	_vmrequests_vec.push_back(cVMRequest((ID)request_index,55));
	//}

	//for (;request_index <= total_num_profile_70;request_index++)
	//{
	//	_vmrequests_vec.push_back(cVMRequest((ID)request_index,70));
	//}

	return;
}

void initializeResourceRequest(map<pair<double,uint>,double>& _resource_request)
{
	_resource_request.clear();

	_resource_request.insert(make_pair(make_pair(55,1),55));
	_resource_request.insert(make_pair(make_pair(55,2),34));
	_resource_request.insert(make_pair(make_pair(55,3),24));
	_resource_request.insert(make_pair(make_pair(55,4),19));

	_resource_request.insert(make_pair(make_pair(70,1),70));
	_resource_request.insert(make_pair(make_pair(70,2),51));
	_resource_request.insert(make_pair(make_pair(70,3),36));
	_resource_request.insert(make_pair(make_pair(70,4),29));
	
	return;
}
	

void allocateVMRequest(cVMRequest& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{
	cCplexRuntime *p_cplex_runingtime = new cCplexRuntime;
	p_cplex_runingtime->VarInit(_vmrequest,_server_vec,_resource_request);
	p_cplex_runingtime->ModelConstruction(_vmrequest,_server_vec);
	p_cplex_runingtime->ProblemSolve(_vmrequest,_server_vec,_resource_request);

	free(p_cplex_runingtime);
	
	return;
}