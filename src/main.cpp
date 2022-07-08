#include "AutorunFile.h"
#include "Frontend.h"
#include "Installer.h"
#include "System.h"

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QVector>


namespace {
QList<QObject*> create_model()
{
    return {
        new Frontend(
            QStringLiteral("EmulationStation"),
            QStringLiteral("Frontend used by RetroLX for launching emulators (default)."),
            QStringLiteral("es.png"),
            QStringLiteral("emulationstation"),
            QStringLiteral("emulationstation")
        ),
        new Frontend(
            QStringLiteral("Pegasus"),
            QStringLiteral("A graphical frontend for browsing your game library and launching all kinds of emulators from the same place."),
            QStringLiteral("pegasus.png"),
            QStringLiteral("pegasus-fe"),
            QStringLiteral("pegasus-fe")
        ),
        new Frontend(
            QStringLiteral("RetroArch"),
            QStringLiteral("RetroArch is a frontend for emulators, game engines and media players."),
            QStringLiteral("retroarch.png"),
            QStringLiteral("retroarch"),
            QStringLiteral("retroarch")
        ),
        new Frontend(
            QStringLiteral("RetroFE"),
            QStringLiteral("RetroFE is a graphical frontend for command line emulators such as MAME, MESS and Nestopia."),
            QStringLiteral("retrofe.png"),
            QStringLiteral("retrofe"),
            QStringLiteral("retrofe")
        ),
    };
}

} // namespace


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qInfo() << "RetroLX Launcher";
    qInfo() << "Based on RetroPie Frontend Chooser for RetroPie by Mátyás Mustoha";

    const QList<QObject*> frontendModel(create_model());
    Installer installer;
    AutorunFile autorun;
    System system;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("frontendModel"), QVariant::fromValue(frontendModel));
    engine.rootContext()->setContextProperty(QStringLiteral("installer"), &installer);
    engine.rootContext()->setContextProperty(QStringLiteral("autorun"), &autorun);
    engine.rootContext()->setContextProperty(QStringLiteral("system"), &system);
    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
