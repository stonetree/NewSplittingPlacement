#include "cServer.h"
#include "cVMRequest.h"




/************************************************************************/
/* For cplex                                                            */
typedef IloArray<IloIntVarArray>                      TwoDM;    /*2维整型变量数组*/
typedef IloArray<IloArray<IloIntVarArray>>            ThreeDM;  /*3维整型变量数组*/
typedef IloArray<IloArray<IloArray<IloIntVarArray>>>  FourDM;  /*4维整型变量数组*/
typedef IloArray<IloNumArray>                      TwoDMParam;    /*2维整型变量数组*/
/************************************************************************/

extern uint total_num_profile_55;
extern uint total_num_profile_70;
extern double total_server_capacity;
extern uint total_num_servers;

extern void initializeServers(vector<cServer>& _server_vec);
extern void initializeVMRequests(vector<cVMRequest>& _vmrequests_vec);
extern void initializeResourceRequest(map<pair<double,uint>,double>& _resource_request);
extern void allocateVMRequest(cVMRequest& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request);