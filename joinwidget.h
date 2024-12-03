#ifndef JOINWIDGET_H
#define JOINWIDGET_H

#include <QWidget>

#include "maingamewidget.h"
#include "mainwindow.h"

namespace Ui {
class JoinWidget;
}

class JoinWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JoinWidget(QWidget *parent = nullptr);
    JoinWidget(QWidget *parent = nullptr, MainWindow *w = nullptr);
    ~JoinWidget();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::JoinWidget* ui;
    MainGameWidget* main_game_widget;
    MainWindow* main_window;
};

#endif // JOINWIDGET_H
