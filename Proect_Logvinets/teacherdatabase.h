#ifndef TEACHERDATABASE_H
#define TEACHERDATABASE_H
#include <QSqlDatabase>
#include <QString>

class TeacherDataBase
{
public:
    TeacherDataBase();
    bool openDatabase(const QString &path);
    bool getStudentData(int userIdPr, QString &name, QString &family, QString &patronymic, QString &address, QString &phone, QString &group);
    bool getFacultatives(int userIdPr, QVector<QVector<QString>> &facultativesData);
    bool getUniqueFacultativeNames(int userIdPr, QStringList &facultativeNames);
    bool isStudentEnrolledInFacultative(const QString &name, const QString &family, const QString &patronymic, const QString &facultative);
    bool addGrade(const QString &name, const QString &family, const QString &patronymic, const QString &facultative, const QString &grade);
    bool getStudentSchedule(const QString &name, const QString &family, const QString &patronymic, QVector<QVector<QString>> &schedule);
    bool getStudentGrades(const QString &name, const QString &family, const QString &patronymic, QVector<QVector<QString>> &grades);
    bool addStudentToFacultative(const QString &name, const QString &family, const QString &patronymic, const QString &selectedFacultative);
private:
    QSqlDatabase db;
};

#endif // TEACHERDATABASE_H
