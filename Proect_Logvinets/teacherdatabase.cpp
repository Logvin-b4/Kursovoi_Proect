#include "teacherdatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSet>
TeacherDataBase::TeacherDataBase() {}
bool TeacherDataBase::openDatabase(const QString &path) {
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

bool TeacherDataBase::getStudentData(int userIdPr, QString &name, QString &family, QString &patronymic, QString &address, QString &phone, QString &group) {
    QSqlQuery query;
    query.prepare("SELECT a.name, a.family, a.patronymic, d.address, d.phone, d.group_name "
                  "FROM Autorizactia a "
                  "JOIN DopInfo d ON a.id = d.id_Autorizactia "
                  "WHERE a.id = :userIdPr");
    query.bindValue(":userIdPr", userIdPr);

    if (query.exec() && query.next()) {
        name = query.value(0).toString();
        family = query.value(1).toString();
        patronymic = query.value(2).toString();
        address = query.value(3).toString();
        phone = query.value(4).toString();
        group = query.value(5).toString();
        return true;
    } else {
        qDebug() << "Ошибка при получении данных преподавателя:" << query.lastError().text();
        return false;
    }
}
bool TeacherDataBase::getFacultatives(int userIdPr, QVector<QVector<QString>> &facultativesData) {
    QSqlQuery query;
    query.prepare("SELECT name, conduct_type, time, groupss "
                  "FROM Facultatives "
                  "WHERE id_Autorizactia = :userIdPr");
    query.bindValue(":userIdPr", userIdPr);

    if (query.exec()) {
        while (query.next()) {
            QVector<QString> rowData;
            rowData.append(query.value(0).toString()); // name
            rowData.append(query.value(1).toString()); // conduct_type
            rowData.append(query.value(2).toString()); // time
            rowData.append(query.value(3).toString()); // groupss
            facultativesData.append(rowData);
        }
        return true;
    } else {
        qDebug() << "Ошибка при получении данных факультативов:" << query.lastError().text();
        return false;
    }
}
bool TeacherDataBase::getUniqueFacultativeNames(int userIdPr, QStringList &facultativeNames) {
    QSqlQuery query;
    query.prepare("SELECT DISTINCT name "
                  "FROM Facultatives "
                  "WHERE id_Autorizactia = :userIdPr");
    query.bindValue(":userIdPr", userIdPr);

    if (query.exec()) {
        while (query.next()) {
            facultativeNames.append(query.value(0).toString());
        }
        return true;
    } else {
        qDebug() << "Ошибка при получении уникальных названий факультативов:" << query.lastError().text();
        return false;
    }
}
bool TeacherDataBase::isStudentEnrolledInFacultative(const QString &name, const QString &family, const QString &patronymic, const QString &facultative) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) "
                  "FROM Grade g "
                  "JOIN Autorizactia a ON g.id_Autorizactia = a.id "
                  "JOIN Facultatives f ON g.id_Facultative_name = f.id "
                  "WHERE a.name = :name AND a.family = :family AND a.patronymic = :patronymic AND f.name = :facultative");
    query.bindValue(":name", name);
    query.bindValue(":family", family);
    query.bindValue(":patronymic", patronymic);
    query.bindValue(":facultative", facultative);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    } else {
        qDebug() << "Ошибка при проверке записи студента на факультатив:" << query.lastError().text();
        return false;
    }
}

bool TeacherDataBase::addGrade(const QString &name, const QString &family, const QString &patronymic, const QString &facultative, const QString &grade) {
    QSqlQuery query;
    query.prepare("UPDATE Grade "
                  "SET grade = :grade "
                  "WHERE id_Autorizactia = (SELECT a.id FROM Autorizactia a WHERE a.name = :name AND a.family = :family AND a.patronymic = :patronymic) "
                  "AND id_Facultative_name IN (SELECT f.id FROM Facultatives f WHERE f.name = :facultative)");
    query.bindValue(":name", name);
    query.bindValue(":family", family);
    query.bindValue(":patronymic", patronymic);
    query.bindValue(":facultative", facultative);
    query.bindValue(":grade", grade);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            return true;
        } else {
            qDebug() << "Ошибка: запись не найдена или оценка не была изменена.";
            return false;
        }
    } else {
        qDebug() << "Ошибка при обновлении оценки:" << query.lastError().text();
            return false;
    }
}
bool TeacherDataBase::getStudentSchedule(const QString &name, const QString &family, const QString &patronymic, QVector<QVector<QString>> &schedule) {
    QSqlQuery query;
    query.prepare("SELECT f.name, f.conduct_type, f.time "
                  "FROM Facultatives f "
                  "JOIN Grade g ON f.id = g.id_Facultative_name "
                  "JOIN Autorizactia a ON g.id_Autorizactia = a.id "
                  "WHERE a.name = :name AND a.family = :family AND a.patronymic = :patronymic");
    query.bindValue(":name", name);
    query.bindValue(":family", family);
    query.bindValue(":patronymic", patronymic);

    if (query.exec()) {
            while (query.next()) {
            QVector<QString> row;
            row.append(query.value(0).toString()); // name
            row.append(query.value(1).toString()); // conduct_type
            row.append(query.value(2).toString()); // time
            schedule.append(row);
            }
            return true;
    } else {
            qDebug() << "Ошибка при получении расписания студента:" << query.lastError().text();
            return false;
    }
}

