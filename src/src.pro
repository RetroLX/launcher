TEMPLATE = app
TARGET = retrolx-launcher

QT += quick gamepad
CONFIG += c++11

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    QT_DISABLE_DEPRECATED_BEFORE=0x060000 \
    QT_NO_CAST_TO_ASCII \
    QT_NO_PROCESS_COMBINED_ARGUMENT_START \

SOURCES += \
    main.cpp \
    Frontend.cpp \
    Installer.cpp \
    AutorunFile.cpp \
    System.cpp

HEADERS += \
    Frontend.h \
    Installer.h \
    AutorunFile.h \
    System.h

RESOURCES += ui.qrc


unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
