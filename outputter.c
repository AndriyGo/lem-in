#include "lemin.h"

void	die(void)
{
	write(1, "ERROR\n", 6);
	exit(-1);
}