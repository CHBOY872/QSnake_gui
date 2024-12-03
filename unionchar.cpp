#include "unionchar.h"

int char_copy(int current_idx, int& bytes_required, const QByteArray& arr, UnionChar *c)
{
    int i = 0;
    while (i + current_idx < arr.length() && bytes_required) {
        c->b = arr[current_idx + i];
        i++;
        bytes_required--;
    }
    return i;
}


int char_copy(int current_idx, int& bytes_required_1, int& bytes_required_2, const QByteArray& arr, UnionChar *c)
{
    int i = 0;
    while (i + current_idx < arr.length() && bytes_required_1 && bytes_required_2) {
        c->b = arr[current_idx + i];
        i++;
        bytes_required_1--;
        bytes_required_2--;
    }
    return i;
}
