// NewSplittingPlacement.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common.h"
#include "cVMRequest.h"
#include "cServer.h"

uint total_num_profile_55 = 50;
uint total_num_profile_70 = 0;
uint total_num_servers = 50;
double total_server_capacity = 100;
double splitable_percentage = 1;

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
		initializeResourceRequest(*input_iterator,resource_request);

		vector<cVMRequest>::iterator iter_vm_request;

		//allocate vm requests
		for (iter_vm_request = vmrequests_vec.begin();iter_vm_request != vmrequests_vec.end();iter_vm_request++)
		{
			allocateVMRequest(*iter_vm_request,server_vec,resource_request);
		}


		//collect output data
		outputResults((*input_iterator)[0],server_vec);
	}
		
	return 0;
}

