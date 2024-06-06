#ifndef STUDENTDATABASE_H
#define STUDENTDATABASE_H

#include <QSqlDatabase>
#include <QString>

class StudentDataBase {
public:
    StudentDataBase();
    bool openDatabase(const QString &path);
    bool getStudentData(int userId, QString &name, QString &family, QString &patronymic, QString &address, QString &phone, QString &group);
    QStringList getAvailableFacultativesForGroup(const QString &group, int userId);
    QStringList getFacultativeInfo(const QString &selectedFacultative, const QString &group);
    QStringList getTeacherInfoForFacultative(const QString &selectedFacultative, const QString &group);
    bool addStudentToFacultative(int userId, const QString &selectedFacultative);
    bool deleteStudentToFacultative(int userId, const QString &selectedoldFacultative);
    QStringList getEnrolledFacultatives(int userId);
    QString getFacultativeTime(const QString &selectedFacultative, const QString &conductType, const QString &group);
    QString getGradeForFacultative(int userId, const QString &selectedFacultative);
private:
    QSqlDatabase db;
};

#endif // STUDENTDATABASE_H
