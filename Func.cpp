#include "stdafx.h"
#include "common.h"
#include "cServer.h"
#include "cVMRequest.h"
#include "cCplexRuntime.h"
#include "cEvent.h"




//int initializeInputFile(vector<vector<double>>& _resource_request_vec)
//{
//	_resource_request_vec.clear();
//
//	ifstream Ifile;
//	double mu,k1,k2,k3,k4;
//	vector<double> tem_cap;
//	vector<vector<double>>::iterator Iter_cap;
//
//	Ifile.open("k55.txt");
//
//	if (!Ifile)
//	{
//		cout<<"Failed to open the file.\n"<<endl;
//		return 1;
//	}
//
//	while (!Ifile.eof())
//	{
//		Ifile>>mu>>k1>>k2>>k3>>k4;
//		tem_cap.clear();
//		tem_cap.push_back(mu);
//		tem_cap.push_back(k1);
//		tem_cap.push_back(k2);
//		tem_cap.push_back(k3);
//		tem_cap.push_back(k4);
//		_resource_request_vec.push_back(tem_cap);
//	}
//	Ifile.close();
//
//	return 0;
//}

int initializeInputFile(vector<vector<double>>& _resource_request_vec)
{
	_resource_request_vec.clear();

	ifstream Ifile;
	double low_55,high_55,low_70,high_70,low_80,high_80,low_90,high_90,low_100,high_100;
	vector<double> tem_cap;
	vector<vector<double>>::iterator Iter_cap;

	Ifile.open("input.txt");

	if (!Ifile)
	{
		cout<<"Failed to open the file.\n"<<endl;
		return 1;
	}

	while (!Ifile.eof())
	{
		Ifile>>low_55>>high_55>>low_70>>high_70>>low_80>>high_80>>low_90>>high_90>>low_100>>high_100;
		tem_cap.clear();
		tem_cap.push_back(low_55);
		tem_cap.push_back(high_55);
		tem_cap.push_back(low_70);
		tem_cap.push_back(high_70);
		tem_cap.push_back(low_80);
		tem_cap.push_back(high_80);
		tem_cap.push_back(low_90);
		tem_cap.push_back(high_90);
		tem_cap.push_back(low_100);
		tem_cap.push_back(high_100);
		_resource_request_vec.push_back(tem_cap);
	}
	Ifile.close();

	return 0;
}

void initializeInputParameters(vector<double>& _input_parameter)
{
	lambda_low_55 = _input_parameter[0];
	lambda_high_55 = _input_parameter[1];
	lambda_low_70 = _input_parameter[2];
	lambda_high_70 = _input_parameter[3];
	lambda_low_80 = _input_parameter[4];
	lambda_high_80 = _input_parameter[5];
	lambda_low_90 = _input_parameter[6];
	lambda_high_90 = _input_parameter[7];
	lambda_low_100 = _input_parameter[8];
	lambda_high_100 = _input_parameter[9];
	
	return;
}

