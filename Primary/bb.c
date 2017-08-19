{
		
			
		
		fprintf (lpFileBack, "Minimize\n obj: \n");
	
		for(i=1; i<N; i++)
			if (bi[i] != 1)
				fprintf(lpFileBack, "+ w_%d ", i);
			
			fprintf (lpFileBack, "\nSubject to \n");
	
	/********* constraint (2) flow constraint for primary path **********/
	
		for(l=0;l<R;l++)
				for (i=0;i<N;i++)
				{
					fprintf(lpFileBack, "c%d: ", counter++);
					for (j=0;j<N;j++)
						if(topologyp[i][j]!=0)
							fprintf(lpFileBack, "+ X_%d_%d_%d ", l, i, j);
				
					for (j=0;j<N;j++)
						if(topologyp[j][i]!=0)
							fprintf(lpFileBack, "- X_%d_%d_%d ", l, j, i);
                                            
					if (i==0)
						fprintf(lpFileBack, " = 1  \n");
					else if (i==request[l][1])
						fprintf(lpFileBack, " = -1 \n");
					else
						fprintf(lpFileBack, " = 0 \n");
				}
	
	/********* constraint (3) optical reach **********/
	
	
		for(l=0;l<R;l++)
		{
			fprintf(lpFileBack, "c%d: ", counter++);
			for (i=0;i<N;i++)
				for (j=0;j<N;j++)
					if(topologyp[i][j]!=0)
					{
						fprintf(lpFileBack, " + %d X_%d_%d_%d ", topologyp[i][j], l, i, j);
					}
			fprintf(lpFileBack," <= %d \n", OR);
		}
			
	/********* constraint (4) & (5) Computing weight of nodes **********/
	
	// constraint (4)
	for(l=0;l<R;l++)
			for (i=1;i<N;i++)
			{
				fprintf(lpFileBack, "c%d: ", counter++);
				fprintf(lpFileBack, " w_%d - X_%d_%d_%d >= 0 \n", i, l, 0, i);
			}
	
	// constraint (5)
			
		for (i=1;i<N;i++)
		{
			fprintf(lpFileBack, "c%d: ", counter++);
			fprintf(lpFileBack, " w_%d ", i);
       for(l=0;l<R;l++)
  			{
						fprintf(lpFileBack, "- X_%d_%d_%d ", l, 0, i);
				}
      fprintf(lpFileBack, " <= 0 \n");
    }

		
	/********* constraint (6) & (7) wavelength continuity constraint **********/
	
	// constraint (6)
	for(k=0;k<K;k++)
    for(l=0;l<R;l++)
				for (i=0;i<N;i++)
					for (j=0;j<N;j++)
						if(topologyp[i][j]!=0 && topologyp[i][j]!=1)
						{
							fprintf(lpFileBack, "c%d: ", counter++);
							fprintf(lpFileBack, " %d X_%d_%d_%d + V_%d_%d <= 1 \n", c[edgesArray[i][j]][k], l, i, j, k, l);
						}					

	// constraint (7)
	for(l=0;l<R;l++)
  {
	  fprintf(lpFileBack, "c%d: ", counter++);
			for(k=0;k<K;k++)
			{
				fprintf(lpFileBack, " + V_%d_%d ", k, l);
			}
			fprintf(lpFileBack, " = 1 \n");
	}
		
	/********* constraint (8), (9) & (10) used to determine value of u_i_j_k_r **********/
	
	// constraint (8)
	for(k=0;k<K;k++)
    for(l=0;l<R;l++)
				for (i=0;i<N;i++)
					for (j=0;j<N;j++)
						if(topologyp[i][j]!=0)
						{
							fprintf(lpFileBack, "c%d: ", counter++);
							fprintf(lpFileBack, " U_%d_%d_%d_%d  - V_%d_%d  <= 0 \n", k, l, i, j, k, l);
						}	

		
	// constraint (9)
	for(k=0;k<K;k++)
    for(l=0;l<R;l++)
			for (i=0;i<N;i++)
				for (j=0;j<N;j++)
					if(topologyp[i][j]!=0)
					{
						fprintf(lpFileBack, "c%d: ", counter++);
						fprintf(lpFileBack, " U_%d_%d_%d_%d  - X_%d_%d_%d  <= 0 \n", k, l, i, j, l, i, j);
          }

		
	// constraint (10)
	for(k=0;k<K;k++)
    for(l=0;l<R;l++)
				for (i=0;i<N;i++)
					for (j=0;j<N;j++)
						if(topologyp[i][j]!=0)
						{
							fprintf(lpFileBack, "c%d: ", counter++);
							fprintf(lpFileBack, " U_%d_%d_%d_%d - V_%d_%d - X_%d_%d_%d >= -1 \n", k, l, i, j, k, l, l, i, j);
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
									fprintf(lpFileBack, "c%d: ", counter++);
									fprintf(lpFileBack, " U_%d_%d_%d_%d + U_%d_%d_%d_%d <= 1\n", k, l, i, j, k, m, i, j);
							}

	
 /*************** BOUNDS *****************************/
	fprintf(lpFileBack,"\nBounds\n");
  for(k=0; k<K; k++)
   for(l=0;l<R;l++)
	  for (i=0;i<N;i++)
	    for (j=0;j<N;j++)
       if(topologyp[i][j]!=0)
          fprintf(lpFileBack, "0 <= U_%d_%d_%d_%d <= 1\n", k, l, i, j);
 
 
  fprintf(lpFileBack,"\nBinaries\n");
  for(l=0;l<R;l++)
		for(i=0;i<N;i++)
  		for(j=0;j<N;j++)
         if(topologyp[i][j]!=0)
            fprintf(lpFileBack, "X_%d_%d_%d \n", l, i, j);
        
  for(l=0;l<R;l++)
		for (k=0; k<K; k++)
        fprintf(lpFileBack, "V_%d_%d \n", k, l);
        
  for (i=1;i<N;i++)
    fprintf(lpFileBack, "w_%d \n", i);
    
  
   
	fprintf(lpFileBack,"\nEnd\n");

	fclose(lpFileBack);


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

		fprintf(outFileBack, "Objective value = %f\nSolution time = %0.3f s\n", objval,solution_time);
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

				if (ch == 'U')
					fprintf(outFileBack, "%-16s= %f\n", cur_colname[num],value[num]);
				
				if (ch == 'w')
				{
					fprintf(outFileBack, "\n%s: ", cur_colname[num]);
					tokenPtr=strtok(colname,"w");
					strcpy(s,"");
					strcat(s,tokenPtr);
					tokenPtr=strtok(s,"_");
					strcpy(s1,"");
					strcat(s1,tokenPtr);
					i=atoi(s1);
					
					bi[i]=1;
				}
				
				if( ch == 'X')
				{	
        
					fprintf(outFileBack, "\n%s: ", cur_colname[num]);
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
					fprintf(outFileBack, "Lightpath %d uses the link %d-->%d\n", l, i, j);
				
					//duplicate =0;
					if(temp!=l)
					{
						counterx = 0;
					}
					if (i!= primaryInfo[l][counterx] && counterx>0)
					{
						for(a=0; a<counterx; a++)
						{
							if(i==primaryInfo[l][a])
							{
								//duplicate = 1;
								goto BEND;
							}
						}
						counterx++;
					}	
										
					primaryInfo[l][counterx] = i;
					primaryInfo[l][counterx+1] = j;
					counterx++;
					
					BEND:
					temp = l;
				
				}
				printf("\n");
				
				if( ch == 'V')
				{
					
					fprintf(outFileBack, "\n%s: ", cur_colname[num]);
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
					fprintf(outFileBack, "Channel %d is used by Lightpath %d \n", i, j);
					
					primaryInfo[j][6]=i;
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
	

	for(i=0;i<R;i++)
	{
		for(j=0;j<N;j++)
		{
			if(primaryInfo[i][j] != -1 && j<6)
			{
				a1 = primaryInfo[i][j];
				b1 = primaryInfo[i][j+1]; 
				c1 = primaryInfo[i][6];
				
				a2 = edgesArray[a1][b1];
				printf("%d ",a2);
				c[a2][c1] = 1;
			}
		}
		
	}
	
	for(i=0;i<E;i++)
	{
		for (j = 0; j < K; j++)
		{
			printf("%d ", c[i][j]);
		}
		printf("\n");
	}
	
	for(i=0;i<D;i++)
	{
		temp = disaster[i];
		
		for(a=0; a<N; a++)
		{
			topologyp[a][temp] = 0;
			for(b=0; b<N; b++)
			{
				topologyp[temp][b] = 0;
			}
		}
	}
	
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			printf("%d ", topologyp[i][j]);
		}
		printf("\n");
	}
	
	for(i=0;i<N;i++)
	{
		printf("%d ", bi[i]);
	}
	printf("\n");
	
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			printf("%d ", edgesArray[i][j]);
		}
		printf("\n");
	}
		
	fclose(info);
	fclose(outFileBack);
			
	}
	