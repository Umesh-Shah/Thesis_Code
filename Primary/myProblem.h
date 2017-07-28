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

#define N 7 	 // 6 nodes + 1 virtual node
#define E 22 	//16 unidirectional edges + 6 virtual edges	
#define OR 1000 // Optical reach
#define K 4 	// No of Channels
#define R 5 	// No of request 