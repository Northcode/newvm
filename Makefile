#compile newvm source

all: main.cpp
	g++ -g -o main main.cpp --std=c++11
