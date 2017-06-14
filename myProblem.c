/*************************************************
/*************************************************
File:   myProblem.c
Written by: Umesh Shah
Date modified: ??????

(1) To compile this program, please type: make
(2) To run this program, please type: ./myProblem
**************************************************/

#include "myProblem.h"
#ifndef CPX_PROTOTYPE_MIN

static void
   free_and_null (char **ptr),
   usage         (char *progname);
#else
static void
   free_and_null (),
   usage         ();
#endif

#ifndef CPX_PROTOTYPE_MIN
int main (int argc, char *argv[])
#else
int main (argc, argv)
int argc;
char *argv[];

#endif
{	int i,j,src,content,r,counter=0;
	int topologyp[N][N]; // for storing the neighbours.
	
	FILE *inputFile, *outFile, *lpFile, *file, *file1, *file2, *file3;

	time_t startTime,endTime;
	double solution_time;
	char ch,*tokenPtr,s[30],s1[30];
	char lpFileName[50],solutionFileName[50];
	
	int nodes[6]={0,1,2,3,4,5};
	int request[2][2];
	int disaster[2], ds, l, k; 
		
	/* Declare and allocate space for the variables and arrays where we
        will store the optimization results including the status, objective
        value, and variable values. */


	char *colname=NULL;

	value=NULL;
	env=NULL;
	lp=NULL;

	int solstat;
    	double objval;
    	int surplus;
    	char **cur_colname = NULL;
    	char *cur_colnamestore = NULL;
    	int cur_colnamespace;
    	int num;
   	int cur_numcols;

	srand(time(NULL));

	//read in topology
	if((inputFile=fopen("topology_6.txt","r"))==NULL)
	{	printf("Input file can not be opened.\n");
		exit(2);
	}

	for(i=0;i<N;i++)
		for (j = 0; j < N; j++)
			fscanf(inputFile,"%d",&topologyp[i][j]);
	
	fclose (inputFile);

	sprintf(lpFileName, "lpFile.txt");

	if((lpFile=fopen(lpFileName,"w"))==NULL)
	{	printf("LP file can not be opened.\n");
		exit(2);
	}

	sprintf(solutionFileName, "solution.txt");

	if((outFile=fopen(solutionFileName,"w"))==NULL)
	{	printf("Solution file can not be opened.\n");
		exit(2);
	}
		 
		 
	/****************read input disaster set************/	
	// For reading the disaster set 
	 file3 = fopen("disaster.txt","r");
	 if(file3 == NULL)
	 {
		printf("error\n");
		return 1;
	 }
	 for (i=0;i<ds;i++)
	 {
		 disaster[i]= -1;
	 }
	 for (i=0;i<ds;i++)
	 {
		fscanf(file3, "%d", &disaster[i]);
	 }
	 fclose(file3);
	
	// Debugging disaster//
	 for (i=0;i<ds;i++)
	 {
		printf("%d ", disaster[i]);
		printf("\n");
	 }
	 
	 
	 /****************read input request set************/	
	// For reading the request set 
	 file2 = fopen("request.txt","r");
	 if(file2 == NULL)
	 {
		printf("error\n");
		return 1;
	 }
	 for (i=0;i<4;i++)
	 {
		 for (j = 0; j < 2; j++)
			request[i][j]=0;
	 }
	 
	 for (i=0;i<4;i++)
	 {
		for (j = 0; j < 2; j++)
			fscanf(file2,"%d",&request[i][j]);
	 }
	 
	 fclose(file2);
	 
	 

	/******  objective function (1) *******/
	
	fprintf (lpFile, "Minimize\n obj: \n");
	
	for(i =0; i<N; i++)
	{
		fprintf(lpFile, "+ w_%d ", i);
		
	}
	fprintf (lpFile, "\nSubject to \n");
	
	/********* constraint (2) flow constraint for primary path **********/
	
		for(l=0;l<R;l++)
			{	
				for (i=0;i<N;i++)
				{
					fprintf(lpFile, "c%d: ", counter++);
					for (j=0;j<N;j++)
					{	if(topologyp[i][j]==1)
							fprintf(lpFile, "+ X_%d_%d_%d ", i, j, l);
					}	
					for (j=0;j<N;j++)
					{	
						if(topologyp[j][i]==1)
							fprintf(lpFile, "- X_%d_%d_%d ", j, i, l);
					}	
					if (i== 0)
					{
						fprintf(lpFile, " = 1  \n");
					}
					else if (i==request[l][1])
					{
						fprintf(lpFile, " = -1 \n");
					}
					else
					{
						fprintf(lpFile, " = 0 \n");
					}
				}
			}
		
	/********* constraint (3) optical reach **********/
	
		for(l=0;l<R;l++)
			{
				for (i=0;i<N;i++)
				{
					fprintf(lpFile, "c%d: ", counter++);
					for (j=0;j<N;j++)
					{
						fprintf(lpFile, " + L_%d_%d * X_%d_%d_%d ", i, j, i, j, l);
					}
					fprintf(lpFile,"- %d <= 0 \n", D);
				}
			}
			
	/********* constraint (4) & (5) Computing weight of nodes **********/
	
	// constraint (4)
		for(l=0;l<R;l++)
			{	
				for (i=0;i<N;i++)
				{
					for (j=0;j<N;j++)
					{
						fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, " w_%d - X_%d_%d_%d >= 0 \n", i, i, j, l);
					}
				}
			}
	
	// constraint (5)
		for(l=0;l<R;l++)
			{	
				for (i=0;i<N;i++)
				{
					fprintf(lpFile, "c%d: ", counter++);
					fprintf(lpFile, " w_%d ", i);
					for (j=0;j<N;j++)
					{
						fprintf(lpFile, "- X_%d_%d_%d ", i, j, l);
					}
					fprintf(lpFile, " <= 0 \n");
				}
			}
		

	/********* constraint (6) & (7) wavelength continuity constraint **********/
	
	// constraint (6)
		for(l=0;l<R;l++)
		{
			for(k=0;k<K;k++)
			{
				for (i=0;i<N;i++)
				{
					for (j=0;j<N;j++)
					{
						fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, " c_%d_%d_%d * X_%d_%d_%d + U_%d_%d -1 <= 0 \n", i, j, k, i, j, l, k, l);
					}					
				}
			}
		}
	
	
	// constraint (7)
		for(l=0;l<R;l++)
		{
			fprintf(lpFile, "c%d: ", counter++);
			for(k=0;k<K;k++)
			{
				fprintf(lpFile, " + U_%d_%d ", k, l);
			}
			fprintf(lpFile, " -1 <= 0 \n");
		}
		
	/********* constraint (8), (9) & (10) used to determine value of u_i_j_k_r **********/
	
	// constraint (8)
		for(l=0;l<R;l++)
		{
			for(k=0;k<K;k++)
			{
				for (i=0;i<N;i++)
				{
					for (j=0;j<N;j++)
					{
						fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, " U_%d_%d_%d_%d  - U_%d_%d  <= 0 \n", i, j, k, l, k, l);
					}					
				}
			}
		}
		
	// constraint (9)
		for(l=0;l<R;l++)
		{
			for(k=0;k<K;k++)
			{
				for (i=0;i<N;i++)
				{
					for (j=0;j<N;j++)
					{
						fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, " U_%d_%d_%d_%d  - X_%d_%d_%d  <= 0 \n", i, j, k, l, i, j, l);
					}					
				}
			}
		}
		
	// constraint (10)
		for(l=0;l<R;l++)
		{
			for(k=0;k<K;k++)
			{
				for (i=0;i<N;i++)
				{
					for (j=0;j<N;j++)
					{
						fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, " U_%d_%d_%d_%d - U_%d_%d - X_%d_%d_%d +1 >= 0 \n", i, j, k, l, k, l, i, j, l);
					}					
				}
			}
		}
		
			
	
	fprintf(lpFile,"\nEnd\n");

	fclose(lpFile);


		/* Initialize the CPLEX environment */

		startTime = clock();

		env = CPXopenCPLEX(&status);

		if(env == NULL)
		{	refreshCPLEXEnvironment();
			printf("env error.\n");
			exit(3);
		}

		//set time limit
		status=CPXsetdblparam(env,CPX_PARAM_TILIM,7200.0);

		/* Turn on output to the screen */
		status = CPXsetintparam(env,CPX_PARAM_SCRIND,CPX_ON);

		if(status)
		{	refreshCPLEXEnvironment();
			printf("status error.\n");
			exit(3);
		}

		/* Create the problem, using the filename as the problem name */

		lp = CPXcreateprob(env, &status, lpFileName);

		if(lp == NULL)
		{	refreshCPLEXEnvironment();
			printf("\nerror:CPXcreateprob()\n");
			exit(3);
		}

		/* Read LP file into an existing CPLEX problem object */

		status = CPXreadcopyprob(env, lp, lpFileName, NULL);

		if(status)
		{	refreshCPLEXEnvironment();
			printf("\nerror: CPXreadcopyprob()\n");
			exit(3);
		}

		/* optimize the problem and obtain solution */

		status = CPXmipopt(env, lp);

		if(status)
		{	refreshCPLEXEnvironment();
			printf("\nerror:CPXmipopt()\n");
			exit(3);
		}

		/* get solution status */

		solstat = CPXgetstat(env, lp);

		/* get objective value */

		status = CPXgetmipobjval(env, lp, &objval);

		if(status)
		{	refreshCPLEXEnvironment();
			printf("\nerror:CPXgetmipobjval()\n");
			exit(3);
		}

		endTime = clock();

		solution_time = (double)(endTime - startTime)/(double)CLOCKS_PER_SEC;

		fprintf(outFile, "Objective value = %f\nSolution time = %0.3f s\n", objval,solution_time);

		/* The size of the problem should be obtained by asking CPLEX what the
		   actual size is. cur_numcols stores the current number of columns */

		cur_numcols = CPXgetnumcols(env, lp);

		/* Allocate space for solution */

		value = (double *)malloc(cur_numcols * sizeof(double));

		if(value == NULL)
		{	refreshCPLEXEnvironment();
			printf("\nerror:Allocating space for solution\n");
			exit(3);
		}

		status = CPXgetmipx(env, lp, value, 0, cur_numcols - 1 );

		if(status)
		{	refreshCPLEXEnvironment();
			printf("\nerror:CPXgetmipx()\n");
			exit(3);
		}

		/* Use CPXgetcolname() to obtain the actual variables' name */

		status = CPXgetcolname(env, lp, NULL, NULL, 0, &surplus, 0, cur_numcols - 1);

		if((status != CPXERR_NEGATIVE_SURPLUS) && (status != 0))
		{	refreshCPLEXEnvironment();
			printf("\nerror:CPXgetcolname()\n");
			exit(3);
		}

		cur_colnamespace = -surplus;

		if ( cur_colnamespace > 0 )
		{
			cur_colname      = (char **) malloc (sizeof(char *)*cur_numcols);
			cur_colnamestore = (char *)  malloc (cur_colnamespace);

			if( cur_colname == NULL || cur_colnamestore == NULL )
			{	status = -1;
				refreshCPLEXEnvironment();
				printf("\nerror:cur_colname == NULL || cur_colnamestore == NULL.\n");
				exit(3);
			}

          		status = CPXgetcolname (env, lp, cur_colname, cur_colnamestore, cur_colnamespace, &surplus, 0, cur_numcols-1);

			if(status)
			{	refreshCPLEXEnvironment();
				printf("\nerror:CPXgetcolname()\n");
				exit(3);
			}
		}
			
		/* Write out the solution */

	    	// colname = (char *) (malloc (10));

		// for (num = 0; num < cur_numcols; num++)
		// {
			// if ((value[num] != 0)&& (value[num] > SMALL_F))
			// {
				// strcpy(colname,"");
				// colname = cur_colname[num];
				// ch = colname[0];

				// if (ch == 'T' || ch == 'G' || ch == 'R' || ch == 'f')
					// fprintf(outFile, "%-16s= %f\n", cur_colname[num],value[num]);
				// if( ch == 'X')
				// {	fprintf(outFile, "\n%s:", cur_colname[num]);
					// tokenPtr=strtok(colname,"X");
					// strcpy(s,"");
					// strcat(s,tokenPtr);
					// tokenPtr=strtok(s,"_");
					// strcpy(s1,"");
					// strcat(s1,tokenPtr);
					// i=atoi(s1);
					// tokenPtr=strtok(NULL,"_");
					// strcpy(s1,"");
					// strcat(s1,tokenPtr);
					// j=atoi(s1);

					// fprintf(outFile, "Relay node %d uses the link %d-->%d to relay node %d\n", i,i,j,j);
				// }

			// } /*end of value==1*/
		// }  /*end of for*/
	
	fclose(outFile);

	free_and_null((char **) &value);
	free_and_null((char **) &cur_colname);
	free_and_null((char **) &cur_colnamestore);
	
	return 0;
}



