CXX = g++
CXXFLAGS  = -Wall -std=gnu++17
LIBS = -lpthread
OBJS = main.o GeneticAlgorithm.o SortingNetwork.o Random.o
PROGRAM = main.cpp GeneticAlgorithm.cpp SortingNetwork.cpp Random.cpp

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(OBJS) -o a

clean:
	rm *.o a

