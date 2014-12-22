// NewSplittingPlacement.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common.h"
#include "cVMRequest.h"
#include "cServer.h"
#include "cEvent.h"

//uint total_num_profile_55 = 100;
//uint total_num_profile_70 = 0;
uint total_num_servers = 50;
uint total_requests = 100;
double total_server_capacity = 100;

double splitable_percentage = -1;

double total_used = 0;
uint request_index = 0;

uint total_time_slot = 60;
uint total_num = 1;

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
	for (splittable_index = 0;splittable_index<1.1;splittable_index+=0.5)
	{
		splitable_percentage = splittable_index;
		total_num = 10;
		input_iterator = resource_request_vec.begin();
		for (;input_iterator != resource_request_vec.end();input_iterator++)
		{
			for (total_num = 100;total_num<1001;total_num+=100)
			{
				total_requests = total_num;
				total_num_servers = total_requests;

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

				//resource requirement
				map<pair<double,uint>,double> resource_request;

				//initialize the resource request
				//initializeResourceRequest(*input_iterator,resource_request);
				initializeResourceRequest();

				vector<cVMRequest>::iterator iter_vm_request;

				//allocate vm requests
				request_index = 1;

				multimap<uint,cEvent> event_map;
				initializeEvent(event_map,vmrequests_vec);

				//allocateGlobalVMRequest(vmrequests_vec,server_vec,resource_request);

				multimap<uint,cEvent>::iterator iter_event = event_map.begin();
				for (;iter_event != event_map.end();iter_event++)
				{
					if (iter_event->second.getEventType() == ARRIVAL)
					{
						cout<<"The "<<request_index<<"th request"<<endl;
						//allocateVMRequest(*(iter_event->second.getRequestPoint()),server_vec,resource_request);
						allocateVMRequestGreedy(*(iter_event->second.getRequestPoint()),server_vec,resource_request);
						//allocateVMRequestFFS(*(iter_event->second.getRequestPoint()),server_vec,resource_request);
						request_index++;
					}
					else
					{
						updateServCandidate(*(iter_event->second.getRequestPoint()));
					}
				}
				//collect output data
				outputResults((*input_iterator)[0],server_vec,vmrequests_vec);
			}//for(total_num=10

		}//for(input_file...
	}//For(splittable_index...
	

		
	return 0;
}

