CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: structs

structs: main.cpp func.cpp structs.h func.h
	$(CXX) $(CXXFLAGS) main.cpp func.cpp -o structs

clean:
	rm -f structs

.PHONY: all clean