QT += sql
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminhome.cpp \
    authorizationdatabase.cpp \
    createbase.cpp \
    createdeleteuserdatabase.cpp \
    main.cpp \
    mainwindow.cpp \
    studentdatabase.cpp \
    studenthome.cpp \
    teacherdatabase.cpp \
    teacherhome.cpp

HEADERS += \
    adminhome.h \
    authorizationdatabase.h \
    createbase.h \
    createdeleteuserdatabase.h \
    mainwindow.h \
    studentdatabase.h \
    studenthome.h \
    teacherdatabase.h \
    teacherhome.h

FORMS += \
    adminhome.ui \
    mainwindow.ui \
    studenthome.ui \
    teacherhome.ui

TRANSLATIONS += \
    Proect_Logvinets_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    guf.qrc
