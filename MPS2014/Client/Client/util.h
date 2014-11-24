#ifndef UTILH
#define UTILH

#include<iostream>

void mySleep(int sleepMs);

void assert(int err, char* msg=NULL);

void assert(void* err, char* msg=NULL);

#endif