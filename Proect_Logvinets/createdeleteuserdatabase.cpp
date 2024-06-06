#include "createdeleteuserdatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>

bool CreateDeleteUserDataBase::openDatabase(const QString &path) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open()) {
        return false;
    } else {
        return true;
    }
}

CreateDeleteUserDataBase::CreateDeleteUserDataBase() {
    // Возможно, здесь вы захотите выполнить какие-то начальные настройки
}

CreateDeleteUserDataBase::~CreateDeleteUserDataBase() {
    // Возможно, здесь вам потребуется выполнить какие-то действия перед удалением объекта
}

QString CreateDeleteUserDataBase::getUserRole(int userId) {
    QSqlQuery query;
    query.prepare("SELECT role FROM Autorizactia WHERE id = :userId");
    query.bindValue(":userId", userId);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    } else {
        return "";
    }
}

int CreateDeleteUserDataBase::insertIntoAutorizactia(const QString &name, const QString &surname, const QString &patronymic, const QString &role, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO Autorizactia (name, family, patronymic, role, password) VALUES (:name, :surname, :patronymic, :role, :password)");
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":patronymic", patronymic);
    query.bindValue(":role", role);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Ошибка при вставке в таблицу Autorizactia:" << query.lastError().text();
            return -1; // возвращаем -1 в случае ошибки
    }

    // Возвращаем последний вставленный id
    return query.lastInsertId().toInt();
}

int CreateDeleteUserDataBase::getUserIdFromAutorizactia(const QString &name, const QString &surname, const QString &patronymic) {
    QSqlQuery query;
    query.prepare("SELECT id FROM Autorizactia WHERE name = :name AND family = :surname AND patronymic = :patronymic");
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":patronymic", patronymic);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        return -1;
    }
}

bool CreateDeleteUserDataBase::insertIntoDopInfo(int userId, const QString &address, const QString &phone, const QString &group) {
    QSqlQuery query;
    query.prepare("INSERT INTO DopInfo (Id_Autorizactia, address, phone, group_name) VALUES (:userId, :address, :phone, :group)");
    query.bindValue(":userId", userId);
    query.bindValue(":address", address);
    query.bindValue(":phone", phone);
    query.bindValue(":group", group);

    return query.exec();
}

bool CreateDeleteUserDataBase::deleteStudent(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM Autorizactia WHERE id = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении студента из таблицы Autorizactia:" << query.lastError().text();
        return false;
    }
    query.prepare("DELETE FROM DopInfo WHERE Id_Autorizactia = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении студента из таблицы DopInfo:" << query.lastError().text();
        return false;
    }
    query.prepare("DELETE FROM Grade WHERE id_Autorizactia = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении студента из таблицы Grade:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CreateDeleteUserDataBase::deleteTeacher(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM Grade WHERE id_Facultative_name IN (SELECT id FROM Facultatives WHERE id_Autorizactia = :userId)");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении оценок преподавателя из таблицы Grade:" << query.lastError().text();
        return false;
    }
    query.prepare("DELETE FROM Autorizactia WHERE id = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении преподавателя из таблицы Autorizactia:" << query.lastError().text();
        return false;
    }
    query.prepare("DELETE FROM DopInfo WHERE Id_Autorizactia = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении преподавателя из таблицы DopInfo:" << query.lastError().text();
        return false;
    }
    query.prepare("DELETE FROM Facultatives WHERE id_Autorizactia = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении преподавателя из таблицы Facultatives:" << query.lastError().text();
        return false;
    }
    return true;
}

bool CreateDeleteUserDataBase::deleteAdmin(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM Autorizactia WHERE id = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении администратора из таблицы Autorizactia:" << query.lastError().text();
        return false;
    }
    query.prepare("DELETE FROM DopInfo WHERE Id_Autorizactia = :userId");
    query.bindValue(":userId", userId);
    if (!query.exec()) {
        qDebug() << "Ошибка при удалении администратора из таблицы DopInfo:" << query.lastError().text();
        return false;
    }
    return true;
}
bool CreateDeleteUserDataBase::insertIntoFacultatives(int teacherId, const QString &name, const QString &conductType, const QString &seats, const QString &time, const QString &group) {
    QSqlQuery query;
    query.prepare("INSERT INTO Facultatives (id_Autorizactia, name, conduct_type, seats, time, groupss) VALUES (:teacherId, :name, :conductType, :seats, :time, :group)");
    query.bindValue(":teacherId", teacherId);
    query.bindValue(":name", name);
    query.bindValue(":conductType", conductType);
    query.bindValue(":seats", seats);
    query.bindValue(":time", time);
    query.bindValue(":group", group);

    if (!query.exec()) {
        qDebug() << "Ошибка при вставке в таблицу Facultatives:" << query.lastError().text();
            return false;
    }
    return true;
}
bool CreateDeleteUserDataBase::isFacultativeExists(const QString &name, const QString &time, const QString &group) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Facultatives WHERE name = :name AND time = :time AND groupss = :group");
    query.bindValue(":name", name);
    query.bindValue(":time", time);
    query.bindValue(":group", group);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return (count > 0);
    } else {
        qDebug() << "Ошибка при проверке уникальности факультатива:" << query.lastError().text();
        return false;
    }
}

bool CreateDeleteUserDataBase::isFacultativeConductType(const QString &name, const QString &conduct_type, const QString &group) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Facultatives WHERE name = :name AND conduct_type = :conduct_type AND groupss = :group");
    query.bindValue(":name", name);
    query.bindValue(":conduct_type", conduct_type);
    query.bindValue(":group", group);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return (count > 0);
    } else {
        qDebug() << "Ошибка при проверке уникальности факультатива:" << query.lastError().text();
        return false;
    }
}

