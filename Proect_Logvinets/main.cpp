#include "mainwindow.h"
#include "createbase.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CreateBase dbHandler;
    // Подключаемся к базе данных в указанной папке, если её нет, то создаём её (Вместо "database90" можете назвать иначе, как хотите, чтобы называлась база данных)
    if (!dbHandler.connectToDatabase("", "database777.db")) {
        qDebug() << "Не удалось подключиться к базе данных";
        return 1;
    }

    // Создаем таблицу Autorizactia
    if (!dbHandler.createAutorizactiaTable()) {
        qDebug() << "Не удалось создать таблицу Autorizactia";
        return 1;
    }

    // Создаем таблицу DopInfo
    if (!dbHandler.createDopInfoTable()) {
        qDebug() << "Не удалось создать таблицу DopInfo";
        return 1;
    }

    // Создаем таблицу Facultatives
    if (!dbHandler.createFacultativesTable()) {
        qDebug() << "Не удалось создать таблицу Facultatives";
        return 1;
    }

    // Создаем таблицу Grade
    if (!dbHandler.createGradeTable()) {
        qDebug() << "Не удалось создать таблицу Grade";
        return 1;
    }

    // Вносим первого пользователя
    if (!dbHandler.createInitialData()) {
        qDebug() << "Не удалось создать пользователя";
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
