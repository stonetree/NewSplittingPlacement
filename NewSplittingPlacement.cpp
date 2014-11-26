// NewSplittingPlacement.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common.h"
#include "cVMRequest.h"
#include "cServer.h"

uint total_num_profile_55 = 100;
uint total_num_profile_70 = 0;
uint total_num_servers = 50;
double total_server_capacity = 100;
double splitable_percentage = 1;

double total_used = 0;
uint request_index = 0;

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

	for (;input_iterator != resource_request_vec.end();input_iterator++)
	{
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
		
		//allocateGlobalVMRequest(vmrequests_vec,server_vec,resource_request);
		for (iter_vm_request = vmrequests_vec.begin();iter_vm_request != vmrequests_vec.end();iter_vm_request++)
		{
			cout<<"The "<<request_index<<"th request"<<endl;
			allocateVMRequest(*iter_vm_request,server_vec,resource_request);
			//allocateVMRequestGreedy(*iter_vm_request,server_vec,resource_request);
			request_index++;
		}


		//collect output data
		outputResults((*input_iterator)[0],server_vec);
	}
		
	return 0;
}

