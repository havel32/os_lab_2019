#include "sum.h"

int SumArrayPart(int* array, size_t start, size_t end) {
    int sum = 0;
    for (size_t i = start; i < end; ++i) {
        sum += array[i];
    }
    return sum;
}
