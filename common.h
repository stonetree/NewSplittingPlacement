#include "cServer.h"
#include "cVMRequest.h"


extern uint total_num_profile_55;
extern uint total_num_profile_70;
extern uint total_server_capacity;
extern uint total_num_servers;

extern void initializeServers(vector<cServer>& _server_vec);
extern void initializeVMRequests(vector<cVMRequest>& _vmrequests_vec);
extern void initializeResourceRequest(map<pair<uint,uint>,double>& _resource_request);
extern void allocateVMRequest(cVMRequest& _vmrequest,vector<cServer>& _server_vec);