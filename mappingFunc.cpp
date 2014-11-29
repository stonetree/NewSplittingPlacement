#include "stdafx.h"
#include "common.h"

double ResourceRequirement55(double _original,double _lambda,uint _num_svm)
{
	double resource_required;

	double lambda_start = 0.7;
	double lambda_end = 1.4;

	double rate_start = -50;
	double rate_end = 50;


	double lambda_rate;

	lambda_rate = (_lambda - lambda_start)*(rate_end - rate_start)/(lambda_end - lambda_start) + rate_start;

	if (1 == _num_svm)
	{
		resource_required = 0;
	}
	else if (4 == _num_svm)
	{
		resource_required = 49.5+32*atan((lambda_rate+2)/8);
	}
	else if (3 == _num_svm)
	{
		resource_required = 45+25.5*atan((lambda_rate+4)/9);
	}
	else
	{
		resource_required = 36+11*atan((lambda_rate+8)/10);
	}

	resource_required = (1 + resource_required * 0.01)*_original/(_num_svm);

	return resource_required;
}

double ResourceRequirement70(double _original,double _lambda,uint _num_svm)
{
	double resource_required;

	double lambda_start = 0.5;
	double lambda_end = 1.2;

	double rate_start = -45;
	double rate_end = 42;


	double lambda_rate;

	lambda_rate = (_lambda - lambda_start)*(rate_end - rate_start)/(lambda_end - lambda_start) + rate_start;

	if (1 == _num_svm)
	{
		resource_required = 0;
	}
	else if (4 == _num_svm)
	{
		resource_required = 58+34*atan((lambda_rate+1)/7);
	}
	else if (3 == _num_svm)
	{
		resource_required = 51.7+26.3*atan((lambda_rate+4.5)/8.6);
	}
	else
	{
		resource_required = 35+11*atan((lambda_rate+8)/16);
	}

	resource_required = (1 + resource_required * 0.01)*_original/(_num_svm);

	return resource_required;
}

double ResourceRequirement80(double _original,double _lambda,uint _num_svm)
{
	double resource_required;

	double lambda_start = 0.4;
	double lambda_end = 1;

	double rate_start = -40;
	double rate_end = 35;


	double lambda_rate;

	lambda_rate = (_lambda - lambda_start)*(rate_end - rate_start)/(lambda_end - lambda_start) + rate_start;

	if (1 == _num_svm)
	{
		resource_required = 0;
	}
	else if (4 == _num_svm)
	{
		resource_required = 67+36*atan((lambda_rate+1)/7);
	}
	else if (3 == _num_svm)
	{
		resource_required = 56.7+28*atan((lambda_rate+5)/8);
	}
	else
	{
		resource_required = 36+11.3*atan((lambda_rate+8)/25);
	}

	resource_required = (1 + resource_required * 0.01)*_original/(_num_svm);

	return resource_required;
}

double ResourceRequirement90(double _original,double _lambda,uint _num_svm)
{
	double resource_required;

	double lambda_start = 0.2;
	double lambda_end = 0.8;

	double rate_start = -35;
	double rate_end = 27;


	double lambda_rate;

	lambda_rate = (_lambda - lambda_start)*(rate_end - rate_start)/(lambda_end - lambda_start) + rate_start;

	if (1 == _num_svm)
	{
		resource_required = 0;
	}
	else if (4 == _num_svm)
	{
		resource_required = 76.5+38.12*atan((lambda_rate+1)/6);
	}
	else if (3 == _num_svm)
	{
		resource_required = 63+29*atan((lambda_rate+4)/7.5);
	}
	else
	{
		resource_required = 35+11.5*atan((lambda_rate+8)/33);
	}

	resource_required = (1 + resource_required * 0.01)*_original/(_num_svm);

	return resource_required;
}

double ResourceRequirement100(double _original,double _lambda,uint _num_svm)
{
	double resource_required;

	double lambda_start = 0.1;
	double lambda_end = 0.6 ;

	double rate_start = -30;
	double rate_end = 20;


	double lambda_rate;

	lambda_rate = (_lambda - lambda_start)*(rate_end - rate_start)/(lambda_end - lambda_start) + rate_start;

	if (1 == _num_svm)
	{
		resource_required = 0;
	}
	else if (4 == _num_svm)
	{
		resource_required = 85.6+41.2*atan((lambda_rate+1)/5);
	}
	else if (3 == _num_svm)
	{
		resource_required = 70+30.8*atan((lambda_rate+4)/7);
	}
	else
	{
		resource_required = 36+11*atan((lambda_rate+8)/50);
	}

	resource_required = (1 + resource_required * 0.01)*_original/(_num_svm);

	return resource_required;
}

void loadResourceRequirementfunc()
{
	//Load the requirement mapping functions
	resourceRequirement.clear();
	resourceRequirement.insert(make_pair(55,ResourceRequirement55));
	resourceRequirement.insert(make_pair(70,ResourceRequirement70));
	resourceRequirement.insert(make_pair(80,ResourceRequirement80));
	resourceRequirement.insert(make_pair(90,ResourceRequirement90));
	resourceRequirement.insert(make_pair(100,ResourceRequirement100));

	return;
}