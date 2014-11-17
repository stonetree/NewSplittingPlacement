// NewSplittingPlacement.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "common.h"
#include "cVMRequest.h"
#include "cServer.h"

uint total_num_profile_55 = 50;
uint total_num_profile_70 = 50;
uint total_num_servers = 100;
uint total_server_capacity = 100;

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
	map<pair<uint,uint>,double> resource_request;
	
	//initialize the resource request
	initializeResourceRequest(resource_request);

	vector<cVMRequest>::iterator iter_vm_request;

	for (iter_vm_request = vmrequests_vec.begin();iter_vm_request != vmrequests_vec.end();iter_vm_request++)
	{
		allocateVMRequest(*iter_vm_request,server_vec);
	}

	
	return 0;
}

