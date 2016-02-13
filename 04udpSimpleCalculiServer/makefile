.PHONY: all clean cleanAll

all: 01client 01server

01client: 01udpCalculiClient.c commWrapper.c
	gcc -Wall $^ -o $@

01client: commWrapper.h

01server: 01udpCalculiServer.c commWrapper.c functions01.c
	gcc -Wall $^ -o $@

01server: functions01.h commWrapper.h


clean:
	rm -f *.o

cleanAll: clean
	rm -f 01client 01server
