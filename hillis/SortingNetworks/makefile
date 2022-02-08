CXX = g++
CXXFLAGS  = -g -Wall -std=gnu++17
LIBS = -lpthread -lm
SRC = SortingNetwork.cpp Random.cpp
OBJ = $(SRC:.cpp=.o)
SN = sn.o GeneticAlgorithm.o
CO = co.o Coevolution.o TestCases.o

sn: $(SN) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

co: $(CO) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o sn co

