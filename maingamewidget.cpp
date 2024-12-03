#include <QGridLayout>
#include <QKeyEvent>
#include <qlabel.h>

#include "image.h"
#include "menuwidget.h"
#include "maingamewidget.h"
#include "ui_maingamewidget.h"
#include "square.h"
#include "mainwindow.h"
#include "circle.h"
#include "gameobjects.h"

MainGameWidget::MainGameWidget(QWidget *parent, QTcpSocket *_socket, MainWindow *_main_window)
    : QWidget(parent)
    , ui(new Ui::MainGameWidget)
    , socket(_socket)
    , snake_l(0)
    , food_l(0)
    , main_snake(0)
    , grid(0)
    , main_window(_main_window)
    , prev_main_snake(0)
    , prev_snake_l(0)
    , prev_food_l(0)
    , menu_widget(new MenuWidget(this, parent->width(), parent->height()))
    , prev_pos_main(-1, -1)
    , is_disconnected(false)
{
    ui->setupUi(this);
    menu_widget->hide();
    setFocusPolicy(Qt::StrongFocus);
    menu_widget->addMainGameWidget(this);
}

MainGameWidget::~MainGameWidget()
{
    snake_list* tmp_s;
    food_list* tmp_f;
    if (socket) {
        disconnect(socket, &QTcpSocket::readyRead, this, &MainGameWidget::onDataReceived);
        disconnect(socket, &QTcpSocket::disconnected, this, &MainGameWidget::onDisconnect);
        disconnect(socket, &QTcpSocket::disconnected, this, &MainGameWidget::onSocketError);
        socket->close();
        socket->deleteLater();
        socket = 0;
    }
    delete ui;
    while (snake_l) {
        tmp_s = snake_l;
        snake_l = snake_l->next;
        delete tmp_s->o;
        delete tmp_s;
    }
    while (food_l) {
        tmp_f = food_l;
        food_l = food_l->next;
        delete tmp_f->f;
        delete tmp_f;
    }
    if (main_snake)
        delete main_snake;
    if (prev_main_snake)
        delete prev_main_snake;
    while (prev_food_l) {
        tmp_f = prev_food_l;
        prev_food_l = prev_food_l->next;
        delete tmp_f->f;
        delete tmp_f;
    }
    if (prev_snake_l) {
        tmp_s = prev_snake_l;
        prev_snake_l = prev_snake_l->next;
        delete tmp_s->o;
        delete tmp_s;
    }
}

void MainGameWidget::configure()
{
    menu_widget->setMainWindow(main_window);
    show();
    setFocus();
    /* notify a server about UI protocol usage */
    data = "ui\r\n\0";
    socket->write(data);
    socket->waitForBytesWritten();
    connect(socket, &QTcpSocket::readyRead, this, &MainGameWidget::onDataReceived);
    connect(socket, &QTcpSocket::disconnected, this, &MainGameWidget::onDisconnect);
    connect(socket, &QTcpSocket::disconnected, this, &MainGameWidget::onSocketError);
}


void MainGameWidget::onDataReceived()
{
    if (!socket)
        return;
    while (socket->bytesAvailable()) {
        data = socket->readAll();
        object_processor.Process(data, this);
        data.clear();
    }
}

void MainGameWidget::onDisconnect()
{
    qDebug() << "Socket disconnected";
    if (socket && socket->error() == QAbstractSocket::RemoteHostClosedError)
        is_disconnected = true;
    if (socket) {
        socket->close();
        socket->deleteLater();
        socket = 0;
    }
    if (main_window)
        main_window->showJoinWidget();
    hide();
    deleteLater();
    main_window->showMessage("Game over", "Ok");
}

void MainGameWidget::onSocketError()
{
    if (is_disconnected)
        return;
    qDebug() << "Error while connection";
    if (socket) {
        socket->close();
        socket->deleteLater();
        socket = 0;
    }
    if (main_window)
        main_window->showJoinWidget();
    hide();
    deleteLater();
    main_window->showMessage("You were disconnected", "Ok");
}

