#include "cServer.h"
#include "cVMRequest.h"

class cEvent;


/************************************************************************/
/* For cplex                                                            */
typedef IloArray<IloIntVarArray>                      TwoDM;    /*2维整型变量数组*/
typedef IloArray<IloArray<IloIntVarArray>>            ThreeDM;  /*3维整型变量数组*/
typedef IloArray<IloArray<IloArray<IloIntVarArray>>>  FourDM;  /*4维整型变量数组*/
typedef IloArray<IloNumArray>                      TwoDMParam;    /*2维整型数组*/
/************************************************************************/

typedef double (*MappingFunction)(double _original,double _lambda,uint _num_svm);

extern uint total_num_profile_55;
extern uint total_num_profile_70;
extern double total_server_capacity;
extern uint total_num_servers;
extern double splitable_percentage;
extern uint total_time_slot;

extern double  lambda_low_55;
extern double  lambda_high_55;
extern double  lambda_low_70;
extern double  lambda_high_70;
extern double  lambda_low_80;
extern double  lambda_high_80;
extern double  lambda_low_90;
extern double  lambda_high_90;
extern double  lambda_low_100;
extern double  lambda_high_100;

extern double total_used;
extern uint request_index;

extern map<double,MappingFunction> resourceRequirement;
extern map<ID,cServer*> usedServers;
extern map<ID,cServer*> unusedServers;

extern void initializeServers(vector<cServer>& _server_vec);
extern void initializeVMRequests(vector<cVMRequest>& _vmrequests_vec);
extern void initializeResourceRequest(vector<double>& _input_cap,map<pair<double,uint>,double>& _resource_request);
extern void initializeResourceRequest();
extern void initializeEvent(multimap<uint,cEvent>& _event_map,vector<cVMRequest>& _request_vec);
extern void allocateVMRequest(cVMRequest& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request);
//extern void allocateGlobalVMRequest(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request);
extern void allocateVMRequestGreedy(cVMRequest& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request);
extern void outputResults(double _arai,vector<cServer>& _server_vec);
extern int initializeInputFile(vector<vector<double>>& _resource_request_vec);
extern void initializeInputParameters(vector<double>& _input_parameter);
extern void loadResourceRequirementfunc();
extern void releaseUsedResource(cVMRequest& _request,vector<cServer>& _server_vec);
extern void updateServWeight(cVMRequest& _request,vector<cServer>& _server_vec);
extern void updateServCandidate(cVMRequest& _vmrequest);