bool TeacherDataBase::getStudentGrades(const QString &name, const QString &family, const QString &patronymic, QVector<QVector<QString>> &grades) {
    QSqlQuery query;
    query.prepare("SELECT f.name, g.grade "
                  "FROM Grade g "
                  "JOIN Facultatives f ON g.id_Facultative_name = f.id "
                  "JOIN Autorizactia a ON g.id_Autorizactia = a.id "
                  "WHERE a.name = :name AND a.family = :family AND a.patronymic = :patronymic");
    query.bindValue(":name", name);
    query.bindValue(":family", family);
    query.bindValue(":patronymic", patronymic);

    if (query.exec()) {
            QSet<QString> uniqueFacultatives;
            while (query.next()) {
            QString facultativeName = query.value(0).toString();
            if (!uniqueFacultatives.contains(facultativeName)) {
                uniqueFacultatives.insert(facultativeName);
                QVector<QString> row;
                row.append(facultativeName); // name
                row.append(query.value(1).toString()); // grade
                grades.append(row);
            }
            }
            return true;
    } else {
            qDebug() << "Ошибка при получении оценок студента:" << query.lastError().text();
            return false;
    }
}

bool TeacherDataBase::addStudentToFacultative(const QString &name, const QString &family, const QString &patronymic, const QString &selectedFacultative) {
    QSqlQuery query;

    // Получаем id и группу студента
    query.prepare("SELECT a.id, d.group_name "
                  "FROM Autorizactia a "
                  "JOIN DopInfo d ON a.id = d.id_Autorizactia "
                  "WHERE a.name = :name AND a.family = :family AND a.patronymic = :patronymic");
    query.bindValue(":name", name);
    query.bindValue(":family", family);
    query.bindValue(":patronymic", patronymic);
    if (!query.exec() || !query.next()) {
            qDebug() << "Ошибка при получении id и группы студента:" << query.lastError().text();
            return false;
    }
    int studentId = query.value(0).toInt();
    QString studentGroup = query.value(1).toString();

    // Получаем все id факультативов по его имени и проверяем группу
    query.prepare("SELECT id, groupss FROM Facultatives WHERE name = :selectedFacultative");
    query.bindValue(":selectedFacultative", selectedFacultative);
    if (!query.exec()) {
            qDebug() << "Ошибка при получении id и группы факультативов:" << query.lastError().text();
            return false;
    }

    QList<int> facultativeIds;
    while (query.next()) {
            int facultativeId = query.value(0).toInt();
            QString facultativeGroup = query.value(1).toString();

            if (facultativeGroup == studentGroup) {
            facultativeIds.append(facultativeId);
            }
    }

    if (facultativeIds.isEmpty()) {
            qDebug() << "Факультатив с указанным именем не найден для группы студента";
            return false;
    }

    // Проверяем, записан ли студент уже на факультатив
    query.prepare("SELECT COUNT(*) FROM Grade WHERE id_Autorizactia = :studentId AND id_Facultative_name IN (SELECT id FROM Facultatives WHERE name = :selectedFacultative AND groupss = :studentGroup)");
    query.bindValue(":studentId", studentId);
    query.bindValue(":selectedFacultative", selectedFacultative);
    query.bindValue(":studentGroup", studentGroup);
    if (!query.exec() || !query.next()) {
            qDebug() << "Ошибка при проверке записи студента на факультатив:" << query.lastError().text();
            return false;
    }

    int count = query.value(0).toInt();
    if (count > 0) {
            qDebug() << "Студент уже записан на этот факультатив";
            return false;
    }

    // Выполняем добавление записи в таблицу Grade
    for (int facultativeId : facultativeIds) {
            query.prepare("INSERT INTO Grade (id_Autorizactia, id_Facultative_name) VALUES (:studentId, :facultativeId)");
            query.bindValue(":studentId", studentId);
            query.bindValue(":facultativeId", facultativeId);

            if (!query.exec()) {
            qDebug() << "Ошибка при добавлении студента на факультатив:" << query.lastError().text();
            return false;
            }
    }

    // Уменьшаем количество мест для всех строк с этим факультативом
    for (int facultativeId : facultativeIds) {
            query.prepare("UPDATE Facultatives SET seats = seats - 1 WHERE id = :facultativeId");
            query.bindValue(":facultativeId", facultativeId);
            if (!query.exec()) {
            qDebug() << "Ошибка при обновлении количества мест:" << query.lastError().text();
            db.rollback();
            return false;
            }
    }

    return true;
}
