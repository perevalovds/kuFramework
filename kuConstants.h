#pragma once

//ќсновные константы, заголовочные файлы и определени¤

//Имя ОС
#ifdef OS_WINDOWS
#define OS_NAME "windows"
#define OS_NAME_SHORT "win"
#define OS_DEFINED
#endif
#ifdef OS_LINUX
#define OS_NAME "linux"
#define OS_NAME_SHORT "linux"
#define OS_DEFINED
#endif


#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>  //ostringsream
#include <iomanip>  //setprecision
#include <fstream>
#include <algorithm>
#include <cfloat>
using namespace std;

//Типы данных
typedef unsigned char u8;
typedef short int s16;
typedef unsigned short int u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef long long int s64;
typedef long long unsigned int u64;

struct float8 {
    float v[8];
};

typedef long double ldouble;


#ifndef PI
	#define PI       3.14159265358979323846
#endif

#ifndef TWO_PI
	#define TWO_PI   6.28318530717958647693
#endif

#ifndef M_TWO_PI
	#define M_TWO_PI   6.28318530717958647693
#endif

#ifndef DEG_TO_RAD
	#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef RAD_TO_DEG
	#define RAD_TO_DEG (180.0/PI)
#endif

#ifndef MIN
	#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
	#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
	#define CLAMP(val,min,max) (MAX(MIN(val,max),min))
#endif

#ifndef ABS
	#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#define BIT_TO_INT( x, bitmask ) ( ((x) & (bitmask)) ? 1 : 0 )


