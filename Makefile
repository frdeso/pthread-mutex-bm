all: main.cpp
	g++ main.cpp -g -o bm -std=c++11 -static -lpthread
