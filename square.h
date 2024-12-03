#ifndef SQUARE_H
#define SQUARE_H

#include <QFrame>

class Square : public QFrame {
    Q_OBJECT

public:
    explicit Square(QWidget *parent, int w) : QFrame(parent)
    {
        setMinimumSize(w, w);
        setStyleSheet("background-color: #F5F5DC;");
    }
};

#endif // SQUARE_H
