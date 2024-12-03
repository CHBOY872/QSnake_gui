#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class JoinWidget;
class MainGameWidget;
class QHBoxLayout;
class MessageWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addToLayout(QWidget *el);
    void showJoinWidget();
    void showMessage(const char* label, const char* button);

private:
    Ui::MainWindow *ui;
    JoinWidget *join_widget;
    MainGameWidget *main_game_widget;
    MessageWidget *message_widget;
};

#endif // MAINWINDOW_H
