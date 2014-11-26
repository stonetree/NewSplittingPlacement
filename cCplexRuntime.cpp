#include "StdAfx.h"
#include "cCplexRuntime.h"
#include "common.h"

const IloInt LARGENUM = 10000;

double splitting_K_1 = 0;
double splitting_K_2 = 0;
double splitting_K_3 = 0;
double splitting_K_4 = 0;

double last_total_used;
double total_server_used = 0;

const double EulerConstant = std::exp(1.0);

cCplexRuntime::cCplexRuntime(void)
{
	Model = IloModel(env);
	Cplex = IloCplex(env);
	ParamN=5;
	ParamC=100;
}

cCplexRuntime::~cCplexRuntime(void)
{
	Model.end();
	Cplex.end();
	env.end();
}

/*************************************************************************/
/* Function name:  void RA::VarInit()                                    */ 
/* Precondition:  complete the definition of parameters and varieties    */ 
/* Input: None  Output: initialized  parameters and varieties            */ 
/* Function: Initialize the parameters and the varieties                 */
/*************************************************************************/
void cCplexRuntime::VarInit(cVMRequest& _request,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{
	IloInt i,j,k,h;

	uint total_request = 1;

	splitting_K_1 = _request.getOriginalResRequest();
	double original_request = _request.getOriginalResRequest();

	double lambda = _request.getLambda();

	//the number of all requirements
	m_SizeI = total_request;

	//the number of all bins
	m_SizeJ = _server_vec.size();

	//the limit of the number of splitting bin
	m_SizeK=4;

	//the index of split sub-requirement for each requirement 
	m_SizeH=4;


	//variety x means which bin the sub-requirement of each requirement is placed   
	VarX = FourDM(env,m_SizeI);
	for (i=0;i<m_SizeI;i++)
	{
		VarX[i] = ThreeDM(env,m_SizeJ);
		for (j=0;j<m_SizeJ;j++)
		{
			VarX[i][j] = TwoDM(env,m_SizeK);
			for (k=0;k<m_SizeK;k++)
			{
				VarX[i][j][k]=IloIntVarArray(env,m_SizeH,0,1);
			}
		}
	}

	//variety y means which splitting strategy is chosen, and only ONE splitting strategy is chosen for each requirement 
	VarY = TwoDM(env,m_SizeI);
	for (i=0;i<m_SizeI;i++)
	{
		VarY[i]=IloIntVarArray(env,m_SizeK,0,1);
	}

	//variety z indicate which bins have been used
	VarZ = IloIntVarArray(env,m_SizeJ,0,1);
	//VarZ = TwoDMParam(env,m_SizeJ);
	

	//parameter R indicates the resources requirement of each sub or original requirement
	//the data is obtained from the previous experiments
	ParamR=TwoDMParam(env,m_SizeI);
	for (i=0;i<m_SizeI;i++)
	{
		//ParamR[i]=IloNumArray(env,m_SizeK,0,1);
		ParamR[i]=IloNumArray(env,m_SizeK);
	}


	for (i = 0; i < total_request; i++)
	{
		for (k = 0; k<m_SizeK;k++)
		{
			//ParamR[i][k]=_resource_request[make_pair(splitting_K_1,k+1)];
			double tem = (resourceRequirement[original_request])(original_request,lambda,k+1);
			ParamR[i][k]= tem;
		}
	}

}


/*************************************************************************/
/* Function name:  void RA::VarInit()                                    */ 
/* Precondition:  complete the definition of parameters and varieties    */ 
/* Input: None  Output: initialized  parameters and varieties            */ 
/* Function: Initialize the parameters and the varieties                 */
/*************************************************************************/
void cCplexRuntime::VarInitGlobal(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{
	IloInt i,j,k,h;

	uint total_request = _vmrequest.size();

	//splitting_K_1 = _request.getOriginalResRequest();
	//double original_request = _request.getOriginalResRequest();

	//double lambda = _request.getLambda();

	//the number of all requirements
	m_SizeI = total_request;

	//the number of all bins
	m_SizeJ = _server_vec.size();

	//the limit of the number of splitting bin
	m_SizeK=4;

	//the index of split sub-requirement for each requirement 
	m_SizeH=4;


	//variety x means which bin the sub-requirement of each requirement is placed   
	VarX = FourDM(env,m_SizeI);
	for (i=0;i<m_SizeI;i++)
	{
		VarX[i] = ThreeDM(env,m_SizeJ);
		for (j=0;j<m_SizeJ;j++)
		{
			VarX[i][j] = TwoDM(env,m_SizeK);
			for (k=0;k<m_SizeK;k++)
			{
				VarX[i][j][k]=IloIntVarArray(env,m_SizeH,0,1);
			}
		}
	}

	//variety y means which splitting strategy is chosen, and only ONE splitting strategy is chosen for each requirement 
	VarY = TwoDM(env,m_SizeI);
	for (i=0;i<m_SizeI;i++)
	{
		VarY[i]=IloIntVarArray(env,m_SizeK,0,1);
	}

	//variety z indicate which bins have been used
	VarZ = IloIntVarArray(env,m_SizeJ,0,1);

	//parameter R indicates the resources requirement of each sub or original requirement
	//the data is obtained from the previous experiments
	ParamR=TwoDMParam(env,m_SizeI);
	for (i=0;i<m_SizeI;i++)
	{
		//ParamR[i]=IloNumArray(env,m_SizeK,0,1);
		ParamR[i]=IloNumArray(env,m_SizeK);
	}


	//if (num_profile_55 != 0)
	{
		for (i = 0; i < total_request; i++)
		{
			for (k = 0; k<m_SizeK;k++)
			{
				//ParamR[i][k]=_resource_request[make_pair(splitting_K_1,k+1)];
				double tem = (resourceRequirement[_vmrequest[i].getOriginalResRequest()])(_vmrequest[i].getOriginalResRequest(),_vmrequest[i].getLambda(),k+1);
				ParamR[i][k]= tem;
			}
		}
	}

}

/**************************************************************************/
/* Function name:  void RA::ModelConstruction()                           */ 
/* Precondition:  complete the initialization of parameters and varieties */ 
/* Input: None  Output: the completed problem model including the con-    */
/* trains and the objective function                                      */ 
/* Function: build the mathematical model for cplex to resolve            */
/**************************************************************************/
void cCplexRuntime::ModelConstruction(cVMRequest& _request,vector<cServer>& _server_vec)
{
	cout<<"Model Construction Beginning,Please wait......"<<endl;

	//not to be used
	IloInt i,j,k,h;

	/*************************************************************************/
	/* the 1st constraint                                                  */
	/* the total amount of packets put in a common bin must be less than the */
	/* capacity of bin                                                       */
	/*************************************************************************/

	for (j=0;j<m_SizeJ;j++)
	{
		IloExpr expr1(env);
		IloExpr expr2(env);
		for (i=0;i<m_SizeI;i++)
		{
			for (k=0;k<m_SizeK;k++)
			{
				for (h=0;h<=k;h++)
					expr1 += VarX[i][j][k][h] * ParamR[i][k];
			}
		}
		expr2=_server_vec[j].getRemainingCapacity()*VarZ[j];
		//expr2=_server_vec[j].getRemainingCapacity();
		Model.add(expr1<=expr2);
		expr1.end();
		expr2.end();
	}

	/*************************************************************************/
	/* the 2nd constraint                                                 */
	/* for each sub or original requirement, there is only ONE bin to be     */
	/* placed                                                                */
	/*************************************************************************/



	for (i=0;i<m_SizeI;i++)
	{
		for (k=0;k<m_SizeK;k++)
		{
			for (h=0;h<=k;h++)
			{
				IloExpr expr3(env);
				IloExpr expr4(env);
				for (j=0;j<m_SizeJ;j++)
				{
					expr3+=VarX[i][j][k][h];
				}
				expr4=VarY[i][k];
				Model.add(expr3==expr4);
				expr3.end();
				expr4.end();
			}

			for (;h<m_SizeK;h++)
			{
				IloExpr expr03(env);
				for (j=0;j<m_SizeJ;j++)
				{
					expr03+=VarX[i][j][k][h];
				}
				Model.add(expr03 == 0);
				expr03.end();
			}
		}
	}


	/*************************************************************************/
	/* the 3rd constraint                                                  */
	/* restricting the variety x to be zero if the sub-requirement index is  */
	/* large beyond the total sub requirements of each requirement after     */
	/* being split                                                           */
	/*************************************************************************/

	for (i=0;i<m_SizeI;i++)
	{
		for (k=0;k<m_SizeK;k++)
		{
			IloExpr expr5(env);
			IloExpr expr6(env);
			for (h=0;h<m_SizeH;h++)
			{
				for (j=0;j<m_SizeJ;j++)
				{
					expr5+=VarX[i][j][k][h];
				}
			}
			expr6=(k+1)*VarY[i][k];
			Model.add(expr5==expr6);
			expr5.end();
			expr6.end();
		}
	}
	/************************************************************************/
	/* the 4th constraint                                                 */
	/* it's no longer to be used                                            */
	/************************************************************************/

	//for (i=0;i<m_SizeI;i++)
	//{
	//	for (k=0;k<m_SizeK;k++)
	//	{
	//		for (h=0;h<m_SizeH-1;h++)
	//		{
	//			IloExpr expr7(env);
	//			IloExpr expr8(env);
	//			for (j=0;j<m_SizeJ;j++)
	//			{
	//				expr7+=VarX[i][j][k][h+1];
	//				expr8+=VarX[i][j][k][h];
	//			}
	//			Model.add(expr7<=expr8);
	//			expr7.end();
	//			expr8.end();
	//		}
	//	}
	//}

	/************************************************************************/
	/* the 5th constraint                                                   */
	/* restricting only ONE splitting strategy is chosen for each           */
	/*  requirement                                                         */
	/************************************************************************/

	for (i=0;i<m_SizeI;i++)
	{
		//if the request is non-splittable the original resource is the only choice available
		if (!_request.getVMRequestSplittable())
		{
			Model.add(VarY[i][0]==1);
		}

		IloExpr expr9(env);
		for (k=0;k<m_SizeK;k++)
		{
			expr9+=VarY[i][k];
		}
		Model.add(expr9==1);
		expr9.end();
	}

	/************************************************************************/
	/* the 6th constraint                                                   */
	/* restricting at most only one server can be used for each            */
	/* application                                                         */
	/************************************************************************/

	for (i=0;i<m_SizeI;i++)
	{
		for (j=0;j<m_SizeJ;j++)
		{
			for (k=0;k<m_SizeK;k++)
			{
				IloExpr expr10(env);
				for (h=0;h<m_SizeH;h++)
				{
					expr10+=VarX[i][j][k][h];
				}
				Model.add(expr10<=1);
				expr10.end();
			}
		}

	}

	/************************************************************************/
	/* the objective function                                               */
	/* minimizing the total amount of bins in use                           */
	/************************************************************************/

	IloExpr obj(env);

	for (i=0;i<m_SizeI;i++)
	{
		for (j=0;j<m_SizeJ;j++)
		{
			for (k=0;k<m_SizeK;k++)
			{
				for (h=0;h<m_SizeH;h++)
				{
					obj += VarX[i][j][k][h]*ParamR[i][k]*(_server_vec[j]).getServWeight();
				}
			}
		}
	}

	Model.add(IloMinimize(env,obj));
	obj.end();
	cout<<"Model Construction is successful"<<endl;
}

void cCplexRuntime::ModelConstructionGlobal(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec)
{
	cout<<"Model Construction Beginning,Please wait......"<<endl;

	//not to be used
	IloInt i,j,k,h;

	/*************************************************************************/
	/* the 1st constraint                                                  */
	/* the total amount of packets put in a common bin must be less than the */
	/* capacity of bin                                                       */
	/*************************************************************************/

	for (j=0;j<m_SizeJ;j++)
	{
		IloExpr expr1(env);
		IloExpr expr2(env);
		for (i=0;i<m_SizeI;i++)
		{
			for (k=0;k<m_SizeK;k++)
			{
				for (h=0;h<=k;h++)
					expr1 += VarX[i][j][k][h] * ParamR[i][k];
			}
		}
		expr2=_server_vec[j].getServCapacity()*VarZ[j];
		//expr2=_server_vec[j].getRemainingCapacity();
		Model.add(expr1<=expr2);
		expr1.end();
		expr2.end();
	}

	/*************************************************************************/
	/* the 2nd constraint                                                 */
	/* for each sub or original requirement, there is only ONE bin to be     */
	/* placed                                                                */
	/*************************************************************************/



	for (i=0;i<m_SizeI;i++)
	{
		for (k=0;k<m_SizeK;k++)
		{
			for (h=0;h<=k;h++)
			{
				IloExpr expr3(env);
				IloExpr expr4(env);
				for (j=0;j<m_SizeJ;j++)
				{
					expr3+=VarX[i][j][k][h];
				}
				expr4=VarY[i][k];
				Model.add(expr3==expr4);
				expr3.end();
				expr4.end();
			}

			for (;h<m_SizeK;h++)
			{
				IloExpr expr03(env);
				for (j=0;j<m_SizeJ;j++)
				{
					expr03+=VarX[i][j][k][h];
				}
				Model.add(expr03 == 0);
				expr03.end();
			}
		}
	}


	/*************************************************************************/
	/* the 3rd constraint                                                  */
	/* restricting the variety x to be zero if the sub-requirement index is  */
	/* large beyond the total sub requirements of each requirement after     */
	/* being split                                                           */
	/*************************************************************************/

	for (i=0;i<m_SizeI;i++)
	{
		for (k=0;k<m_SizeK;k++)
		{
			IloExpr expr5(env);
			IloExpr expr6(env);
			for (h=0;h<m_SizeH;h++)
			{
				for (j=0;j<m_SizeJ;j++)
				{
					expr5+=VarX[i][j][k][h];
				}
			}
			expr6=(k+1)*VarY[i][k];
			Model.add(expr5==expr6);
			expr5.end();
			expr6.end();
		}
	}
	/************************************************************************/
	/* the 4th constraint                                                 */
	/* it's no longer to be used                                            */
	/************************************************************************/

	//for (i=0;i<m_SizeI;i++)
	//{
	//	for (k=0;k<m_SizeK;k++)
	//	{
	//		for (h=0;h<m_SizeH-1;h++)
	//		{
	//			IloExpr expr7(env);
	//			IloExpr expr8(env);
	//			for (j=0;j<m_SizeJ;j++)
	//			{
	//				expr7+=VarX[i][j][k][h+1];
	//				expr8+=VarX[i][j][k][h];
	//			}
	//			Model.add(expr7<=expr8);
	//			expr7.end();
	//			expr8.end();
	//		}
	//	}
	//}

	/************************************************************************/
	/* the 5th constraint                                                   */
	/* restricting only ONE splitting strategy is chosen for each           */
	/*  requirement                                                         */
	/************************************************************************/

	for (i=0;i<m_SizeI;i++)
	{
		//if the request is non-splittable the original resource is the only choice available
		if (!_vmrequest[i].getVMRequestSplittable())
		{
			Model.add(VarY[i][0]==1);
		}

		IloExpr expr9(env);
		for (k=0;k<m_SizeK;k++)
		{
			expr9+=VarY[i][k];
		}
		Model.add(expr9==1);
		expr9.end();
	}

	/************************************************************************/
	/* the 6th constraint                                                   */
	/* restricting at most only one server can be used for each            */
	/* application                                                         */
	/************************************************************************/

	for (i=0;i<m_SizeI;i++)
	{
		for (j=0;j<m_SizeJ;j++)
		{
			for (k=0;k<m_SizeK;k++)
			{
				IloExpr expr10(env);
				for (h=0;h<m_SizeH;h++)
				{
					expr10+=VarX[i][j][k][h];
				}
				Model.add(expr10<=1);
				expr10.end();
			}
		}

	}

	/************************************************************************/
	/* the objective function                                               */
	/* minimizing the total amount of bins in use                           */
	/************************************************************************/

	IloExpr obj(env);
	for (j=0;j<m_SizeJ;j++)
	{
		obj += VarZ[j];
	}

	Model.add(IloMinimize(env,obj));
	obj.end();
	cout<<"Model Construction is successful"<<endl;
}

void cCplexRuntime::ProblemSolve(cVMRequest& _request,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{

	IloInt i,j,k,h;

	ofstream result_output;
	result_output.open("cplex_output.txt",ios_base::app);
	if (!result_output)
	{
		cerr<<"unable to open output file: optimization.txt";
		exit(-1);
	} 

	cout<<"Problem solving is beginning......"<<endl;

	Cplex.extract(Model);
	//Cplex.exportModel("NR-MF.lp");
	IloBool  flag = Cplex.solve();
	if (flag == IloTrue)
	{
		cout<<"The End....."<<endl;
	}
	env.out() << "Solution status = " << Cplex.getStatus() << endl;
	env.out() << "Solution value = " << Cplex.getObjValue() << endl;

	result_output<<" "<<Cplex.getStatus()<<" "<<Cplex.getObjValue()<<endl;
	result_output.close();

	//get placement variables
	total_server_used = 0;
	for (i=0;i<m_SizeI;i++)
	{
		for (k=0;k<m_SizeK;k++)
		{
			for (h=0;h<m_SizeH;h++)					
			{
				for (j=0;j<m_SizeJ;j++)
				{
					total_server_used += Cplex.getValue(VarX[i][j][k][h]);
					if (Cplex.getValue(VarX[i][j][k][h])>0.5)
					{
						total_used += (resourceRequirement[_request.getOriginalResRequest()])(_request.getOriginalResRequest(),_request.getLambda(),k+1);
						//(_server_vec[j]).setServOccupied((_server_vec[j]).getServOccupied()+_resource_request[make_pair(_request.getOriginalResRequest(),(uint)(k+1))]);
						(_server_vec[j]).setServOccupied((_server_vec[j]).getServOccupied()+(resourceRequirement[_request.getOriginalResRequest()])(_request.getOriginalResRequest(),_request.getLambda(),k+1));			
						(_server_vec[j]).setServWeight(1/(total_server_capacity*exp(((_server_vec[j]).getServOccupied()/total_server_capacity))));
						//(_server_vec[j]).setServWeight(1/(total_server_capacity*log(EulerConstant + ((_server_vec[j]).getServOccupied()/total_server_capacity))));
						//double new_weight = log(1 + 0.01 + ((_server_vec[j]).getServOccupied()/total_server_capacity)) / log(1 + 0.01);
						//(_server_vec[j]).setServWeight(1/(total_server_capacity*new_weight));
					}
					
				}
			}
		}
	}

	double current_used = total_used - last_total_used;
	last_total_used = total_used;
	Cplex.clear();
	Model.end();
	Cplex.end();
	env.end();

	return;

}

void cCplexRuntime::ProblemSolveGlobal(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{

	IloInt i,j,k,h;

	ofstream result_output;
	result_output.open("cplex_output.txt",ios_base::app);
	if (!result_output)
	{
		cerr<<"unable to open output file: optimization.txt";
		exit(-1);
	} 

	cout<<"Problem solving is beginning......"<<endl;

	Cplex.extract(Model);
	//Cplex.exportModel("NR-MF.lp");
	IloBool  flag = Cplex.solve();
	if (flag == IloTrue)
	{
		cout<<"The End....."<<endl;
	}
	env.out() << "Solution status = " << Cplex.getStatus() << endl;
	env.out() << "Solution value = " << Cplex.getObjValue() << endl;

	result_output<<" "<<Cplex.getStatus()<<" "<<Cplex.getObjValue()<<endl;
	result_output.close();

	Cplex.clear();
	Model.end();
	Cplex.end();
	env.end();

	return;

}
