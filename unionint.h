#ifndef UNIONINT_H
#define UNIONINT_H

#include <QByteArray>

union UnionInt {
    int num;
    unsigned char b[4];
};

int int_copy(int current_idx, int& bytes_required, const QByteArray& arr, UnionInt *num);
int int_copy(int current_idx, int& bytes_required_1, int& bytes_required_2, const QByteArray& arr, UnionInt *num);

#endif // UNIONINT_H
