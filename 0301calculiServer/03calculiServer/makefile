.PHONY: all clean cleanAll

all: 01client 03server

01client: 01calculiClient.c commWrapper.c
	gcc -Wall $^ -o $@

01client: commWrapper.h

03server: 03calculiServer.c commWrapper.c functions01.c communicationMonitor.c
	gcc -Wall $^ -lpthread -o $@

03server: functions01.h commWrapper.h

03server communicationMonitor.c: communicationMonitor.h

clean:
	rm -f *.o

cleanAll: clean
	rm -f 01client 03server
