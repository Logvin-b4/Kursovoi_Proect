#include "studentdatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

                  StudentDataBase::StudentDataBase() {}

bool StudentDataBase::openDatabase(const QString &path) {
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

bool StudentDataBase::getStudentData(int userId, QString &name, QString &family, QString &patronymic, QString &address, QString &phone, QString &group) {
    QSqlQuery query;
    query.prepare("SELECT a.name, a.family, a.patronymic, d.address, d.phone, d.group_name "
                  "FROM Autorizactia a "
                  "JOIN DopInfo d ON a.id = d.id_Autorizactia "
                  "WHERE a.id = :userId");
    query.bindValue(":userId", userId);

    if (query.exec() && query.next()) {
        name = query.value(0).toString();
        family = query.value(1).toString();
        patronymic = query.value(2).toString();
        address = query.value(3).toString();
        phone = query.value(4).toString();
        group = query.value(5).toString();
        return true;
    } else {
        qDebug() << "Ошибка при получении данных студента:" << query.lastError().text();
        return false;
    }
}
QStringList StudentDataBase::getAvailableFacultativesForGroup(const QString &group, int userId) {
    QStringList facultatives;
    QSqlQuery query;
    query.prepare("SELECT f.name FROM Facultatives f "
                  "LEFT JOIN Grade g ON f.id = g.id_Facultative_name AND g.id_Autorizactia = :userId "
                  "WHERE f.groupss = :group AND g.id_Autorizactia IS NULL AND f.seats > 0");
    query.bindValue(":group", group);
    query.bindValue(":userId", userId);

    if (query.exec()) {
        while (query.next()) {
            facultatives << query.value(0).toString();
        }
    } else {
        qDebug() << "Ошибка при получении доступных факультативов:" << query.lastError().text();
    }

    return facultatives;
}

QStringList StudentDataBase::getFacultativeInfo(const QString &selectedFacultative, const QString &group) {
    QStringList conductTypes;
    QSqlQuery query;
    query.prepare("SELECT conduct_type FROM Facultatives WHERE name = :selectedFacultative AND groupss = :group");
    query.bindValue(":selectedFacultative", selectedFacultative);
    query.bindValue(":group", group);

    if (query.exec()) {
        while (query.next()) {
            conductTypes << query.value(0).toString();
        }
    } else {
        qDebug() << "Ошибка при получении информации о факультативе:" << query.lastError().text();
    }

    return conductTypes;
}
QStringList StudentDataBase::getTeacherInfoForFacultative(const QString &selectedFacultative, const QString &group) {
    QStringList teacherInfo;
    QSqlQuery query;

    // Выполняем запрос для получения ID преподавателя из таблицы Facultatives
    query.prepare("SELECT Id_Autorizactia FROM Facultatives WHERE groupss = :group AND name = :selectedFacultative");
    query.bindValue(":group", group);
    query.bindValue(":selectedFacultative", selectedFacultative);

    if (query.exec() && query.next()) {
        int teacherId = query.value(0).toInt(); // Получаем ID преподавателя

        // Теперь выполняем запрос для получения информации о преподавателе из таблицы Autorizactia по его ID
        query.prepare("SELECT name, family, patronymic FROM Autorizactia WHERE id = :teacherId");
        query.bindValue(":teacherId", teacherId);

        if (query.exec() && query.next()) {
            teacherInfo << query.value(0).toString(); // имя преподавателя
            teacherInfo << query.value(1).toString(); // фамилия преподавателя
            teacherInfo << query.value(2).toString(); // отчество преподавателя
        } else {
            qDebug() << "Ошибка при получении информации о преподавателе:" << query.lastError().text();
        }
    } else {
        qDebug() << "Ошибка при получении ID преподавателя:" << query.lastError().text();
    }

    return teacherInfo;
}
bool StudentDataBase::addStudentToFacultative(int userId, const QString &selectedFacultative) {
    QSqlQuery query;

    // Получаем все id факультативов по его имени
    query.prepare("SELECT id FROM Facultatives WHERE name = :selectedFacultative");
    query.bindValue(":selectedFacultative", selectedFacultative);
    if (!query.exec()) {
        qDebug() << "Ошибка при получении id факультативов:" << query.lastError().text();
        return false;
    }

    QList<int> facultativeIds;
    while (query.next()) {
        facultativeIds.append(query.value(0).toInt());
    }
    qDebug() << "ОААА:" << facultativeIds;
    if (facultativeIds.isEmpty()) {
        qDebug() << "Факультатив с указанным именем не найден";
        return false;
    }

    // Выполняем добавление записи в таблицу Grade
    for (int facultativeId : facultativeIds) {
        query.prepare("INSERT INTO Grade (id_Autorizactia, id_Facultative_name) VALUES (:userId, :facultativeId)");
        query.bindValue(":userId", userId);
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
bool StudentDataBase::deleteStudentToFacultative(int userId, const QString &selectedoldFacultative) {
    QSqlQuery query;

    // Получаем все id факультативов по его имени
    query.prepare("SELECT id FROM Facultatives WHERE name = :selectedFacultative");
    query.bindValue(":selectedFacultative", selectedoldFacultative);
    if (!query.exec()) {
        qDebug() << "Ошибка при получении id факультативов:" << query.lastError().text();
        return false;
    }

    QList<int> facultativeIds;
    while (query.next()) {
        facultativeIds.append(query.value(0).toInt());
    }
    qDebug() << "ОААА:" << facultativeIds;
    if (facultativeIds.isEmpty()) {
        qDebug() << "Факультатив с указанным именем не найден";
        return false;
    }
    // Выполняем добавление записи в таблицу Grade
    for (int facultativeId : facultativeIds) {
        query.prepare("DELETE FROM Grade WHERE id_Autorizactia = :userId AND id_Facultative_name = :facultativeId");
        query.bindValue(":userId", userId);
        query.bindValue(":facultativeId", facultativeId);

        if (!query.exec()) {
            qDebug() << "Ошибка при удалении студента на факультатив1:" << query.lastError().text();
            return false;
        }
    }

    // Уменьшаем количество мест для всех строк с этим факультативом
    for (int facultativeId : facultativeIds) {
        query.prepare("UPDATE Facultatives SET seats = seats + 1 WHERE id = :facultativeId");
        query.bindValue(":facultativeId", facultativeId);
        if (!query.exec()) {
            qDebug() << "Ошибка при обновлении количества мест:" << query.lastError().text();
            db.rollback();
            return false;
        }
    }

    return true;
}
QStringList StudentDataBase::getEnrolledFacultatives(int userId) {
    QStringList facultatives;
    QSqlQuery query;

    // Выполняем запрос для получения id факультативов, на которые записан студент
    query.prepare("SELECT DISTINCT f.name "
                  "FROM Grade g "
                  "JOIN Facultatives f ON g.id_Facultative_name = f.id "
                  "WHERE g.id_Autorizactia = :userId");
    query.bindValue(":userId", userId);

    if (query.exec()) {
        while (query.next()) {
            QString facultativeName = query.value(0).toString();
            if (!facultatives.contains(facultativeName)) {
                facultatives << facultativeName;
            }
        }
    } else {
        qDebug() << "Ошибка при получении факультативов, на которые записан студент:" << query.lastError().text();
    }

    return facultatives;
}
QString StudentDataBase::getFacultativeTime(const QString &selectedFacultative, const QString &conductType, const QString &group) {
    QString time;
    QSqlQuery query;
    query.prepare("SELECT time FROM Facultatives WHERE name = :selectedFacultative AND conduct_type = :conductType AND groupss = :group");
    query.bindValue(":selectedFacultative", selectedFacultative);
    query.bindValue(":conductType", conductType);
    query.bindValue(":group", group);

    if (query.exec() && query.next()) {
        time = query.value(0).toString();
    } else {
        qDebug() << "Ошибка при получении времени проведения факультатива:" << query.lastError().text();
    }

    return time;
}
QString StudentDataBase::getGradeForFacultative(int userId, const QString &selectedFacultative) {
    QString grade;
    QSqlQuery query;
    query.prepare("SELECT g.grade FROM Grade g "
                  "JOIN Facultatives f ON g.id_Facultative_name = f.id "
                  "WHERE g.id_Autorizactia = :userId AND f.name = :selectedFacultative");
    query.bindValue(":userId", userId);
    query.bindValue(":selectedFacultative", selectedFacultative);

    if (query.exec() && query.next()) {
        grade = query.value(0).toString();
    } else {
        qDebug() << "Ошибка при получении оценки для факультатива:" << query.lastError().text();
    }

    return grade;
}

