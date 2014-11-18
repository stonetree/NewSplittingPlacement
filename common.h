#include "cServer.h"
#include "cVMRequest.h"




/************************************************************************/
/* For cplex                                                            */
typedef IloArray<IloIntVarArray>                      TwoDM;    /*2ά���ͱ�������*/
typedef IloArray<IloArray<IloIntVarArray>>            ThreeDM;  /*3ά���ͱ�������*/
typedef IloArray<IloArray<IloArray<IloIntVarArray>>>  FourDM;  /*4ά���ͱ�������*/
typedef IloArray<IloNumArray>                      TwoDMParam;    /*2ά���ͱ�������*/
/************************************************************************/

extern uint total_num_profile_55;
extern uint total_num_profile_70;
extern double total_server_capacity;
extern uint total_num_servers;
extern double splitable_percentage;

extern void initializeServers(vector<cServer>& _server_vec);
extern void initializeVMRequests(vector<cVMRequest>& _vmrequests_vec);
extern void initializeResourceRequest(vector<double>& _input_cap,map<pair<double,uint>,double>& _resource_request);
extern void allocateVMRequest(cVMRequest& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request);
extern void outputResults(double _arai,vector<cServer>& _server_vec);
extern int initializeInputFile(vector<vector<double>>& _resource_request_vec);