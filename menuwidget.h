#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>

#include "maingamewidget.h"
#include "mainwindow.h"

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent, int _width, int _height);
    ~MenuWidget();

    void keyPressEvent(QKeyEvent *event) override;
    void addMainGameWidget(MainGameWidget *_m) { m = _m; }
    void setMainWindow(MainWindow *_mw) { mw = _mw; };

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MenuWidget *ui;
    MainGameWidget *m;
    MainWindow *mw;
};

#endif // MENUWIDGET_H
