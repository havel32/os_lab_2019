#include "swap.h"

void Swap(char *left, char *right)
{
	char buff = *left;
	*left = *right;
	*right = buff;
}
