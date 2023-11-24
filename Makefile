CXX = g++
CXXFLAGS = -Wall -g -std=c++17


all: prims kruskals kruskals_opt

run: prims kruskals kruskals_opt
	./prims
	./kruskals
	./kruskals_opt