#ifndef CIRCLE_H
#define CIRCLE_H

#include <QWidget>
#include <QPainter>

class Circle : public QWidget
{
    Q_OBJECT

public:
    Circle(QWidget *parent = nullptr) : QWidget(parent) {}

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setBrush(QBrush(QColor("#FFA500")));
        painter.setPen(Qt::NoPen);
        QRectF rect(0, 0, width(), height());
        painter.drawEllipse(rect);
    }
};

#endif // CIRCLE_H
