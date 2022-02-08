CXX = g++
CXXFLAGS  = -g -Wall -std=gnu++17
LIBS = -lpthread -lm
SOURCES = main.cpp GeneticAlgorithm.cpp SortingNetwork.cpp Random.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = a

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm *.o a

