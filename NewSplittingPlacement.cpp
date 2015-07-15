// NewSplittingPlacement.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common.h"
#include "cVMRequest.h"
#include "cServer.h"
#include "cEvent.h"

/************************************************************************/
/* Simulation Configuration  */
/************************************************************************/

//uint total_num_profile_55 = 100;
//uint total_num_profile_70 = 0;
const TIME_T total_running_time =  100;

//indicate whether solving the offline optimal problem (1) or not (0)
const uint optimal_flag = 1;

uint total_num_servers = 10;

//will be set after the set of vm requests is initialized
uint total_requests = 0;


double server_capacity = 100;

//it's used when optimal solution is needed for online algorithm
//It's set "1" if optimal solution is needed or not
//right now it's not used
bool cplex_flag = false;

double splitable_percentage = -1;

double total_used = 0;
uint request_index = 0;

double arrival_rate_per_100 = 50;
double departure_rate = 1.0/20;

//uint total_time_slot = 60;
uint total_num = 1;

TIME_T cur_time = 0;
uint total_server_used = 0;

map<double,MappingFunction> resourceRequirement;
map<ID,cServer*> usedServers;
map<ID,cServer*> unusedServers;

double lambda_low_55 = 0.7;
double lambda_high_55 = 1.4;
double lambda_low_70 = 0.5;
double lambda_high_70 = 1.2;
double lambda_low_80 = 0.4;
double lambda_high_80 = 1;
double lambda_low_90 = 0.2;
double lambda_high_90 = 0.8;
double lambda_low_100 = 0.1;
double lambda_high_100 = 0.6;

int _tmain(int argc, _TCHAR* argv[])
{
	//store the resource required by the original vm or its svms
	vector<vector<double>> resource_request_vec;

	//input the info about resource requests
	if (initializeInputFile(resource_request_vec))
	{
		cout<<"Can not locate the input file"<<endl;
		exit(0);
	}

	vector<vector<double>>::iterator input_iterator = resource_request_vec.begin();

	double splittable_index = 0;
	for (;input_iterator != resource_request_vec.end();input_iterator++)
	{
		//iterations over the workload applications handled
		for (splittable_index = 0;splittable_index<1.1;splittable_index+=0.5)
		{
			//iterations over the proportion of splittable applications
			splitable_percentage = splittable_index;
			total_num = 10;
			//input_iterator = resource_request_vec.begin();
			for (arrival_rate_per_100 = 4;arrival_rate_per_100 <= 7; arrival_rate_per_100+=0.5)
			//iterations over the application arrival rates
			//for (total_num = 100;total_num<1001;total_num+=100)
			{
				//total_requests = total_num;
				//total_num_servers = total_requests;

				unusedServers.clear();
				usedServers.clear();
				//initialize parameters
				initializeInputParameters((*input_iterator));

				//servers
				vector<cServer>  server_vec;

				//Initialize servers
				initializeServers(server_vec);

				//vm requests
				vector<cVMRequest> vmrequests_vec;
				initializeVMRequests(vmrequests_vec);
				total_requests = vmrequests_vec.size();

				//resource requirement
				map<pair<double,uint>,double> resource_request;

				//initialize the resource request
				//initializeResourceRequest(*input_iterator,resource_request);
				initializeResourceRequest();

				vector<cVMRequest>::iterator iter_vm_request;

				//allocate vm requests
				request_index = 1;

				multimap<TIME_T,cEvent> event_map;
				initializeEvent(event_map,vmrequests_vec);

				if (optimal_flag == 1)
				{
					//solving the Integer linear programming problem
					//offline algorithm
					allocateVMRequest((*input_iterator)[0],vmrequests_vec,server_vec,resource_request);
				}
				else
				{
					//the VMSA algorithm
					//online algorithm
					multimap<TIME_T,cEvent>::iterator iter_event = event_map.begin();

					for(cur_time = 1;cur_time<=total_running_time;cur_time++)
					{
						if (iter_event != event_map.end())
						{
							if (cur_time>=iter_event->first)
							{

								if (iter_event->second.getEventType() == ARRIVAL)
								{
									cout<<"The "<<request_index<<"th request"<<endl;
									allocateVMRequestGreedy(*(iter_event->second.getRequestPoint()),server_vec,resource_request);
									//allocateVMRequestFFS(*(iter_event->second.getRequestPoint()),server_vec,resource_request);
									request_index++;
								}
								else
								{
									updateServCandidate(*(iter_event->second.getRequestPoint()));
								}

								iter_event++;
							}

						}

						vector<cServer>::iterator iter_server_vec = server_vec.begin();
						for (;iter_server_vec != server_vec.end();iter_server_vec++)
						{
							if (iter_server_vec->getServCapacity() - iter_server_vec->getResidual() >0)
							{
								total_server_used++;
							}
						}
					}
				}//execution for heuristic algorithm

				//collect output data
				outputResults((*input_iterator)[0],server_vec,vmrequests_vec);
				
				//reset the parameters
				total_server_used=0;
			}//for (arrival_rate_per_100
		}//For(splittable_index...
	}//for(input_file..
		
	return 0;
}

