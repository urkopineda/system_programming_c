#include <stdio.h>
#include "07monitor.h"

#define N_PRO 2
#define N_CON 1

void* producersFunction(void*);
void* consumersFunction(void*);

int main(void) {
	pthread_t idProductores[N_PRO], idConsumidores[N_CON];
	char str[30];
	int i;
	FIFO fifo;
	initFifo(&fifo);
	for (i = 0; i < N_PRO; i++) pthread_create(&idProductores[i], NULL, producersFunction, (void*) &fifo);
	for (i = 0; i < N_CON; i++) pthread_create(&idConsumidores[i], NULL, consumersFunction, (void*) &fifo);
	for (i = 0; i < N_PRO; i++) pthread_join(idProductores[i], NULL);
	for (i = 0; i < N_CON; i++) pthread_join(idConsumidores[i], NULL);
	destroyFifo(&fifo);
	printf("type \"Return\" to finish\n");
	fgets(str, sizeof(str), stdin);
	interrupt(&fifo);
	return 0;
}

void* producersFunction(void* arg) {
	float f = 15;
	int i = 0;
	PFIFO pFifo = (PFIFO) arg;
	while (i != -1) {
		sleep(1);
		i = put(pFifo, f);
		f += 15;
	}
	return NULL;
}

void* consumersFunction(void* arg) {
	float f = 0;
	int i = 0;
	PFIFO pFifo = (PFIFO) arg;
	while (i != -1) {
		sleep(1);
		i = get(pFifo, &f);
		printf("Consuming %f\n",f);
	}
	return NULL;
}

