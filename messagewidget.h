#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(QWidget *parent = nullptr);
    ~MessageWidget();

    void set(const char* label, const char* button, int _width, int _height);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MessageWidget *ui;
};

#endif // MESSAGEWIDGET_H