void initializeServers(vector<cServer>& _server_vec)
{
	uint server_index;
	_server_vec.clear();
	unusedServers.clear();

	for (server_index = 1;server_index <= total_num_servers;server_index++)
	{
		_server_vec.push_back(cServer((ID)server_index,1/server_capacity,server_capacity));
	}

	vector<cServer>::iterator iter_server = _server_vec.begin();
	for (;iter_server != _server_vec.end();iter_server++)
	{
		unusedServers.insert(make_pair(iter_server->getServID(),&(*iter_server)));
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
	double lambda;
	TIME_T interval_time,arrival_time,duration_time;
	//gsl_rng_type * T_55,T_70,T_80,T_90,T_100;
	gsl_rng * r_55,*r_70,*r_80,*r_90,*r_100;
	gsl_rng *r_arriva_time,*r_duration_time;

	_vmrequests_vec.clear();

	gsl_rng_env_setup();
	gsl_rng_default_seed = 10;
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	r_55 = gsl_rng_alloc (T);
	r_70 = gsl_rng_alloc (T);
	r_80 = gsl_rng_alloc (T);
	r_90 = gsl_rng_alloc (T);
	r_100 = gsl_rng_alloc (T);

	T_splittable = gsl_rng_default;
	r_splittable = gsl_rng_alloc(T_splittable);

	r_arriva_time = gsl_rng_alloc(T);
	r_duration_time = gsl_rng_alloc(T);

	TIME_T current_time = 0;
	request_index = 1;
	
	while (1)
	//for (request_index = 1;request_index <= total_requests;request_index++)
	{
		random_num = gsl_rng_uniform_int (r, 1);
		double_splittable = gsl_rng_uniform(r_splittable);

		interval_time = (TIME_T)gsl_ran_exponential(r_arriva_time,100/arrival_rate_per_100);
		arrival_time = (TIME_T)(current_time + interval_time);
		current_time = arrival_time;
		duration_time = (TIME_T)gsl_ran_exponential(r_duration_time,1/departure_rate);

		if (duration_time < 1)
		{
			duration_time+=1;
		}

		//Where the request is splittable.
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
			//original resource request = 55;
			lambda = gsl_ran_flat(r_55,lambda_low_55,lambda_high_55);
			_vmrequests_vec.push_back(cVMRequest((ID)request_index,55,lambda,arrival_time,duration_time,bool_splittable));
		}
		else if (random_num == 1)
		{
			//original resource requirement = 70;
			lambda = gsl_ran_flat(r_70,lambda_low_70,lambda_high_70);
			_vmrequests_vec.push_back(cVMRequest((ID)request_index,70,lambda,arrival_time,duration_time,bool_splittable));
		}
		else if (random_num == 2)
		{
			//original resource requirement = 80;
			lambda = gsl_ran_flat(r_80,lambda_low_80,lambda_high_80);
			_vmrequests_vec.push_back(cVMRequest((ID)request_index,80,lambda,arrival_time,duration_time,bool_splittable));
		}
		else if (random_num == 3)
		{
			//original resource requirement = 90;
			lambda = gsl_ran_flat(r_90,lambda_low_90,lambda_high_90);
			_vmrequests_vec.push_back(cVMRequest((ID)request_index,90,lambda,arrival_time,duration_time,bool_splittable));
		}
		else
		{
			//original resource requirement = 100;
			lambda = gsl_ran_flat(r_100,lambda_low_100,lambda_high_100);
			_vmrequests_vec.push_back(cVMRequest((ID)request_index,100,lambda,arrival_time,duration_time,bool_splittable));
		}

		request_index++;
		if (current_time > total_running_time)
		{
			break;
		}
	}

	gsl_rng_free(r);
	gsl_rng_free(r_splittable);
	gsl_rng_free(r_55);
	gsl_rng_free(r_70);
	gsl_rng_free(r_80);
	gsl_rng_free(r_90);
	gsl_rng_free(r_100);
	gsl_rng_free(r_arriva_time);
	gsl_rng_free(r_duration_time);
	
	
	
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

void initializeResourceRequest()
{
	loadResourceRequirementfunc();	
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
	
void initializeEvent(multimap<TIME_T,cEvent>& _event_map,vector<cVMRequest>& _request_vec)
{
	_event_map.clear();

	vector<cVMRequest>::iterator iter_request_vec = _request_vec.begin();
	for (;iter_request_vec != _request_vec.end();iter_request_vec++)
	{
		_event_map.insert(make_pair(iter_request_vec->getArrivalTime(),cEvent(ARRIVAL,&(*iter_request_vec))));
		_event_map.insert(make_pair(iter_request_vec->getDepartureTime(),cEvent(DEPARTURE,&(*iter_request_vec))));
	}
	
	return;
}

//void allocateVMRequest(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
//{
//	return;
//}

void allocateVMRequest(double _arai,vector<cVMRequest>& _vmrequest_vec,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{
	cCplexRuntime *p_cplex_runingtime = new cCplexRuntime;
	p_cplex_runingtime->VarInit(_vmrequest_vec,_server_vec,_resource_request);
	p_cplex_runingtime->ModelConstruction(_vmrequest_vec,_server_vec);
	p_cplex_runingtime->ProblemSolve(_arai,_vmrequest_vec,_server_vec,_resource_request);

	free(p_cplex_runingtime);
	
	return;
}

//void allocateGlobalVMRequest(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
//{
//	cCplexRuntime *p_cplex_runingtime = new cCplexRuntime;
//	p_cplex_runingtime->VarInitGlobal(_vmrequest,_server_vec,_resource_request);
//	p_cplex_runingtime->ModelConstructionGlobal(_vmrequest,_server_vec);
//	p_cplex_runingtime->ProblemSolveGlobal(_vmrequest,_server_vec,_resource_request);
//
//	free(p_cplex_runingtime);
//
//	return;
//}

void allocateVMRequestGreedy(cVMRequest& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{
	vector<cServer*> tobePlaced;

	double original_request = _vmrequest.getOriginalResRequest();
	double lambda = _vmrequest.getLambda();
	double svm_request;
	double num_svm;
	TIME_T arrival_time,duration_time,departure_time;
	arrival_time = _vmrequest.getArrivalTime();
	duration_time = _vmrequest.getDurationTime();
	departure_time = _vmrequest.getDepartureTime();

	//find servers with enough capacity from those servers that have already been used.
	uint i;

	uint max_num_svm = 2;
	if (!_vmrequest.getVMRequestSplittable())
	{
		max_num_svm = 1;
	}
	for (i = 1;i<=max_num_svm;i++)
	{
		tobePlaced.clear();

		num_svm = i;
		svm_request = (resourceRequirement[_vmrequest.getOriginalResRequest()])(_vmrequest.getOriginalResRequest(),_vmrequest.getLambda(),i);
		
		map<ID,cServer*>::iterator iter_used_servers;		
		//rearranging the set of already used physical servers in descending order according to their residual resource
		multimap<double,cServer*> sort_used_servers;
		for (iter_used_servers = usedServers.begin();iter_used_servers != usedServers.end();iter_used_servers++)
		{
			sort_used_servers.insert(make_pair(iter_used_servers->second->getResidual(),iter_used_servers->second));
		}


		//SVMs belonging to the same application should be deployed on different physical servers
		multimap<double,cServer*>::reverse_iterator riter_sort_used_servers;
		for (riter_sort_used_servers = sort_used_servers.rbegin();riter_sort_used_servers != sort_used_servers.rend();riter_sort_used_servers++)
		{

			if ((riter_sort_used_servers->second)->enoughResidual(svm_request))
			{
				tobePlaced.push_back(riter_sort_used_servers->second);
			}

			if (tobePlaced.size() == i)
			{
				//find enough capacities from already being used servers
				_vmrequest.setSVMNumber(i);
				_vmrequest.setSVMResRequest(svm_request);
				_vmrequest.setServed(true);
				vector<cServer*>::iterator iter_place_servers = tobePlaced.begin();
				for (;iter_place_servers != tobePlaced.end();iter_place_servers++)
				{
					//(*iter_place_servers)->setTimeResourceUsed(_vmrequest,svm_request);
					(*iter_place_servers)->allocateResidual(svm_request,_vmrequest.getArrivalTime(),_vmrequest.getDurationTime());
					_vmrequest.host_server_vec.push_back(*iter_place_servers);
				}

				return;
			}
		}
		
	}//

	//place the current request on to an unused server
	map<ID,cServer*>::iterator iter_unused_servers = unusedServers.begin();
	for (;iter_unused_servers != unusedServers.end();iter_unused_servers++)
	{

		if ((iter_unused_servers->second)->enoughResidual(_vmrequest.getOriginalResRequest()))
		//if ((iter_unused_servers->second)->enoughCapacity(arrival_time,departure_time,_vmrequest.getOriginalResRequest()))
		{
			//find a server having enough capacity from the set of unused servers
			//iter_unused_servers->second->setServOccupied(iter_unused_servers->second->getServOccupied() + _vmrequest.getOriginalResRequest());
			//iter_unused_servers->second->setTimeResourceUsed(_vmrequest,_vmrequest.getOriginalResRequest());
			(iter_unused_servers->second)->allocateResidual(_vmrequest.getOriginalResRequest(),_vmrequest.getArrivalTime(),_vmrequest.getDurationTime());
			_vmrequest.setSVMNumber(1);
			_vmrequest.setSVMResRequest(_vmrequest.getOriginalResRequest());
			_vmrequest.setServed(true);
			_vmrequest.host_server_vec.push_back(iter_unused_servers->second);
			usedServers.insert(make_pair(iter_unused_servers->second->getServID(),iter_unused_servers->second));
			unusedServers.erase(iter_unused_servers);

			return;
		}
	}
	
	return;
}

void outputResults(double _arai,vector<cServer>& _server_vec,vector<cVMRequest>& _vmrequest_vec)
{
	ofstream result_output;
	ofstream result_vm_num;
	result_output.open("output.txt",ios_base::app);
	result_vm_num.open("output_vm.txt",ios_base::app);
	double total_resource_used = 0;

	uint counting_accepted_requests = 0;
	double physical_resource_per_accepted_request = 0.0;

	double vm_1,vm_2,vm_3,vm_4;
	vm_1 = vm_2 = vm_3 = vm_4 = 0;

	if (!result_output)
	{
		cerr<<"unable to open output file: optimization.txt";
		exit(-1);
	} 

	uint used_server_count = 0;
	double average_utilization = 0;
	vector<cServer>::iterator iter_server;
	map<TIME_T,double>::iterator iter_server_used;

	//for (iter_server = _server_vec.begin();iter_server != _server_vec.end();iter_server++)
	//{
	//	uint time_slot;
	//	for (time_slot = 0;time_slot < total_time_slot;time_slot++)
	//	{
	//		iter_server_used = iter_server->server_time_residual.find(time_slot);
	//		if (iter_server_used != iter_server->server_time_residual.end())
	//		{
	//			used_server_count++;
	//			//break;
	//		}
	//	}
	//}


	//for (iter_server = _server_vec.begin();iter_server != _server_vec.end();iter_server++)
	//{
	//	TIME_T time_slot;
	//	for (time_slot = 0;time_slot < total_time_slot;time_slot++)
	//	{
	//		iter_server_used = iter_server->server_time_residual.find(time_slot);
	//		if (iter_server_used != iter_server->server_time_residual.end())
	//		{
	//			used_server_count++;
	//			average_utilization += iter_server_used->second;
	//		}
	//	}		
	//}

	for (iter_server = _server_vec.begin();iter_server != _server_vec.end();iter_server++)
	{
		total_resource_used += iter_server->getTotalResUsed();
	}

	vector<cVMRequest>::iterator iter_vm_request;
	for (iter_vm_request = _vmrequest_vec.begin();iter_vm_request != _vmrequest_vec.end();iter_vm_request++)
	{
		if (iter_vm_request->getServed() == true)
		{
			counting_accepted_requests++;
			uint svm_num = iter_vm_request->getSVMNumber();
			switch (svm_num)
			{
			case 1:
				vm_1++;
				break;
			case 2:
				vm_2++;
				break;
			case 3:
				vm_3++;
				break;
			case 4:
				vm_4++;
				break;
			}
		}
	}

	result_output<<_arai<<" "<<arrival_rate_per_100<<" "<<splitable_percentage<<" "<<(double)total_server_used<<" "<<(double)counting_accepted_requests/_vmrequest_vec.size()<<" "<<(double)total_resource_used/(total_num_servers*server_capacity*total_running_time)<<" "<<endl;
	result_output.close();

	result_vm_num<<_arai<<" "<<arrival_rate_per_100<<" "<<splitable_percentage<<" "<<vm_1/counting_accepted_requests<<" "<<vm_2/counting_accepted_requests<<" "<<vm_3/counting_accepted_requests<<" "<<vm_4/counting_accepted_requests<<endl;
	result_vm_num.close();
	return; 
}

void releaseUsedResource(cVMRequest& _request,vector<cServer>& _server_vec)
{
	vector<cServer*>::iterator iter_p_server = _request.host_server_vec.begin();
	for (;iter_p_server != _request.host_server_vec.end();iter_p_server++)
	{
		
	}
	
	
	return;
}
void updateServWeight(cVMRequest& _request,vector<cServer>& _server_vec)
{
	return;
}

void updateServCandidate(cVMRequest& _vmrequest)
{
	if (!_vmrequest.getServed())
	{
		//the request had not been served yet
		//no resource release function should be done
		return;
	}
	
	TIME_T arrival_time = _vmrequest.getArrivalTime();
	TIME_T duration_time = _vmrequest.getDurationTime();
	TIME_T departure_time = _vmrequest.getDepartureTime();
	TIME_T time_slot = 0;

	map<ID,cServer*>::iterator iter_used_server;
	vector<cServer*>::iterator iter_host_server = _vmrequest.host_server_vec.begin();
	for (;iter_host_server != _vmrequest.host_server_vec.end();iter_host_server++)
	{
		//release the resources occupied by the request
		(*iter_host_server)->releaseResidual(_vmrequest.getSVMResRequest());

		if (cplex_flag == true)
		{
			//cplex is using to find the optimal solution
			//following codes will not be executed
			continue;
		}

		iter_used_server = usedServers.find((*iter_host_server)->getServID());
		if (iter_used_server == usedServers.end())
		{
			cout<<"Already deleted."<<endl;
			continue;
		}

		bool unused_flag = true;

		if (!iter_used_server->second->isEmpty())
		{
			//current server is still being used by other applications
			unused_flag = false;
		}

		//for (time_slot = departure_time;time_slot < total_time_slot;time_slot++)
		//{
		//	if (iter_used_server->second->getTimeResidualCapacity(time_slot) < iter_used_server->second->getServCapacity())
		//	{
		//		unused_flag = false;
		//		break;
		//	}
		//}

		if (unused_flag == true)
		{
			usedServers.erase((*iter_host_server)->getServID());
			unusedServers.insert(make_pair((*iter_host_server)->getServID(),(*iter_host_server)));
		}
	}
	
	
	return;
}

void allocateVMRequestFFS(cVMRequest& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{
	multimap<double,cServer*> server_candidate_sorted;
	
	vector<cServer*> tobePlaced;

	double original_request = _vmrequest.getOriginalResRequest();
	double lambda = _vmrequest.getLambda();
	double svm_request;
	uint num_svm;
	TIME_T arrival_time,duration_time,departure_time;
	arrival_time = _vmrequest.getArrivalTime();
	duration_time = _vmrequest.getDurationTime();
	departure_time = _vmrequest.getDepartureTime();

	//find servers with enough capacity from those servers that have already been used.

	uint max_num_svm = 4;
	if (!_vmrequest.getVMRequestSplittable())
	{
		max_num_svm = 1;
	}
	for (num_svm = 1;num_svm<=max_num_svm;num_svm++)
	{
		tobePlaced.clear();
		server_candidate_sorted.clear();

		svm_request = (resourceRequirement[_vmrequest.getOriginalResRequest()])(_vmrequest.getOriginalResRequest(),_vmrequest.getLambda(),num_svm);
		map<ID,cServer*>::iterator iter_used_servers;
		for (iter_used_servers = usedServers.begin();iter_used_servers != usedServers.end();iter_used_servers++)
		{

			//find all feasible servers and rearrange them with ascending order based on their residual capacities
			if ((iter_used_servers->second)->enoughCapacity(arrival_time,departure_time,svm_request))
			{
				server_candidate_sorted.insert(make_pair(iter_used_servers->second->getTimeResidualCapacity(arrival_time,departure_time),iter_used_servers->second));
				//tobePlaced.push_back(iter_used_servers->second);
			}

			if (server_candidate_sorted.size() >= num_svm)
			{
				_vmrequest.setSVMNumber(num_svm);
				_vmrequest.setSVMResRequest(svm_request);
				uint svm_index = 0;
				multimap<double,cServer*>::iterator iter_server_tobeplaced;
				for (iter_server_tobeplaced = server_candidate_sorted.begin();iter_server_tobeplaced!=server_candidate_sorted.end();iter_server_tobeplaced++)
				{
					if (svm_index<num_svm)
					{
						(iter_server_tobeplaced->second)->setTimeResourceUsed(_vmrequest,svm_request);
						_vmrequest.host_server_vec.push_back(iter_server_tobeplaced->second);
						svm_index++;
					}
					else
					{
						break;
					}

				}

				return;
			}//have found enough resources available from already being used servers
		}

	}//

	//place the current request on to an unused server
	map<ID,cServer*>::iterator iter_unused_servers = unusedServers.begin();
	for (;iter_unused_servers != unusedServers.end();iter_unused_servers++)
	{

		if ((iter_unused_servers->second)->enoughCapacity(arrival_time,departure_time,_vmrequest.getOriginalResRequest()))
		{
			//iter_unused_servers->second->setServOccupied(iter_unused_servers->second->getServOccupied() + _vmrequest.getOriginalResRequest());
			iter_unused_servers->second->setTimeResourceUsed(_vmrequest,_vmrequest.getOriginalResRequest());
			_vmrequest.setSVMNumber(1);
			_vmrequest.setSVMResRequest(_vmrequest.getOriginalResRequest());
			_vmrequest.host_server_vec.push_back(iter_unused_servers->second);
			usedServers.insert(make_pair(iter_unused_servers->second->getServID(),iter_unused_servers->second));
			unusedServers.erase(iter_unused_servers);

			return;
		}
	}

	return;
}
