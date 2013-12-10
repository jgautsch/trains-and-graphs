# Authors: Raul Santelices, Brian DuSell
# Contact: cse30331fa13_tas@listserv.nd.edu
#
# You can add more files, but do not change existing file names
#
# Course: CSE 30331

all: test_prog3

test_prog3: test_prog3.o train_sim.o graph.o
	g++ -Wall -o $@ $^

graph.o: graph.cpp
	g++ -c graph.cpp

train_sim.o: train_sim.cxx train_sim.h
	g++ -Wall -c $<

test_prog3.o: test_prog3.cxx train_sim.h unit_test.h color.h
	g++ -Wall -c $<

clean:
	rm -f test_prog3 *.exe *.o >& /dev/null
