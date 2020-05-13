CXX     = g++
CXXFLAG = -Wall -Werror -std=c++17

CLI_INCLUDE = CLI/include/


all: myCalculator

myCalculator: cli-utility.o
	$(CXX) $(CXXFLAG) -o $@ $<

cli-utility.o: cli-utility.cpp CLI11.hpp
	$(CXX) -I$(CLI_INCLUDE) $(CXXFLAG) -c $<

.PHONY: clean

clean:
	rm -rf cli-utility.o myCalculator
	