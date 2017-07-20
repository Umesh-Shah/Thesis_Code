SYSTEM = x86-64_linux
LIBFORMAT = static_pic

ILOGDIR = /opt/ibm/ILOG/CPLEX_Studio1262
CPLEXDIR = $(ILOGDIR)/cplex
CONCERTDIR = $(ILOGDIR)/concert

CPLEXLIBDIR = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CPLEXINCDIR = $(CPLEXDIR)/include
CONCERTINCDIR = $(CONCERTDIR)/include

#LNFLAGS = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)
#INCFLAGS = -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)

#CXX = g++
#CXXOPT = -O3
#CXXFLAGS = $(INCFLAGS) -DIL_STD $(CXXOPT) -std=c++11 -Wall -Wextra
#CXXLNFLAGS = $(LNFLAGS) -lilocplex -lcplex -lconcert -lm -lpthread -lnsl

CC = g++
COPT = -O0 -g3
CFLAGS = $(COPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)
CLNFLAGS = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) -lilocplex -lcplex -lconcert -lm -lpthread -lnsl




OBJS=\
	myProblem.o \

EXE=\
	myProblem \

$(EXE): $(OBJS)
	 $(CC) $(CFLAGS) $(OBJS) -o $(EXE) $(CLNFLAGS)


ALLHEADER=\
	myProblem.h \

myProblem.o: $(ALLHEADER) myProblem.c
	$(CC) -c $(CFLAGS) myProblem.c -o myProblem.o

clean:
	rm *.o
