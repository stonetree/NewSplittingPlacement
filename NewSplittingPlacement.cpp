// NewSplittingPlacement.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common.h"
#include "cVMRequest.h"
#include "cServer.h"

uint total_num_profile_55 = 50;
uint total_num_profile_70 = 50;
uint total_num_servers = 100;
double total_server_capacity = 100;

int _tmain(int argc, _TCHAR* argv[])
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
	initializeResourceRequest(resource_request);

	vector<cVMRequest>::iterator iter_vm_request;

	for (iter_vm_request = vmrequests_vec.begin();iter_vm_request != vmrequests_vec.end();iter_vm_request++)
	{
		allocateVMRequest(*iter_vm_request,server_vec,resource_request);
	}

	ofstream result_output;
	result_output.open("output.txt",ios_base::app);
	if (!result_output)
	{
		cerr<<"unable to open output file: optimization.txt";
		exit(-1);
	} 

	uint used_server_count = 0;
	vector<cServer>::iterator iter_server = server_vec.begin();
	for (;iter_server != server_vec.end();iter_server++)
	{
		if (0 != iter_server->getServOccupied())
		{
			used_server_count++;
		}
	}
	result_output<<used_server_count<<endl;
	result_output.close();
	
	return 0;
}

