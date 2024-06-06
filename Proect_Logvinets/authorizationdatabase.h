#ifndef AUTHORIZATIONDATABASE_H
#define AUTHORIZATIONDATABASE_H

#include <QSqlDatabase>

class AuthorizationDatabase {
public:
    AuthorizationDatabase();
    bool openDatabase(const QString &path);
    QString getUserRole(const QString &name, const QString &family, const QString &patronymic, const QString &password);
    int getUserId(const QString &name, const QString &family, const QString &patronymic, const QString &password);

private:
    QSqlDatabase db;
};

#endif // AUTHORIZATIONDATABASE_H
