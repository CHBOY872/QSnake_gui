#ifndef UNIONCHAR_H
#define UNIONCHAR_H

#include <QByteArray>

union UnionChar {
    char c;
    unsigned char b;
};

int char_copy(int current_idx, int& bytes_required, const QByteArray& arr, UnionChar *c);
int char_copy(int current_idx, int& bytes_required_1, int& bytes_required_2, const QByteArray& arr, UnionChar *c);

#endif // UNIONCHAR_H
