all:
	g++ -std=c++2a -Wall -pedantic -Ofast -pthread -fopenmp -march=native main.cpp
