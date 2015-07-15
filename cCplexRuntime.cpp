/************************************************************************/
/* It's an OFFLINE optimal Integer Linear Programming model             */
/************************************************************************/


#include "StdAfx.h"
#include "cCplexRuntime.h"
#include "common.h"



const IloInt LARGENUM = 10000;

double splitting_K_1 = 0;
double splitting_K_2 = 0;
double splitting_K_3 = 0;
double splitting_K_4 = 0;

double last_total_used = 0;


const double EulerConstant = std::exp(1.0);

static uint residual_time_slot = 0;

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
void cCplexRuntime::VarInit(vector<cVMRequest>& _vmrequest_vec,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{
	IloInt i,j,k,h;

	//splitting_K_1 = _request.getOriginalResRequest();
	//double original_request = _request.getOriginalResRequest();

	//double lambda = _request.getLambda();

	//the number of all requirements
	m_SizeI = _vmrequest_vec.size();

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

	/************************************************************************/
	/* Z might be a variable with two dimensions if it indicates whether a  */ 
	/* server is used at any time                                           */
	/************************************************************************/
	
	//variety z indicate which bins have been used
	//VarZ = IloIntVarArray(env,m_SizeJ,0,1);
	VarZ = TwoDM(env,m_SizeJ);
	for (j = 0;j<m_SizeJ;j++)
	{
		//VarZ[j]=IloIntVarArray(env,_request.getDurationTime(),0,1);
		VarZ[j]=IloIntVarArray(env,(uint)total_running_time,0,1);
	}
	

	//parameter R indicates the resources requirement of each sub or original requirement
	//the data is obtained from the previous experiments
	ParamR=TwoDMParam(env,m_SizeI);
	for (i=0;i<m_SizeI;i++)
	{
		//ParamR[i]=IloNumArray(env,m_SizeK,0,1);
		ParamR[i]=IloNumArray(env,m_SizeK);
	}


	for (i = 0; i < _vmrequest_vec.size(); i++)
	{
		for (k = 0; k<m_SizeK;k++)
		{
			//ParamR[i][k]=_resource_request[make_pair(splitting_K_1,k+1)];
			double tem = (resourceRequirement[_vmrequest_vec[i].getOriginalResRequest()])(_vmrequest_vec[i].getOriginalResRequest(),_vmrequest_vec[i].getLambda(),k+1);
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
//void cCplexRuntime::VarInitGlobal(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
//{
//	IloInt i,j,k,h;
//
//	uint total_request = _vmrequest.size();
//
//	//splitting_K_1 = _request.getOriginalResRequest();
//	//double original_request = _request.getOriginalResRequest();
//
//	//double lambda = _request.getLambda();
//
//	//the number of all requirements
//	m_SizeI = total_request;
//
//	//the number of all bins
//	m_SizeJ = _server_vec.size();
//
//	//the limit of the number of splitting bin
//	m_SizeK=4;
//
//	//the index of split sub-requirement for each requirement 
//	m_SizeH=4;
//
//
//	//variety x means which bin the sub-requirement of each requirement is placed   
//	VarX = FourDM(env,m_SizeI);
//	for (i=0;i<m_SizeI;i++)
//	{
//		VarX[i] = ThreeDM(env,m_SizeJ);
//		for (j=0;j<m_SizeJ;j++)
//		{
//			VarX[i][j] = TwoDM(env,m_SizeK);
//			for (k=0;k<m_SizeK;k++)
//			{
//				VarX[i][j][k]=IloIntVarArray(env,m_SizeH,0,1);
//			}
//		}
//	}
//
//	//variety y means which splitting strategy is chosen, and only ONE splitting strategy is chosen for each requirement 
//	VarY = TwoDM(env,m_SizeI);
//	for (i=0;i<m_SizeI;i++)
//	{
//		VarY[i]=IloIntVarArray(env,m_SizeK,0,1);
//	}
//
//	//variety z indicate which bins have been used
//	VarZ = TwoDM(env,m_SizeJ);
//	residual_time_slot = total_request - _request.getDepartureTime();
//	for (j = 0;j<m_SizeJ;j++)
//	{
//		VarZ[j]=IloIntVarArray(env,residual_time_slot,0,1);
//	}
//
//	//parameter R indicates the resources requirement of each sub or original requirement
//	//the data is obtained from the previous experiments
//	ParamR=TwoDMParam(env,m_SizeI);
//	for (i=0;i<m_SizeI;i++)
//	{
//		//ParamR[i]=IloNumArray(env,m_SizeK,0,1);
//		ParamR[i]=IloNumArray(env,m_SizeK);
//	}
//
//
//	//if (num_profile_55 != 0)
//	{
//		for (i = 0; i < total_request; i++)
//		{
//			for (k = 0; k<m_SizeK;k++)
//			{
//				//ParamR[i][k]=_resource_request[make_pair(splitting_K_1,k+1)];
//				double tem = (resourceRequirement[_vmrequest[i].getOriginalResRequest()])(_vmrequest[i].getOriginalResRequest(),_vmrequest[i].getLambda(),k+1);
//				ParamR[i][k]= tem;
//			}
//		}
//	}
//
//}

///**************************************************************************/
///* Function name:  void RA::ModelConstruction()                           */ 
///* Precondition:  complete the initialization of parameters and varieties */ 
///* Input: None  Output: the completed problem model including the con-    */
///* trains and the objective function                                      */ 
///* Function: build the mathematical model for cplex to resolve            */
///**************************************************************************/
//void cCplexRuntime::ModelConstruction(cVMRequest& _request,vector<cServer>& _server_vec)
//{
//	cout<<"Model Construction Beginning,Please wait......"<<endl;
//
//	//not to be used
//	IloInt i,j,k,h;
//
//	/*************************************************************************/
//	/* the 1st constraint                                                  */
//	/* the total amount of packets put in a common bin must be less than the */
//	/* capacity of bin                                                       */
//	/*************************************************************************/
//
//	uint duration_time = _request.getDurationTime();
//	uint arrival_time = _request.getArrivalTime();
//	
//	uint time_slot;
//	for (j=0;j<m_SizeJ;j++)
//	{
//		IloExpr expr1(env);
//		for (i=0;i<m_SizeI;i++)
//		{
//			for (k=0;k<m_SizeK;k++)
//			{
//				for (h=0;h<=k;h++)
//					expr1 += VarX[i][j][k][h] * ParamR[i][k];
//			}
//		}
//		for (time_slot = 0;time_slot<duration_time;time_slot++)
//		{
//			IloExpr expr2(env);
//			expr2=_server_vec[j].getTimeResidualCapacity(arrival_time + time_slot)*VarZ[j][time_slot];
//			//expr2=_server_vec[j].getRemainingCapacity();
//			Model.add(expr1<=expr2);
//			expr2.end();
//		}
//		expr1.end();
//	}
//
//	/*************************************************************************/
//	/* the 1.5st constraint                                                  */
//	/* the server that has been already used by previous app should be set   */
//	/* available for current vm request                                      */
//	/*************************************************************************/
//
//	duration_time = _request.getDurationTime();
//	arrival_time = _request.getArrivalTime();
//	for (j=0;j<m_SizeJ;j++)
//	{
//		for (time_slot = 0;time_slot<duration_time;time_slot++)
//		{
//			if (_server_vec[j].getTimeResidualCapacity(arrival_time + time_slot) < _server_vec[j].getServCapacity())
//			{
//				Model.add(VarZ[j][time_slot] == 1);
//			}
//		}
//	}
//
//	/*************************************************************************/
//	/* the 2nd constraint                                                 */
//	/* for each sub or original requirement, there is only ONE bin to be     */
//	/* placed                                                                */
//	/*************************************************************************/
//
//
//
//	for (i=0;i<m_SizeI;i++)
//	{
//		for (k=0;k<m_SizeK;k++)
//		{
//			for (h=0;h<=k;h++)
//			{
//				IloExpr expr3(env);
//				IloExpr expr4(env);
//				for (j=0;j<m_SizeJ;j++)
//				{
//					expr3+=VarX[i][j][k][h];
//				}
//				expr4=VarY[i][k];
//				Model.add(expr3==expr4);
//				expr3.end();
//				expr4.end();
//			}
//
//			for (;h<m_SizeK;h++)
//			{
//				IloExpr expr03(env);
//				for (j=0;j<m_SizeJ;j++)
//				{
//					expr03+=VarX[i][j][k][h];
//				}
//				Model.add(expr03 == 0);
//				expr03.end();
//			}
//		}
//	}
//
//
//	/*************************************************************************/
//	/* the 3rd constraint                                                  */
//	/* restricting the variety x to be zero if the sub-requirement index is  */
//	/* large beyond the total sub requirements of each requirement after     */
//	/* being split                                                           */
//	/*************************************************************************/
//
//	for (i=0;i<m_SizeI;i++)
//	{
//		for (k=0;k<m_SizeK;k++)
//		{
//			IloExpr expr5(env);
//			IloExpr expr6(env);
//			for (h=0;h<m_SizeH;h++)
//			{
//				for (j=0;j<m_SizeJ;j++)
//				{
//					expr5+=VarX[i][j][k][h];
//				}
//			}
//			expr6=(k+1)*VarY[i][k];
//			Model.add(expr5==expr6);
//			expr5.end();
//			expr6.end();
//		}
//	}
//	/************************************************************************/
//	/* the 4th constraint                                                 */
//	/* it's no longer to be used                                            */
//	/************************************************************************/
//
//	//for (i=0;i<m_SizeI;i++)
//	//{
//	//	for (k=0;k<m_SizeK;k++)
//	//	{
//	//		for (h=0;h<m_SizeH-1;h++)
//	//		{
//	//			IloExpr expr7(env);
//	//			IloExpr expr8(env);
//	//			for (j=0;j<m_SizeJ;j++)
//	//			{
//	//				expr7+=VarX[i][j][k][h+1];
//	//				expr8+=VarX[i][j][k][h];
//	//			}
//	//			Model.add(expr7<=expr8);
//	//			expr7.end();
//	//			expr8.end();
//	//		}
//	//	}
//	//}
//
//	/************************************************************************/
//	/* the 5th constraint                                                   */
//	/* restricting only ONE splitting strategy is chosen for each           */
//	/*  requirement                                                         */
//	/************************************************************************/
//
//	for (i=0;i<m_SizeI;i++)
//	{
//		//if the request is non-splittable the original resource is the only choice available
//		if (!_request.getVMRequestSplittable())
//		{
//			Model.add(VarY[i][0]==1);
//		}
//
//		IloExpr expr9(env);
//		for (k=0;k<m_SizeK;k++)
//		{
//			expr9+=VarY[i][k];
//		}
//		Model.add(expr9==1);
//		expr9.end();
//	}
//
//	/************************************************************************/
//	/* the 6th constraint                                                   */
//	/* restricting at most only one server can be used for each            */
//	/* application                                                         */
//	/************************************************************************/
//
//	for (i=0;i<m_SizeI;i++)
//	{
//		for (j=0;j<m_SizeJ;j++)
//		{
//			for (k=0;k<m_SizeK;k++)
//			{
//				IloExpr expr10(env);
//				for (h=0;h<m_SizeH;h++)
//				{
//					expr10+=VarX[i][j][k][h];
//				}
//				Model.add(expr10<=1);
//				expr10.end();
//			}
//		}
//
//	}
//
//	/************************************************************************/
//	/* the objective function                                               */
//	/* minimizing the total amount of bins in use                           */
//	/************************************************************************/
//
//	IloExpr obj(env);
//
//	//for (i=0;i<m_SizeI;i++)
//	//{
//	//	for (j=0;j<m_SizeJ;j++)
//	//	{
//	//		for (k=0;k<m_SizeK;k++)
//	//		{
//	//			for (h=0;h<m_SizeH;h++)
//	//			{
//
//	//				double tem = (_server_vec[j]).getTimeWeight(_request.getArrivalTime(),_request.getDurationTime())*(_request.getDurationTime());
//	//				obj += VarX[i][j][k][h]*ParamR[i][k]*tem;
//
//	//			}
//	//		}
//	//	}
//	//}
//
//	for (j=0;j<m_SizeJ;j++)
//	{
//		for (time_slot = 0;time_slot<duration_time;time_slot++)
//		{
//					obj += VarZ[j][time_slot];
//		}
//	}
//
//	Model.add(IloMinimize(env,obj));
//	obj.end();
//	cout<<"Model Construction is successful"<<endl;
//}

/**************************************************************************/
/* Function name:  void RA::ModelConstruction()                           */ 
/* Precondition:  complete the initialization of parameters and varieties */ 
/* Input: None  Output: the completed problem model including the con-    */
/* trains and the objective function                                      */ 
/* Function: build the mathematical model for cplex to resolve            */
/**************************************************************************/
void cCplexRuntime::ModelConstruction(vector<cVMRequest>& _request_vec,vector<cServer>& _server_vec)
//void cCplexRuntime::ModelConstruction(cVMRequest& _request,vector<cServer>& _server_vec)
{
	cout<<"Model Construction Beginning,Please wait......"<<endl;

	//not to be used
	IloInt i,j,k,h;

	/*************************************************************************/
	/* the 1st constraint                                                  */
	/* the total amount of packets put in a common bin must be less than the */
	/* capacity of bin                                                       */
	/*************************************************************************/

	TIME_T departure_time;
	TIME_T arrival_time;

	//TIME_T time_slot;
	TIME_T time;
	for (time = 0;time < total_running_time; time++)
	{
		for (j=0;j<m_SizeJ;j++)
		{
			IloExpr expr1(env);
			for (i=0;i<m_SizeI;i++)
			{
				departure_time = (TIME_T)_request_vec[i].getDepartureTime();
				arrival_time = (TIME_T)_request_vec[i].getArrivalTime();

				//If time is not in the range of the duration time of application i
				//ignore current application.
				if (!(time>=arrival_time)&&(time<=departure_time))
				{
					continue;
				}

				for (k=0;k<m_SizeK;k++)
				{
					for (h=0;h<=k;h++)
						expr1 += VarX[i][j][k][h] * ParamR[i][k];
				}
			}
			IloExpr expr2(env);
			expr2 = _server_vec[j].getServCapacity()*VarZ[j][(uint)time];
			Model.add(expr1<=expr2);
			expr2.end();
			expr1.end();
		}
	}


	/*************************************************************************/
	/* the 1.5st constraint                                                  */
	/* the server that has been already used by previous app should be set   */
	/* available for current vm request                                      */
	/*************************************************************************/
	
	//duration_time = _request.getDurationTime();
	//arrival_time = _request.getArrivalTime();
	//for (j=0;j<m_SizeJ;j++)
	//{
	//	if (_server_vec[j].getResidual()<_server_vec[j].getServCapacity() + 1)
	//	{
	//		Model.add(VarZ[j] == 1);
	//	}
	//	//for (time_slot = 0;time_slot<duration_time;time_slot++)
	//	//{
	//	//	if (_server_vec[j].getTimeResidualCapacity(arrival_time + time_slot) < _server_vec[j].getServCapacity())
	//	//	{
	//	//		Model.add(VarZ[j][time_slot] == 1);
	//	//	}
	//	//}
	//}

	/*************************************************************************/
	/* the 2nd constraint                                                 */
	/* for each sub SVM or the original LVM, there is only ONE physical server to be     */
	/* placed for each of which                                                               */
	/*************************************************************************/



	for (i=0;i<m_SizeI;i++)
	{
		for (k=0;k<m_SizeK;k++)
		{
			for (h=0;h<=k;h++)
			{
				IloExpr expr21(env);
				IloExpr expr22(env);
				for (j=0;j<m_SizeJ;j++)
				{
					expr21+=VarX[i][j][k][h];
				}
				expr22=VarY[i][k];
				Model.add(expr21==expr22);
				expr21.end();
				expr22.end();
			}


			
			for (;h<m_SizeK;h++)
			{
				IloExpr expr23(env);
				for (j=0;j<m_SizeJ;j++)
				{
					expr23+=VarX[i][j][k][h];
				}
				Model.add(expr23 == 0);
				expr23.end();
			}

		}
	}


	//no longer used
	//it's replaced by constrain 2 above
	///*************************************************************************/
	///* the 3rd constraint                                                  */
	///* restricting the variety x to be zero if the SVM index is  */
	///* large beyond the total number of SVMs after the application is split  */
	///*************************************************************************/

	//for (i=0;i<m_SizeI;i++)
	//{
	//	for (k=0;k<m_SizeK;k++)
	//	{
	//		IloExpr expr5(env);
	//		IloExpr expr6(env);
	//		for (h=0;h<m_SizeH;h++)
	//		{
	//			for (j=0;j<m_SizeJ;j++)
	//			{
	//				expr5+=VarX[i][j][k][h];
	//			}
	//		}
	//		expr6=(k+1)*VarY[i][k];
	//		Model.add(expr5==expr6);
	//		expr5.end();
	//		expr6.end();
	//	}
	//}
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
		if (!_request_vec[i].getVMRequestSplittable())
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
	/* SVM                                                       */
	/************************************************************************/

	//for (i=0;i<m_SizeI;i++)
	//{
	//	for (j=0;j<m_SizeJ;j++)
	//	{
	//		IloExpr expr61(env);
	//		for (k=0;k<m_SizeK;k++)
	//		{
	//			for (h=0;h<m_SizeH;h++)
	//			{
	//				expr61+=VarX[i][j][k][h];
	//			}
	//		}
	//		Model.add(expr61<=1);
	//		expr61.end();
	//	}

	//}

	/************************************************************************/
	/* the objective function                                               */
	/* minimizing the total amount of bins in use                           */
	/************************************************************************/

	IloExpr obj(env);

	//for (i=0;i<m_SizeI;i++)
	//{
	//	for (j=0;j<m_SizeJ;j++)
	//	{
	//		for (k=0;k<m_SizeK;k++)
	//		{
	//			for (h=0;h<m_SizeH;h++)
	//			{

	//				//double tem = (_server_vec[j]).getTimeWeight(_request.getArrivalTime(),_request.getDurationTime())*(_request.getDurationTime());
	//				obj += VarX[i][j][k][h]*ParamR[i][k]*VarZ[j];

	//			}
	//		}
	//	}
	//}

	for (time = 0;time<total_running_time;time++)
	{
		for (j=0;j<m_SizeJ;j++)
		{
			for (time = 0;time<total_running_time;time++)
			{
				obj += VarZ[j][(uint)time];
			}
		}
	}

	Model.add(IloMinimize(env,obj));
	obj.end();
	cout<<"Model Construction is successful"<<endl;
}

//void cCplexRuntime::ModelConstructionGlobal(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec)
//{
//	cout<<"Model Construction Beginning,Please wait......"<<endl;
//
//	//not to be used
//	IloInt i,j,k,h;
//
//	/*************************************************************************/
//	/* the 1st constraint                                                  */
//	/* the total amount of packets put in a common bin must be less than the */
//	/* capacity of bin                                                       */
//	/*************************************************************************/
//
//	for (j=0;j<m_SizeJ;j++)
//	{
//		IloExpr expr1(env);
//		IloExpr expr2(env);
//		for (i=0;i<m_SizeI;i++)
//		{
//			for (k=0;k<m_SizeK;k++)
//			{
//				for (h=0;h<=k;h++)
//					expr1 += VarX[i][j][k][h] * ParamR[i][k];
//			}
//		}
//		expr2=_server_vec[j].getServCapacity()*VarZ[j];
//		//expr2=_server_vec[j].getRemainingCapacity();
//		Model.add(expr1<=expr2);
//		expr1.end();
//		expr2.end();
//	}
//
//	/*************************************************************************/
//	/* the 2nd constraint                                                 */
//	/* for each sub or original requirement, there is only ONE bin to be     */
//	/* placed                                                                */
//	/*************************************************************************/
//
//
//
//	for (i=0;i<m_SizeI;i++)
//	{
//		for (k=0;k<m_SizeK;k++)
//		{
//			for (h=0;h<=k;h++)
//			{
//				IloExpr expr3(env);
//				IloExpr expr4(env);
//				for (j=0;j<m_SizeJ;j++)
//				{
//					expr3+=VarX[i][j][k][h];
//				}
//				expr4=VarY[i][k];
//				Model.add(expr3==expr4);
//				expr3.end();
//				expr4.end();
//			}
//
//			for (;h<m_SizeK;h++)
//			{
//				IloExpr expr03(env);
//				for (j=0;j<m_SizeJ;j++)
//				{
//					expr03+=VarX[i][j][k][h];
//				}
//				Model.add(expr03 == 0);
//				expr03.end();
//			}
//		}
//	}
//
//
//	/*************************************************************************/
//	/* the 3rd constraint                                                  */
//	/* restricting the variety x to be zero if the sub-requirement index is  */
//	/* large beyond the total sub requirements of each requirement after     */
//	/* being split                                                           */
//	/*************************************************************************/
//
//	for (i=0;i<m_SizeI;i++)
//	{
//		for (k=0;k<m_SizeK;k++)
//		{
//			IloExpr expr5(env);
//			IloExpr expr6(env);
//			for (h=0;h<m_SizeH;h++)
//			{
//				for (j=0;j<m_SizeJ;j++)
//				{
//					expr5+=VarX[i][j][k][h];
//				}
//			}
//			expr6=(k+1)*VarY[i][k];
//			Model.add(expr5==expr6);
//			expr5.end();
//			expr6.end();
//		}
//	}
//	/************************************************************************/
//	/* the 4th constraint                                                 */
//	/* it's no longer to be used                                            */
//	/************************************************************************/
//
//	//for (i=0;i<m_SizeI;i++)
//	//{
//	//	for (k=0;k<m_SizeK;k++)
//	//	{
//	//		for (h=0;h<m_SizeH-1;h++)
//	//		{
//	//			IloExpr expr7(env);
//	//			IloExpr expr8(env);
//	//			for (j=0;j<m_SizeJ;j++)
//	//			{
//	//				expr7+=VarX[i][j][k][h+1];
//	//				expr8+=VarX[i][j][k][h];
//	//			}
//	//			Model.add(expr7<=expr8);
//	//			expr7.end();
//	//			expr8.end();
//	//		}
//	//	}
//	//}
//
//	/************************************************************************/
//	/* the 5th constraint                                                   */
//	/* restricting only ONE splitting strategy is chosen for each           */
//	/*  requirement                                                         */
//	/************************************************************************/
//
//	for (i=0;i<m_SizeI;i++)
//	{
//		//if the request is non-splittable the original resource is the only choice available
//		if (!_vmrequest[i].getVMRequestSplittable())
//		{
//			Model.add(VarY[i][0]==1);
//		}
//
//		IloExpr expr9(env);
//		for (k=0;k<m_SizeK;k++)
//		{
//			expr9+=VarY[i][k];
//		}
//		Model.add(expr9==1);
//		expr9.end();
//	}
//
//	/************************************************************************/
//	/* the 6th constraint                                                   */
//	/* restricting at most only one server can be used for each            */
//	/* application                                                         */
//	/************************************************************************/
//
//	for (i=0;i<m_SizeI;i++)
//	{
//		for (j=0;j<m_SizeJ;j++)
//		{
//			for (k=0;k<m_SizeK;k++)
//			{
//				IloExpr expr10(env);
//				for (h=0;h<m_SizeH;h++)
//				{
//					expr10+=VarX[i][j][k][h];
//				}
//				Model.add(expr10<=1);
//				expr10.end();
//			}
//		}
//
//	}
//
//	/************************************************************************/
//	/* the objective function                                               */
//	/* minimizing the total amount of bins in use                           */
//	/************************************************************************/
//
//	IloExpr obj(env);
//	for (j=0;j<m_SizeJ;j++)
//	{
//		obj += VarZ[j];
//	}
//
//	Model.add(IloMinimize(env,obj));
//	obj.end();
//	cout<<"Model Construction is successful"<<endl;
//}

void cCplexRuntime::ProblemSolve(double _arai,vector<cVMRequest>& _vmrequest_vec,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
{

	cplex_flag = true;
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
	else
	{
		Cplex.clear();
		Model.end();
		Cplex.end();
		env.end();

		return;
	}

	env.out() << "Solution status = " << Cplex.getStatus() << endl;


	if ((Cplex.getCplexStatus() != CPX_STAT_OPTIMAL) && (Cplex.getCplexStatus() != CPX_STAT_FEASIBLE))
	{
		result_output<<" "<<Cplex.getStatus()<<endl;
		Cplex.clear();
		Model.end();
		Cplex.end();
		env.end();

		return;
	}

	 env.out() << "Solution value = " << Cplex.getObjValue() << endl;

	result_output<<_arai<<" "<<arrival_rate_per_100<<" "<<splitable_percentage<<" "<<Cplex.getStatus()<<" "<<Cplex.getObjValue()<<endl;
	result_output.close();

	total_server_used = (uint)Cplex.getObjValue();

	vector<vector<double>> splitting_strategy;
	//get the splitting strategy
	for (i = 0;i<m_SizeI;i++)
	{
		vector<double> app_splitting_strategy;
		for (k=0;k<m_SizeK;k++)
		{
			app_splitting_strategy.push_back(Cplex.getValue(VarY[i][k]));
		}
		splitting_strategy.push_back(app_splitting_strategy);
	}
	
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
					//if (Cplex.getValue(VarX[i][j][k][h])>0.5)
					//	total_server_used += Cplex.getValue(VarX[i][j][k][h]);
					if (Cplex.getValue(VarX[i][j][k][h])>0.5)
					{
						_vmrequest_vec[i].setSVMNumber(k+1);
						_vmrequest_vec[i].setSVMResRequest((resourceRequirement[_vmrequest_vec[i].getOriginalResRequest()])(_vmrequest_vec[i].getOriginalResRequest(),_vmrequest_vec[i].getLambda(),k+1));
						_vmrequest_vec[i].host_server_vec.push_back(&(_server_vec[j]));
						_vmrequest_vec[i].setServed(true);

						//_server_vec[j].allocateResidual(_vmrequest_vec[i].getSVMResRequest(),_vmrequest_vec[i].getArrivalTime(),_vmrequest_vec[i].getDurationTime());

						//total_used += (resourceRequirement[_vmrequest_vec[i].getOriginalResRequest()])(_vmrequest_vec[i].getOriginalResRequest(),_vmrequest_vec[i].getLambda(),k+1);
						//(_server_vec[j]).setServOccupied((_server_vec[j]).getServOccupied()+_resource_vmrequest_vec[i][make_pair(_vmrequest_vec[i].getOriginalResRequest(),(uint)(k+1))]);
						
						//(_server_vec[j]).setTimeResourceUsed(_vmrequest_vec[i],(resourceRequirement[_vmrequest_vec[i].getOriginalResRequest()])(_vmrequest_vec[i].getOriginalResRequest(),_vmrequest_vec[i].getLambda(),k+1));
						//(_server_vec[j]).setTimeWeight(_vmrequest_vec[i],(resourceRequirement[_vmrequest_vec[i].getOriginalResRequest()])(_vmrequest_vec[i].getOriginalResRequest(),_vmrequest_vec[i].getLambda(),k+1));
						//(_server_vec[j]).setServOccupied((_server_vec[j]).getServOccupied()+(resourceRequirement[_vmrequest_vec[i].getOriginalResRequest()])(_vmrequest_vec[i].getOriginalResRequest(),_vmrequest_vec[i].getLambda(),k+1));			
						//(_server_vec[j]).setServWeight(1/(total_server_capacity*exp(((_server_vec[j]).getServOccupied()/total_server_capacity))));
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

//void cCplexRuntime::ProblemSolveGlobal(vector<cVMRequest>& _vmrequest,vector<cServer>& _server_vec,map<pair<double,uint>,double>& _resource_request)
//{
//
//	IloInt i,j,k,h;
//
//	ofstream result_output;
//	result_output.open("cplex_output.txt",ios_base::app);
//	if (!result_output)
//	{
//		cerr<<"unable to open output file: optimization.txt";
//		exit(-1);
//	} 
//
//	cout<<"Problem solving is beginning......"<<endl;
//
//	Cplex.extract(Model);
//	//Cplex.exportModel("NR-MF.lp");
//	IloBool  flag = Cplex.solve();
//	if (flag == IloTrue)
//	{
//		cout<<"The End....."<<endl;
//	}
//	env.out() << "Solution status = " << Cplex.getStatus() << endl;
//	env.out() << "Solution value = " << Cplex.getObjValue() << endl;
//
//	result_output<<" "<<Cplex.getStatus()<<" "<<Cplex.getObjValue()<<endl;
//	result_output.close();
//
//	Cplex.clear();
//	Model.end();
//	Cplex.end();
//	env.end();
//
//	return;
//
//}
