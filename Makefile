# Compiler binary:
CPP= g++

# Recommended compiler flags for speed:
#	OpenMP enabled
#	full binary code optimization
#	full error and warning reports
#	no range checking within BRKGA:
#CFLAGS= -O3 -fopenmp -Wextra -Wall -Weffc++ -ansi -pedantic -Woverloaded-virtual -Wcast-align -Wpointer-arith
CFLAGS= -Wall -O3 -Wextra -Wall -Wc++11-extensions -ansi -pedantic -Woverloaded-virtual -Wcast-align -Wpointer-arith

# Compiler flags for debugging; uncomment if needed:
#	range checking enabled in the BRKGA API
#	OpenMP disabled
#	no binary code optimization
#CFLAGS= -DRANGECHECK -Wextra -Wall -Weffc++ -ansi -pedantic -Woverloaded-virtual -Wcast-align -Wpointer-arith

OBJECTS= uniform_striping_decoder.o topology_solver.o Population.o topology_striping.o mincost.o

# Targets:
all: topol_striping

topol_striping: $(OBJECTS)
	$(CPP) $(CFLAGS) $(OBJECTS) -o topol_striping

topology_striping.o:
	$(CPP) $(CFLAGS) -c topology_striping.cc	

uniform_striping_decoder.o:
	$(CPP) $(CFLAGS) -c uniform_striping_decoder.cc

topology_solver.o:
	$(CPP) $(CFLAGS) -c topology_solver.cc

Population.o:
	$(CPP) $(CFLAGS) -c brkgaAPI/Population.cpp

mincost.o:
	$(CPP) $(CFLAGS) -c mincost.cpp

# Remove:
clean:
	rm -f topol_striping $(OBJECTS)
