#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "joinwidget.h"
#include "messagewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , join_widget(0)
    , main_game_widget(0)
    , message_widget(new MessageWidget(this))
{
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);
    QHBoxLayout* layout = findChild<QHBoxLayout*>("main_horizontal_layout");
    join_widget = new JoinWidget(this, this);
    layout->addWidget(join_widget);
    join_widget->show();
    message_widget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addToLayout(QWidget *el)
{
    QHBoxLayout* layout = findChild<QHBoxLayout*>("main_horizontal_layout");
    layout->addWidget(el);
}

void MainWindow::showJoinWidget()
{
    if (join_widget)
        join_widget->show();
}

void MainWindow::showMessage(const char* label, const char* button)
{
    message_widget->set(label, button, width(), height());
    message_widget->show();
}
