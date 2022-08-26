#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main(int argc, char **argv)
{
	char **args = parseUserInput(argc, argv);   
    	do_dir(args[0], args);
    	free(args);
    	return 0;
}
