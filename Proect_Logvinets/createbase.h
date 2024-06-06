#ifndef CREATEBASE_H
#define CREATEBASE_H

#include <QObject>
#include <QtSql>

class CreateBase : public QObject
{
    Q_OBJECT
public:
    explicit CreateBase(QObject *parent = nullptr);
    bool connectToDatabase(const QString &dbPath, const QString &dbName);
    bool createAutorizactiaTable();
    bool createDopInfoTable();
    bool createFacultativesTable();
    bool createGradeTable();
    bool createInitialData();

private:
    QSqlDatabase m_db; // Объект базы данных
};

#endif // CREATEBASE_H
