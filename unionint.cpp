#include "unionint.h"

int int_copy(int current_idx, int& bytes_required, const QByteArray& arr, UnionInt *num)
{
    int i = 0;
    while (i + current_idx < arr.length() && bytes_required) {
        num->b[i] = arr[current_idx + i];
        i++;
        bytes_required--;
    }
    return i;
}


int int_copy(int current_idx, int& bytes_required_1, int& bytes_required_2, const QByteArray& arr, UnionInt *num)
{
    int i = 0;
    while (i + current_idx < arr.length() && bytes_required_1 && bytes_required_2) {
        num->b[i] = arr[current_idx + i];
        i++;
        bytes_required_1--;
        bytes_required_2--;
    }
    return i;
}
