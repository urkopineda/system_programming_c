#ifndef THREADR_H
#define THREADR_H

#include "buffer.h"
#include <stdio.h>
#include <unistd.h>

void * consumerFunction(void * arg);
void * producersFunction(void * arg);

#endif