void MainGameWidget::prepareMainSnake()
{
    if (main_snake)
        delete main_snake;
    main_snake = new MainSnake;
}

void MainGameWidget::drawMainSnake()
{
    if (!main_snake || !grid)
        return;
    MainSnake::item* tmp = main_snake->first;
    QWidget* w;
    Image* i;
    Position p;
    QVBoxLayout* layout;
    if (tmp) {                  /* Main snakes head */
        w = grid->itemAtPosition(tmp->p.y, tmp->p.x)->widget();
        w->setStyleSheet("background-color: #2E8B57;");
        if (w->layout() == nullptr) {
            layout = new QVBoxLayout(w);
            layout->setContentsMargins(0, 0, 0, 0);
            w->setLayout(layout);
        }
        p = Position(main_snake->move_vector_x, main_snake->move_vector_y);
        i = new Image(w, p, w->layout());
        i->setParent(w);
        tmp = tmp->next;
    }
    while (tmp) {
        w = grid->itemAtPosition(tmp->p.y, tmp->p.x)->widget();
        w->setStyleSheet("background-color: #3CB371;");
        tmp = tmp->next;
    }
    prev_main_snake = main_snake;
    prev_pos_main = main_snake->first ? main_snake->first->p : Position(-1, -1);
    main_snake = 0;
}

void MainGameWidget::pushOtherSnake(SnakeObject *o)
{
    snake_list *tmp = new snake_list;
    tmp->o = o;
    tmp->next = snake_l;
    snake_l = tmp;
}

void MainGameWidget::drawOtherSnake()
{
    if (!snake_l || !grid) {
        return;
    }
    snake_list* tmp = snake_l;
    SnakeObject::item* tmp_s;
    QWidget* w;
    Image* i;
    Position p;
    QVBoxLayout* layout;
    while (tmp) {
        tmp_s = tmp->o->first;
        if (tmp_s) {                /* Other snakes head */
            w = grid->itemAtPosition(tmp_s->p.y, tmp_s->p.x)->widget();
            w->setStyleSheet("background-color: #B22222");
            if (w->layout() == nullptr) {
                layout = new QVBoxLayout(w);
                layout->setContentsMargins(0, 0, 0, 0);
                w->setLayout(layout);
            }
            p = Position(tmp->o->move_vector_x, tmp->o->move_vector_y);
            i = new Image(w, p, w->layout());
            i->setParent(w);
            tmp_s = tmp_s->next;
        }
        while (tmp_s) {
            w = grid->itemAtPosition(tmp_s->p.y, tmp_s->p.x)->widget();
            w->setStyleSheet("background-color: #FF6347;");
            tmp_s = tmp_s->next;
        }
        tmp = tmp->next;
    }
    prev_snake_l = snake_l;
    snake_l = 0;
}

void MainGameWidget::pushFood(FoodObject *food)
{
    food_list *tmp = new food_list;
    tmp->f = food;
    tmp->next = food_l;
    food_l = tmp;
}

void MainGameWidget::drawFood()
{
    if (!food_l || !grid)
        return;
    food_list* tmp = food_l;
    QWidget* w;
    Circle* c;
    while (tmp) {
        w = grid->itemAtPosition(tmp->f->y, tmp->f->x)->widget();
        c = new Circle(w);
        c->setGeometry(w->rect());
        c->show();
        tmp = tmp->next;
    }
    prev_food_l = food_l;
    food_l = 0;
}

