#include "authorizationdatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AuthorizationDatabase::AuthorizationDatabase() {}

bool AuthorizationDatabase::openDatabase(const QString &path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << "Ошибка: соединение с базой данных не удалось";
        return false;
    } else {
        qDebug() << "База данных: соединение успешно";
        return true;
    }
}

QString AuthorizationDatabase::getUserRole(const QString &name, const QString &family, const QString &patronymic, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT role FROM Autorizactia WHERE name = :name AND family = :family AND patronymic = :patronymic AND password = :password");
    query.bindValue(":name", name);
    query.bindValue(":family", family);
    query.bindValue(":patronymic", patronymic);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        qDebug() << "Ошибка при получении роли пользователя:" << query.lastError().text();
        return "";
    }
}

int AuthorizationDatabase::getUserId(const QString &name, const QString &family, const QString &patronymic, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT id FROM Autorizactia WHERE name = :name AND family = :family AND patronymic = :patronymic AND password = :password");
    query.bindValue(":name", name);
    query.bindValue(":family", family);
    query.bindValue(":patronymic", patronymic);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Ошибка при получении ID пользователя:" << query.lastError().text();
        return -1;
    }
}
