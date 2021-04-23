all:
	g++ main.cpp -s -O2 -lboost_program_options -pthread -o fprime

install:
	cp fprime /usr/local/bin
