all:traffic_control_mutiple_AGV.cpp
	g++ traffic_control_mutiple_AGV.cpp  -o  traffic_control_mutiple_AGV.out

.PHONY:clean
clean:
	rm -f traffic_control_mutiple_AGV.out                    
# #declare variables
# CC:=gcc
# exe:=traffic_control_2AGV
# obj:=traffic_control_2AGV.out
