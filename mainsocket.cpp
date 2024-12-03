#include "mainsocket.h"

MainSocket::MainSocket(QObject *parent)
    : QObject{parent}
{}

Main* MainSocket::InitScoket(const QString& ip, const int port)
{
    return new
}
