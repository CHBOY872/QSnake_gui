#include "messagewidget.h"
#include "ui_messagewidget.h"

MessageWidget::MessageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MessageWidget)
{
    ui->setupUi(this);
}

MessageWidget::~MessageWidget()
{
    delete ui;
}

void MessageWidget::set(const char* label, const char* button, int _width, int _height)
{
    ui->label->setText(label);
    ui->pushButton->setText(button);
    ui->frame->focusWidget();
    resize(_width, _height);
    raise();
}

void MessageWidget::on_pushButton_clicked()
{
    hide();
}

