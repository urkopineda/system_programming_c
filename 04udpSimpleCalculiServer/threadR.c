#include "threadR.h"

void * consumerFunction(void * arg) {
	uint32_t value;
	PMESSAGE pMsg;
	PFIFO pFifo = (PFIFO) arg;
	while(!get(pFifo, &pMsg)) {
		switch(pMsg->type) {
		case GCD: value = gcd(pMsg->param1, pMsg->param2);
			break;
		case LCM: value = lcm(pMsg->param1, pMsg->param2);
			break;
		}
		if (-1 == sendAnswer(sock,to,value)) return -1;
	}
	return NULL;
}

void * producersFunction(void * arg) {
	PMESSAGE pMsg;
	PFIFO pFifo = (PFIFO) arg;
	while(!put(pFifo, pMsg)) {
		
	}
	return NULL;
}
