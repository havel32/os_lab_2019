#include "revert_string.h"


int len(char *str)
{
    int lengh = 0;
    while (*(str+lengh) != '\0'){
        lengh++;
    }
    return lengh;
}


void RevertString(char *str)
{
	int str_len = len(str);
	int left = 0;
	int right = str_len - 1;
	while (left < right){
		char buff = str[left];
		str[left] = str[right];
		str[right] = buff;
		left++;
		right--;
	}

}

