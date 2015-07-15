// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <tchar.h>
#include <string>
#include <iostream>
#include <iterator>
#include <cmath>
#include <list>
#include <algorithm>
#include <fstream>



#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#include "ilcplex/ilocplex.h"
#include "ilcplex/ilocplexi.h"

typedef unsigned int TIME_T;
typedef unsigned int ID;
typedef unsigned int uint;
enum event_type{ARRIVAL,DEPARTURE};

using namespace std;



// TODO: 在此处引用程序需要的其他头文件
