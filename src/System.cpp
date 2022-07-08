#include "System.h"

#include <QProcess>


System::System(QObject *parent)
    : QObject(parent)
{}

void System::reboot() const
{
    QProcess::startDetached(QString("reboot"), {});
}

void System::shutdown() const
{
    QProcess::startDetached(QString("shutdown"), {"-h", "now"});
}
