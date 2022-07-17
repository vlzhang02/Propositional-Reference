###
### Makefile for Propositional Inference Program
###
### Author: Valerie Zhang
###

CXX      = g++
CXXFLAGS = -g3 -Ofast -Wall -Wextra -std=c++11 

forward_chain: main.o SymTable.o KB.o FC.o
	$(CXX) $(CXXFLAGS) -o $@ $^

FC.o: FC.cpp
	$(CXX) $(CXXFLAGS) -c $^

KB.o: KB.cpp 
	$(CXX) $(CXXFLAGS) -c $^

SymTable.o: SymTable.cpp
	$(CXX) $(CXXFLAGS) -c $^

unit_test: unit_test_driver.o SymTable.o KB.o FC.o
	$(CXX) $(CXXFLAGS) $^
	
clean: 
	rm *.o a.out *~ *#