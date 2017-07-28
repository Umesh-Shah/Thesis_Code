/*************************************************
/*************************************************
File:   myProblem.c
Written by: Umesh Shah
Date modified: July 28th 2017

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
	
	FILE *inputFile, *outFile, *lpFile, *file, *file1, *file2, *file3, *info;

	time_t startTime,endTime;
	double solution_time;
	char ch,*tokenPtr,s[30],s1[30];
	char lpFileName[50],solutionFileName[50], infoFileName[50];
	
	int nodes[7]={0,1,2,3,4,5,6};
	int request[2][2];
	int disaster[2], ds, l, m, k, temp = 0, counterx = 0; 
	int edgesArray[N][N]={0};
	int primaryInfo[R][N];
	int c[E][K]={0};
	
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
	
	if((file1=fopen("edgesNumbers.txt","r"))==NULL)
	{	printf("Input file can not be opened.\n");
		exit(2);
	}
	
	for(i=0;i<N;i++)
		for (j = 0; j < N; j++)
			fscanf(inputFile,"%d",&edgesArray[i][j]);
	
	fclose (file1);

	sprintf(lpFileName, "lpFile.txt");
	
	if((file=fopen("PrimaryInfo.txt","r"))==NULL)
	{	printf("PrimaryInfo file can not be opened.\n");
		exit(2);
	}
	
	for(i=0;i<R;i++)
		for(j=0;j<N;j++)
		{
			fscanf(file,"%d",&primaryInfo[i][j]);
		}

	if((lpFile=fopen(lpFileName,"w"))==NULL)
	{	printf("LP file can not be opened.\n");
		exit(2);
	}

	sprintf(solutionFileName, "solution.txt");

	if((outFile=fopen(solutionFileName,"w"))==NULL)
	{	printf("Solution file can not be opened.\n");
		exit(2);
	}
	
	sprintf(infoFileName, "LightpathInfo.txt");
		 
	if((info=fopen(infoFileName,"w"))==NULL)
	{	printf("Lightpath Info file can not be opened.\n");
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
	 
	 for (i=0;i<R;i++)
	 {
		 for (j = 0; j < 2; j++)
			request[i][j]=0;
	 }
	 
	 for (i=0;i<R;i++)
	 {
		for (j = 0; j < 2; j++)
			fscanf(file2,"%d",&request[i][j]);
	 }
	 
	 fclose(file2);
	 
	 

	/******  objective function (1) *******/
	
	fprintf (lpFile, "Minimize\n obj: \n");
	
	for(i=1; i<N; i++)
		fprintf(lpFile, "+ w_%d ", i);
		
	
	fprintf (lpFile, "\nSubject to \n");
	
	/********* constraint (2) flow constraint for primary path **********/
	
		for(l=0;l<R;l++)
				for (i=0;i<N;i++)
				{
					fprintf(lpFile, "c%d: ", counter++);
					for (j=0;j<N;j++)
						if(topologyp[i][j]!=0)
							fprintf(lpFile, "+ X_%d_%d_%d ", l, i, j);
				
					for (j=0;j<N;j++)
						if(topologyp[j][i]!=0)
							fprintf(lpFile, "- X_%d_%d_%d ", l, j, i);
                                            
					if (i==0)
						fprintf(lpFile, " = 1  \n");
					else if (i==request[l][1])
						fprintf(lpFile, " = -1 \n");
					else
						fprintf(lpFile, " = 0 \n");
				}

		
	/********* constraint (3) optical reach **********/
	
		for(l=0;l<R;l++)
		{
			fprintf(lpFile, "c%d: ", counter++);
			for (i=0;i<N;i++)
				for (j=0;j<N;j++)
					if(topologyp[i][j]!=0)
					{
						fprintf(lpFile, " + %d X_%d_%d_%d ", topologyp[i][j], l, i, j);
					}
			fprintf(lpFile," <= %d \n", OR);
		}
			
	/********* constraint (4) & (5) Computing weight of nodes **********/
	
	// constraint (4)
	for(l=0;l<R;l++)
			for (i=1;i<N;i++)
			{
				fprintf(lpFile, "c%d: ", counter++);
				fprintf(lpFile, " w_%d - X_%d_%d_%d >= 0 \n", i, l, 0, i);
			}
	
	// constraint (5)
			
		for (i=1;i<N;i++)
		{
			fprintf(lpFile, "c%d: ", counter++);
			fprintf(lpFile, " w_%d ", i);
       for(l=0;l<R;l++)
  			{
						fprintf(lpFile, "- X_%d_%d_%d ", l, 0, i);
				}
      fprintf(lpFile, " <= 0 \n");
    }

		
	/********* constraint (6) & (7) wavelength continuity constraint **********/
	
	// constraint (6)
	for(k=0;k<K;k++)
    for(l=0;l<R;l++)
				for (i=0;i<N;i++)
					for (j=0;j<N;j++)
						if(topologyp[i][j]!=0)
						{
							fprintf(lpFile, "c%d: ", counter++);
							fprintf(lpFile, " %d X_%d_%d_%d + V_%d_%d <= 1 \n", c[edgesArray[i][j]][k], l, i, j, k, l);
						}					

	// constraint (7)
	for(l=0;l<R;l++)
  {
	  fprintf(lpFile, "c%d: ", counter++);
			for(k=0;k<K;k++)
			{
				fprintf(lpFile, " + V_%d_%d ", k, l);
			}
			fprintf(lpFile, " = 1 \n");
	}
		
	/********* constraint (8), (9) & (10) used to determine value of u_i_j_k_r **********/
	
	// constraint (8)
	for(k=0;k<K;k++)
    for(l=0;l<R;l++)
				for (i=0;i<N;i++)
					for (j=0;j<N;j++)
						if(topologyp[i][j]!=0)
						{
							fprintf(lpFile, "c%d: ", counter++);
							fprintf(lpFile, " U_%d_%d_%d_%d  - V_%d_%d  <= 0 \n", k, l, i, j, k, l);
						}	

		
	// constraint (9)
	for(k=0;k<K;k++)
    for(l=0;l<R;l++)
			for (i=0;i<N;i++)
				for (j=0;j<N;j++)
					if(topologyp[i][j]!=0)
					{
						fprintf(lpFile, "c%d: ", counter++);
						fprintf(lpFile, " U_%d_%d_%d_%d  - X_%d_%d_%d  <= 0 \n", k, l, i, j, l, i, j);
          }

		
	// constraint (10)
	for(k=0;k<K;k++)
    for(l=0;l<R;l++)
				for (i=0;i<N;i++)
					for (j=0;j<N;j++)
						if(topologyp[i][j]!=0)
						{
							fprintf(lpFile, "c%d: ", counter++);
							fprintf(lpFile, " U_%d_%d_%d_%d - V_%d_%d - X_%d_%d_%d >= -1 \n", k, l, i, j, k, l, l, i, j);
						}

	/********* constraint (11) wavelength continuity clash constraint **********/
	  for(k=0;k<K;k++)
      for(l=0;l<R;l++)
  			for(m=0;m<R;m++)
          if(l != m)
  					for (i=0;i<N;i++)
  						for (j=0;j<N;j++)
  							if(topologyp[i][j]!=0)
  							{
									fprintf(lpFile, "c%d: ", counter++);
									fprintf(lpFile, " U_%d_%d_%d_%d + U_%d_%d_%d_%d <= 1\n", k, l, i, j, k, m, i, j);
							}

	
 /*************** BOUNDS *****************************/
	fprintf(lpFile,"\nBounds\n");
  for(k=0; k<K; k++)
   for(l=0;l<R;l++)
	  for (i=0;i<N;i++)
	    for (j=0;j<N;j++)
       if(topologyp[i][j]!=0)
          fprintf(lpFile, "0 <= U_%d_%d_%d_%d <= 1\n", k, l, i, j);
 
 
  fprintf(lpFile,"\nBinaries\n");
  for(l=0;l<R;l++)
		for(i=0;i<N;i++)
  		for(j=0;j<N;j++)
         if(topologyp[i][j]!=0)
            fprintf(lpFile, "X_%d_%d_%d \n", l, i, j);
        
  for(l=0;l<R;l++)
		for (k=0; k<K; k++)
        fprintf(lpFile, "V_%d_%d \n", k, l);
        
  for (i=1;i<N;i++)
    fprintf(lpFile, "w_%d \n", i);
    
  
   
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
		printf("Objective value = %f\nSolution time = %0.3f s\n", objval,solution_time);

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

	    	colname = (char *) (malloc (10));

		for (num = 0; num < cur_numcols; num++)
		{
			if ((value[num] != 0))
			{
				strcpy(colname,"");
				colname = cur_colname[num];
				ch = colname[0];
                
				//if(flag ==1)  //primary work
				if (ch == 'w' || ch == 'U')
					fprintf(outFile, "%-16s= %f\n", cur_colname[num],value[num]);
				
				if( ch == 'X')
				{	
        
					fprintf(outFile, "\n%s: ", cur_colname[num]);
					tokenPtr=strtok(colname,"X");
					strcpy(s,"");
					strcat(s,tokenPtr);
					tokenPtr=strtok(s,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					l=atoi(s1);
					tokenPtr=strtok(NULL,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					i=atoi(s1);
					tokenPtr=strtok(NULL,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					j=atoi(s1);
					fprintf(outFile, "Lightpath %d uses the link %d-->%d\n", l, i, j);
				
					if(temp!=l)
						{
							counterx = 0;
						}
					primaryInfo[l][counterx] = i;
					primaryInfo[l][counterx+1] = j;
					fprintf(info,"%d ",primaryInfo[l][k]);
					//printf("%d ",primaryInfo[l][k]);
					
					counterx++;
					temp = l;
				
				}
				
				if( ch == 'V')
				{
					
					fprintf(outFile, "\n%s: ", cur_colname[num]);
					tokenPtr=strtok(colname,"V");
					strcpy(s,"");
					strcat(s,tokenPtr);
					tokenPtr=strtok(s,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					i=atoi(s1);
					tokenPtr=strtok(NULL,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					j=atoi(s1);
					fprintf(outFile, "Lightpath %d uses channel %d\n", i, j);
					
					primaryInfo[i][6]=j;
				}
			
			}/*end of value==1*/
		}  /*end of for*/
		

	for(i=0;i<R;i++)
	{
		for(j=0;j<N;j++)
		{
			printf("%d ",primaryInfo[i][j]);
		}
		printf("\n");
	}		
	
	fclose(info);
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
