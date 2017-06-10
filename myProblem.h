//include files
#include <ilcplex/cplex.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


//function prototype
int refreshCPLEXEnvironment();

// global variables

double *value;
int status;
CPXENVptr env;
CPXLPptr lp;

// start to define your variables and function propypes here

#define N  14  // 6 nodes	
#define W  8	// Link capacity
#define R 4 // No of request 