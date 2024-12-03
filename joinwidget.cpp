#include <qDebug>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>

#include "joinwidget.h"
#include "ui_joinwidget.h"

JoinWidget::JoinWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JoinWidget)
    , main_game_widget(0)
    , main_window(0)
{
    ui->setupUi(this);
}

JoinWidget::JoinWidget(QWidget *parent, MainWindow *m)
    : QWidget(parent)
    , ui(new Ui::JoinWidget)
    , main_window(m)
{
    ui->setupUi(this);
}

JoinWidget::~JoinWidget()
{
    delete ui;
}

void JoinWidget::on_pushButton_2_clicked()
{
    QByteArray data;
    QTcpSocket *socket = new QTcpSocket();
    QHostAddress ip(ui->ip_line_edit_2->text());
    QString str_port = ui->port_line_edit_2->text();
    short port = str_port.toInt();
    socket->connectToHost(ip, port);
    if (!socket->waitForConnected(5000)) {
        qDebug() << "Connection failed\n";
        main_window->showMessage("Connection failed. Try again", "Ok");
        delete socket;
        socket = 0;
        return;
    }
    if (socket->waitForReadyRead()) {
        data = socket->readAll();
        qDebug() << "Raw data received: " << data.toStdString() << "\n";
    } else {
        qDebug() << "No data received in time\n";
    }
    hide();
    main_game_widget = new MainGameWidget(main_window, socket, main_window);
    main_window->addToLayout(main_game_widget);
    main_game_widget->configure();
    main_game_widget = 0;
}


void JoinWidget::on_pushButton_clicked()
{
    QCoreApplication::quit();
}

