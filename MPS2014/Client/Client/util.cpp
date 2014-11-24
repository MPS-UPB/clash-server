#include "util.h"

#ifdef __unix__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

void mySleep(int sleepMs)
{
#ifdef __unix__
	usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef _WIN32
	Sleep(sleepMs);
#endif
}

void assert(int err, char* msg)
{
	if(err==0)
		std::cout<<"Eroare!"<<msg<<errno;
}

void assert(void* err, char* msg)
{
	if(err==NULL)
		std::cout<<"Eroare!"<<msg;
}