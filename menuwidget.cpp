#include <QKeyEvent>

#include "menuwidget.h"
#include "ui_menuwidget.h"

MenuWidget::MenuWidget(QWidget *parent, int _width, int _height)
    : QWidget(parent)
    , ui(new Ui::MenuWidget)
    , mw(0)
{
    ui->setupUi(this);
    ui->frame->focusWidget();
    resize(_width, _height);
    raise();
}

MenuWidget::~MenuWidget()
{
    delete ui;
}

void MenuWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        hide();
        m->setFocus();
        return;
    }
    // qDebug() << "EVENT RECEIVED: " << event->key();
    QWidget::keyPressEvent(event);
}

void MenuWidget::on_pushButton_clicked()
{
    hide();
    m->setFocus();
    return;
}


void MenuWidget::on_pushButton_2_clicked()
{
    m->hide();
    m->deleteLater();
    hide();
    mw->showJoinWidget();
    deleteLater();
}


void MenuWidget::on_pushButton_4_clicked()
{
    QCoreApplication::quit();
}

