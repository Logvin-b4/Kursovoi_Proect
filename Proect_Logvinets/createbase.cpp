#include "createbase.h"
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
CreateBase::CreateBase(QObject *parent) : QObject(parent) {}

// Метод для подключения к базе данных
bool CreateBase::connectToDatabase(const QString &dbPath, const QString &dbName)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE"); // Устанавливаем драйвер для работы с базой данных SQLite
    m_db.setDatabaseName(dbPath + dbName); // Соединяем путь и имя файла базы данных

    if (!m_db.open()) { // Пытаемся открыть соединение с базой данных
        qDebug() << "Не удалось подключиться к базе данных";
        return false;
    }

    return true;
}

// Метод для создания таблицы Autorizactia
bool CreateBase::createAutorizactiaTable()
{
    QSqlQuery queryUsers(m_db);
    if (!queryUsers.exec("CREATE TABLE IF NOT EXISTS Autorizactia ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                         "name TEXT NOT NULL,"
                         "family TEXT NOT NULL,"
                         "patronymic TEXT NOT NULL,"
                         "role TEXT NOT NULL,"
                         "password TEXT NOT NULL"
                         ")")) {
        qDebug() << "Не удалось создать таблицу Autorizactia:" << queryUsers.lastError().text();
            return false;
    }

    return true;
}

// Метод для создания таблицы DopInfo
bool CreateBase::createDopInfoTable()
{
    QSqlQuery queryCheckTable(m_db);
    queryCheckTable.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='DopInfo'");
    if (!queryCheckTable.exec() || !queryCheckTable.next()) {
        QSqlQuery queryCreateDopInfo(m_db);
        if (!queryCreateDopInfo.exec("CREATE TABLE DopInfo ("
                                     "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                     "id_Autorizactia INTEGER,"
                                     "address TEXT,"
                                     "phone TEXT,"
                                     "group_name,"
                                     "FOREIGN KEY (id_Autorizactia) REFERENCES Autorizactia(id)"
                                     ")")) {
            qDebug() << "Ошибка при создании таблицы works:" << queryCreateDopInfo.lastError().text();
                return false;
        }
    } else {
        qDebug() << "Таблица 'DopInfo' уже существует";
    }

    return true;
}

// Метод для создания таблицы Facultatives
bool CreateBase::createFacultativesTable()
{
    QSqlQuery queryCheckTable(m_db);
    queryCheckTable.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='Facultatives'");
    if (!queryCheckTable.exec() || !queryCheckTable.next()) {
        QSqlQuery queryCreateFacultatives(m_db);
        if (!queryCreateFacultatives.exec("CREATE TABLE Facultatives ("
                                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                          "id_Autorizactia INTEGER,"
                                          "name TEXT,"
                                          "conduct_type TEXT,"
                                          "seats INTEGER,"
                                          "time TEXT,"
                                          "groupss TEXT,"
                                          "FOREIGN KEY (id_Autorizactia) REFERENCES Autorizactia(id)"
                                          ")")) {
            qDebug() << "Ошибка при создании таблицы Facultatives:" << queryCreateFacultatives.lastError().text();
                return false;
        }
    } else {
        qDebug() << "Таблица 'Facultatives' уже существует";
    }

    return true;
}

// Метод для создания таблицы Grade
bool CreateBase::createGradeTable()
{
    QSqlQuery queryCheckTable(m_db);
    queryCheckTable.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='Grade'");
    if (!queryCheckTable.exec() || !queryCheckTable.next()) {
        QSqlQuery queryCreateGrade(m_db);
        if (!queryCreateGrade.exec("CREATE TABLE Grade ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "id_Autorizactia INTEGER,"
                                   "id_Facultative_name INTEGER,"
                                   "grade TEXT,"
                                   "FOREIGN KEY (id_Autorizactia) REFERENCES Autorizactia(id),"
                                   "FOREIGN KEY (id_Facultative_name) REFERENCES Facultatives(id)"
                                   ")")) {
            qDebug() << "Ошибка при создании таблицы Grade:" << queryCreateGrade.lastError().text();
                return false;
        }
    } else {
        qDebug() << "Таблица 'Grade' уже существует";
    }

    return true;
}

// Метод для создания пользователя
bool CreateBase::createInitialData()
{
    QSqlQuery queryCheckTable(m_db);
    queryCheckTable.prepare("SELECT COUNT(*) FROM Autorizactia");
    if (!queryCheckTable.exec() || !queryCheckTable.next()) {
        qDebug() << "Ошибка при проверке наличия данных в таблице Autorizactia:" << queryCheckTable.lastError().text();
            return false;
    }
    int userCount = queryCheckTable.value(0).toInt();

    if (userCount == 0) {
        QSqlQuery query(m_db);

        if (!query.exec("INSERT INTO Autorizactia (name, family, patronymic, role, password) "
                        "VALUES ('Даниил', 'Логвинец', 'Сергеевич', 'Администратор', '1234')")) {
            qDebug() << "Не удалось вставить пользователя в таблицу Autorizactia:" << query.lastError().text();
            return false;
        }

    }

    return true;
}
