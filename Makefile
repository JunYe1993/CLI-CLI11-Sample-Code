CXX     = g++
CXXFLAG = -Wall -Werror -std=c++17

CLI_INCLUDE = CLI/include/


all: tool

tool: cli-utility.o
	$(CXX) $(CXXFLAG) -o $@ $<

cli-utility.o: cli-utility.cpp
	$(CXX) -I$(CLI_INCLUDE) $(CXXFLAG) -c $<

.PHONY: clean

clean:
	rm -rf cli-utility.o tool
	