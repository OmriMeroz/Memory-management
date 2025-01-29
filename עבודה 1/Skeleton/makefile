# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.


all: build

build: clean bin/main.o bin/Auxiliary.o bin/Settlement.o bin/Action.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Simulation.o
	@echo 'Building o files...'
	g++ -o bin/simulation bin/main.o bin/Auxiliary.o bin/Settlement.o bin/Action.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Simulation.o
	@echo 'Finished building o files'

bin/Simulation.o: src/Simulation.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp

bin/SelectionPolicy.o: src/SelectionPolicy.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp

bin/Plan.o: src/Plan.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp

bin/Facility.o: src/Facility.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp

bin/Action.o: src/Action.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/Settlement.o: src/Settlement.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Settlement.o src/Settlement.cpp

bin/Auxiliary.o: src/Auxiliary.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

bin/main.o: src/main.cpp 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

clean: 
	rm -f bin/*