int refreshCPLEXEnvironment()
{	/* Free up the basis and solution */
	free_and_null ((char **) &value);

	/* Free up the problem as allocated by CPXcreateprob, if necessary */
	if ( lp != NULL )
	{
		status = CPXfreeprob (env, &lp);
		if ( status )
			fprintf (stderr, "CPXfreeprob failed, error code %d.\n", status);
 	 }

	/* Free up the CPLEX environment, if necessary */
	if ( env != NULL )
	{
		status = CPXcloseCPLEX (&env);
		/* Note that CPXcloseCPLEX produces no output,
		so the only way to see the cause of the error is to use
		CPXgeterrorstring.  For other CPLEX routines, the errors will
		be seen if the CPX_PARAM_SCRIND indicator is set to CPX_ON. */

		if ( status )
		{
			char  errmsg[1024];
			fprintf (stderr, "Could not close CPLEX environment.\n");
			CPXgeterrorstring (env, status, errmsg);
			fprintf (stderr, "%s", errmsg);
		}
	}
	return 0;
}

/* This simple routine frees up the pointer *ptr, and sets *ptr to NULL */

#ifndef CPX_PROTOTYPE_MIN
static void free_and_null (char **ptr)
#else
static void free_and_null (ptr)
char **ptr;
#endif
{
   if ( *ptr != NULL ) {
      free (*ptr);
      *ptr = NULL;
   }
} /* END free_and_null */


#ifndef CPX_PROTOTYPE_MIN
static void usage (char *progname)
#else
static void usage (progname)
char *progname;
#endif
{
   fprintf (stderr,"Usage: %s inputfile\n", progname);
   fprintf (stderr,"This program uses the CPLEX MIP optimizer.\n");
   fprintf (stderr,"Exiting...\n");
} /* END usage */
