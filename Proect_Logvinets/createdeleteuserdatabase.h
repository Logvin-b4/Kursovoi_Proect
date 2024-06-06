#ifndef CREATEDELETEUSERDATABASE_H
#define CREATEDELETEUSERDATABASE_H

#include <QSqlDatabase>

class CreateDeleteUserDataBase
{
public:
    CreateDeleteUserDataBase();
    ~CreateDeleteUserDataBase();

    bool openDatabase(const QString &path);
    void closeDatabase();
    QString getUserRole(int userId);

    int insertIntoAutorizactia(const QString &name, const QString &surname, const QString &patronymic, const QString &role, const QString &password);
    int getUserIdFromAutorizactia(const QString &name, const QString &surname, const QString &patronymic);
    bool insertIntoDopInfo(int userId, const QString &address, const QString &phone, const QString &group);
    bool insertIntoFacultatives(int teacherId, const QString &name, const QString &conductType, const QString &seats, const QString &time, const QString &group);
    bool isFacultativeExists(const QString &name, const QString &time, const QString &group);
    bool isFacultativeConductType(const QString &name, const QString &conduct_type, const QString &group);
    bool deleteStudent(int userId);
    bool deleteTeacher(int userId);
    bool deleteAdmin(int userId);


private:
    QSqlDatabase db;
};

#endif // CREATEDELETEUSERDATABASE_H
