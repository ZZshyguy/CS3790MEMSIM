DRIVER : memory.o driver.o
	g++ memory.o driver.o -o DRIVER

memory.o : memory.cpp memory.h
	g++ -c memory.cpp

driver.o : driver.cpp memory.h
	g++ -c driver.cpp