void MainGameWidget::generateField()
{
    int i, j;
    int width;
    int height;
    int len;
    Square *s;
    if (ui->frame->layout()) {
        removeAllChildren(ui->frame);
        delete ui->frame->layout();
    }
    // qDebug() << ui->frame->layout();
    grid = new QGridLayout(ui->frame);
    grid->setSpacing(0);
    grid->setContentsMargins(0, 0, 0, 0);
    ui->frame->setLayout(grid);
    width = main_window->width() / field.w;
    height = main_window->height() / field.h;
    qDebug() << width << " " << height;
    len = width > height ? height : width;
    for (i = 0; i < field.h; i++) {
        for (j = 0; j < field.w; j++) {
            s = new Square(ui->frame, len);
            grid->addWidget(s, i, j);
        }
    }
}

void MainGameWidget::removeAllChildren(QWidget *parent)
{
    QLayout* layout = parent->layout();
    QLayoutItem* item;
    while ((item = layout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
}

void MainGameWidget::resetField()
{
    int i;
    QWidget* w;
    MainSnake::item* tmp_m;
    SnakeObject::item* tmp_o;
    snake_list* tmp_l;
    food_list* tmp_food_l;
    QList<QWidget*> children;
    QLayoutItem* item;
    QWidget* widget;
    if (prev_main_snake) {
        tmp_m = prev_main_snake->first;
        while (tmp_m) {
            w = grid->itemAtPosition(tmp_m->p.y, tmp_m->p.x)->widget();
            w->setStyleSheet("background-color: #F5F5DC;");
            if (!w->layout()) {
                tmp_m = tmp_m->next;
                continue;
            }
            while ((item = w->layout()->takeAt(0)))
            {
                if ((widget = item->widget()))
                    widget->deleteLater();
                delete item;
            }
            while ((widget = w->findChild<QWidget*>()))
                delete widget;
            tmp_m = tmp_m->next;
        }
        delete prev_main_snake;
        prev_main_snake = 0;
    }
    while (prev_snake_l) {
        tmp_l = prev_snake_l;
        prev_snake_l = prev_snake_l->next;
        tmp_o = tmp_l->o->first;
        while (tmp_o) {
            w = grid->itemAtPosition(tmp_o->p.y, tmp_o->p.x)->widget();
            w->setStyleSheet("background-color: #F5F5DC;");
            if (!w->layout()) {
                tmp_o = tmp_o->next;
                continue;
            }
            while ((item = w->layout()->takeAt(0)))
            {
                if ((widget = item->widget()))
                    widget->deleteLater();
                delete item;
            }
            while ((widget = w->findChild<QWidget*>()))
                delete widget;
            tmp_o = tmp_o->next;
        }
        delete tmp_l->o;
        delete tmp_l;
    }
    while (prev_food_l) {
        tmp_food_l = prev_food_l;
        prev_food_l = prev_food_l->next;
        w = grid->itemAtPosition(tmp_food_l->f->y, tmp_food_l->f->x)->widget();
        children = w->findChildren<QWidget*>();
        for (QWidget* child : children) {
            if (dynamic_cast<Circle*>(child)) {
                delete child;
            }
        }
        delete tmp_food_l->f;
        delete tmp_food_l;
    }
}

void MainGameWidget::keyPressEvent(QKeyEvent *event) {
    QByteArray msg;
    bool flag = false;
    if (!menu_widget->isHidden()) {
        QWidget::keyPressEvent(event);
        return;
    }
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
        msg = "W\r\n\0";
        flag = true;
    } else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
        msg = "A\r\n\0";
        flag = true;
    } else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
        msg = "S\r\n\0";
        flag = true;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
        msg = "D\r\n\0";
        flag = true;
    } else if (event->key() == Qt::Key_Escape) {
        if (menu_widget->isHidden()) {
            menu_widget->setFocus(Qt::FocusReason::ActiveWindowFocusReason);
            menu_widget->raise();
            menu_widget->show();
        }
    }
    qDebug() << "EVENT RECEIVED: " << event->key();
    if (flag) {
        socket->write(msg);
        socket->waitForBytesWritten();
        return;
    }
    QWidget::keyPressEvent(event);
}
