#include "Communication.h"

int main(int argc, char *argv[])
{
	printf("Argumente Client:\n");
	for(int i=0;i<argc;i++)
		printf("\t%s\n", argv[i]);
	Communication com(argv[0], argv[1]);
	return 0;
}