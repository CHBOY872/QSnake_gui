#ifndef MAINGAMEWIDGET_H
#define MAINGAMEWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QByteArray>

#include "gameobjects.h"
#include "objectprocessor.h"

namespace Ui {
class MainGameWidget;
}

class QGridLayout;
class MainWindow;
class MenuWidget;

class MainGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainGameWidget(QWidget *parent, QTcpSocket *_socket, MainWindow *_main_window);
    ~MainGameWidget();

    void configure();
    FieldObject& getFieldObject() { return field; }
    MainSnake* getMainSnake() { return main_snake; }
    void prepareMainSnake();
    void removeMainSnake() { delete main_snake; }
    void drawMainSnake();
    void drawOtherSnake();
    void pushOtherSnake(SnakeObject*);
    void pushFood(FoodObject*);
    void drawFood();
    void generateField();
    void removeAllChildren(QWidget*);
    void resetField();

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onDataReceived();
    void onDisconnect();
    void onSocketError();

private:
    struct snake_list {
        SnakeObject *o;
        snake_list *next;
    };
    struct food_list {
        FoodObject *f;
        food_list *next;
    };
    Ui::MainGameWidget *ui;
    QTcpSocket* socket;
    QByteArray data;
    ObjectProcessor object_processor;
    FieldObject field;
    snake_list* snake_l;
    food_list* food_l;
    MainSnake* main_snake;
    QGridLayout* grid;
    MainWindow* main_window;
    MainSnake* prev_main_snake;
    snake_list* prev_snake_l;
    food_list* prev_food_l;
    MenuWidget* menu_widget;
    Position prev_pos_main;
    bool is_disconnected;
};

#endif // MAINGAMEWIDGET_H
