#pragma once
#include "common.h"
#include "stdafx.h"

class cServer;
class cVMRequest;



class cCplexRuntime
{
public:
	IloEnv env;
	IloModel Model;
	IloCplex Cplex;
	FourDM   VarX;
	TwoDM    VarY;
	//IloIntVarArray VarZ;
	TwoDM VarZ;
	IloInt    ParamN;
	IloInt    m_SizeI;
	IloInt    m_SizeJ;
	IloInt    m_SizeK;
	IloInt    m_SizeH;
	TwoDMParam ParamR;
	IloNum     ParamC;

	void VarInit(cVMRequest& _request,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request);
	void VarInitGlobal(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request);
	void ModelConstruction(cVMRequest& _request,vector<cServer>& _server_vec);
	void ModelConstructionGlobal(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec);
	void ProblemSolve(cVMRequest& _request,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request);
	void ProblemSolveGlobal(vector<cVMRequest>& _request,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request);

public:
	cCplexRuntime(void);
	~cCplexRuntime(void);
};

