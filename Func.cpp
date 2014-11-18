#include "stdafx.h"
#include "common.h"
#include "cServer.h"
#include "cVMRequest.h"
#include "cCplexRuntime.h"


int initializeInputFile(vector<vector<double>>& _resource_request_vec)
{
	_resource_request_vec.clear();

	ifstream Ifile;
	double mu,k1,k2,k3,k4;
	vector<double> tem_cap;
	vector<vector<double>>::iterator Iter_cap;

	Ifile.open("k55.txt");

	if (!Ifile)
	{
		cout<<"Failed to open the file.\n"<<endl;
		return 1;
	}

	while (!Ifile.eof())
	{
		Ifile>>mu>>k1>>k2>>k3>>k4;
		tem_cap.clear();
		tem_cap.push_back(mu);
		tem_cap.push_back(k1);
		tem_cap.push_back(k2);
		tem_cap.push_back(k3);
		tem_cap.push_back(k4);
		_resource_request_vec.push_back(tem_cap);
	}
	Ifile.close();

	return 0;
}

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
	const gsl_rng_type * T, * T_splittable;
	gsl_rng * r, *r_splittable;
	double double_splittable;
	bool bool_splittable;

	_vmrequests_vec.clear();

	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	T_splittable = gsl_rng_default;
	r_splittable = gsl_rng_alloc(T_splittable);
	
	for (request_index = 1;request_index <= total_num_profile_55 + total_num_profile_70;request_index++)
	{
		random_num = gsl_rng_uniform_int (r, 2);
		double_splittable = gsl_rng_uniform(r_splittable);

		if (double_splittable <= splitable_percentage)
		{
			bool_splittable = true;
		}
		else
		{
			bool_splittable = false;
		}

		
		if (random_num == 0)
		{
			_vmrequests_vec.push_back(cVMRequest((ID)request_index,55,bool_splittable));
		}
		else
		{
			_vmrequests_vec.push_back(cVMRequest((ID)request_index,55,bool_splittable));
		}
	}

	gsl_rng_free(r);
	gsl_rng_free(r_splittable);
	
	
	
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

void initializeResourceRequest(vector<double>& _input_cap,map<pair<double,uint>,double>& _resource_request)
{
	_resource_request.clear();

	_resource_request.insert(make_pair(make_pair(_input_cap[1],1),_input_cap[1]));
	_resource_request.insert(make_pair(make_pair(_input_cap[1],2),_input_cap[2]));
	_resource_request.insert(make_pair(make_pair(_input_cap[1],3),_input_cap[3]));
	_resource_request.insert(make_pair(make_pair(_input_cap[1],4),_input_cap[4]));

	//_resource_request.insert(make_pair(make_pair(70,1),70));
	//_resource_request.insert(make_pair(make_pair(70,2),51));
	//_resource_request.insert(make_pair(make_pair(70,3),36));
	//_resource_request.insert(make_pair(make_pair(70,4),29));
	
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

void outputResults(double _arai,vector<cServer>& _server_vec)
{
	ofstream result_output;
	result_output.open("output.txt",ios_base::app);
	if (!result_output)
	{
		cerr<<"unable to open output file: optimization.txt";
		exit(-1);
	} 

	uint used_server_count = 0;
	double average_utilization = 0;
	vector<cServer>::iterator iter_server = _server_vec.begin();
	for (;iter_server != _server_vec.end();iter_server++)
	{
		if (0 != iter_server->getServOccupied())
		{
			used_server_count++;
		}

		average_utilization += iter_server->getServOccupied();
	}

	result_output<<_arai<<" "<<used_server_count<<" "<<average_utilization/used_server_count<<endl;
	result_output.close();
	return; 
}