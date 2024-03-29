# Makefile for the CITS3402 project 2
#
# The program can be built using the `make` bash command
# Please view README.md for more info on its usage
#
# Clarifications
# The program will automatically generate an ouput file.
# The ouput file will be in binary.
# The program requires the -f flag to specify the input file.
#
# @author Bruce How (22242664)
# @date 25/10/2019

PROGNAME = spath
GCC = mpicc -std=c99 -pedantic -Wall -Werror -fopenmp
DEPENDENCIES = spath.c process.c memory.c dijkstra.c output.c

all: $(DEPENDENCIES)
	@$(GCC) -o $(PROGNAME) $(DEPENDENCIES)
	@echo "make: '$(PROGNAME)' successfully built."

clean: $(DEPENDENCIES)
	@rm $(PROGNAME)
	@echo "make: '$(PROGNAME)' has been removed."
