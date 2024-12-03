#ifndef IMAGE_H
#define IMAGE_H

#include <QLabel>
#include <QPixmap>
#include <QLayout>

#include "gameobjects.h"

class Image : public QWidget
{
    Q_OBJECT

public:
    Image(QWidget *parent, Position &rotation, QLayout* l) : QWidget(parent)
    {
        label = new QLabel(parent);
        pixmap = QPixmap(":/src/images/Head.png");
        rotateImage(rotation);
        addToLayout(l);
    }

private:
    void addToLayout(QLayout* l)
    {
        l->addWidget(label);
    }
    void rotateImage(Position& rotation)
    {
        QTransform transform;
        if (rotation == Position(1, 0)) { /* right */
            transform.rotate(270);
            pixmap = pixmap.transformed(transform);
        } else if (rotation == Position(0, -1)) { /* up */
            transform.rotate(180);
            pixmap = pixmap.transformed(transform);
        } else if (rotation == Position(-1, 0)) { /* left */
            transform.rotate(90);
            pixmap = pixmap.transformed(transform);
        } else if (rotation == Position(0, 1)) { /* down */
            // transform.rotate(90);
            // pixmap = pixmap.transformed(transform);
        }
        label->setPixmap(pixmap);
        label->setFixedSize(parentWidget()->width(), parentWidget()->height());
        label->setScaledContents(true);
    }

private:
    QLabel *label;
    QPixmap pixmap;
};

#endif // IMAGE_H
