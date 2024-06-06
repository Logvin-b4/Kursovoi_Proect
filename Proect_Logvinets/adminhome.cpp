#include "adminhome.h"
#include "mainwindow.h"
#include "ui_adminhome.h"
#include "createdeleteuserdatabase.h"
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
AdminHome::AdminHome(QWidget *parent, int userIdAd) :
    QMainWindow(parent),
    ui(new Ui::AdminHome),
    userIdAd(userIdAd) // Инициализация userId
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0); // Устанавливаем первую вкладку как текущую

    setWindowTitle("Администратор");
    setWindowIcon(QIcon(":/guf/guf.jpg"));
}

AdminHome::~AdminHome()
{
    delete ui;
}

void AdminHome::on_RegisterButton_clicked() {
    // Получение данных из полей формы
    QString name = ui->nameLine->text();
    QString surname = ui->surnameLine->text();
    QString patronymic = ui->patronymicLine->text();
    QString password = ui->passwordline->text();
    QString role = ui->comboBox->currentText();
    QString group = ui->groupLine->text();
    QString address = ui->addressLine->text();
    QString phone = ui->phoneLine->text();
    // Регулярное выражение для проверки русских букв
    // Регулярное выражение для проверки русских букв
    QRegularExpression regExp("^[А-Яа-яЁё]+$");
    QRegularExpressionValidator validator(regExp, this);

    int pos = 0;
    if (validator.validate(name, pos) != QValidator::Acceptable ||
        validator.validate(surname, pos) != QValidator::Acceptable ||
        validator.validate(patronymic, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Ошибка", "Поля ФИО должны содержать только русские буквы!");
        return;
    }


    if (name.isEmpty() || surname.isEmpty() || patronymic.isEmpty() || password.isEmpty() || role.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите обязательные поля(ФИО, пароль, роль)");
        return;
    }
    if (role == "Администратор" || role == "Преподаватель"){
        if (!(group.isEmpty()) || !(address.isEmpty()) || !(phone.isEmpty())) {
            QMessageBox::warning(this, "Ошибка", "Поля группа, адрес и телефон нельзя ввести для Администратора или Преподавателя!");
            return;
        }
    }
    // Проверка заполнения полей в зависимости от роли
    if (!(role == "Администратор" || role == "Преподаватель")) {
        if (group.isEmpty() || address.isEmpty() || phone.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поля группа, адрес и телефон обязательны для студента!");
            return;
        }
        // Проверка адреса
        static QRegularExpression addressRegExp("^[А-Яа-яЁё0-9 ,.]+$");
        if (!addressRegExp.match(address).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Адрес должен содержать только русские буквы, цифры, пробел, запятую и точку!");
            return;
        }

        // Проверка номера телефона (для примера)
        static QRegularExpression phoneRegExp("^89[0-9]{9}$");
        if (!phoneRegExp.match(phone).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Номер телефона должен начинаться с 89 и состоять ровно из 11 цифр!");
            return;
        }

        static QRegularExpression groupRegExp("^[А-Яа-яЁё0-9\\-]+$");
        if (!groupRegExp.match(group).hasMatch()) {
            QMessageBox::warning(this, "Ошибка", "Номер группы должен содержать только цифры и русские буквы!");
            return;
        }
    }


    // Создание объекта для взаимодействия с базой данных
    CreateDeleteUserDataBase db;

    // Вставка данных в таблицу Autorizactia
    int userId = db.insertIntoAutorizactia(name, surname, patronymic, role, password);
    if (userId == -1) {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить пользователя в таблицу Autorizactia!");
        return;
    }
    // Вставка данных в таблицу DopInfo
    if (!(role == "Администратор" || role == "Преподаватель")) {
        if (!db.insertIntoDopInfo(userId, address, phone, group)) {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить дополнительную информацию о студенте в таблицу DopInfo!");
            return;
        }
    }

    QMessageBox::information(this, "Успех", "Пользователь успешно зарегистрирован!");
    ui->nameLine->clear();
    ui->surnameLine->clear();
    ui->patronymicLine->clear();
    ui->passwordline->clear();
    ui->groupLine->clear();
    ui->addressLine->clear();
    ui->phoneLine->clear();
}


void AdminHome::on_deleteButton_clicked()
{
    QString name = ui->deleteNameLine->text();
    QString surname = ui->deleteSurnameLine->text();
    QString patronymic = ui->deletePatronymicLine->text();

    if (name.isEmpty() || surname.isEmpty() || patronymic.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите ФИО пользователя для удаления!");
        return;
    }

    // Создание объекта для взаимодействия с базой данных
    CreateDeleteUserDataBase db;

    // Получение ID пользователя
    int userId = db.getUserIdFromAutorizactia(name, surname, patronymic);
    if (userId == -1) {
        QMessageBox::warning(this, "Ошибка", "Пользователь с указанными ФИО не найден!");
        return;
    }

    // Получение роли пользователя
    QString role = db.getUserRole(userId);
    if (userId == userIdAd){
        QMessageBox::warning(this, "Ошибка", "Вы не можете удалить самого себя!");
        return;
    }
    // Удаление пользователя в зависимости от его роли
    if (role == "Студент") {
        if (!db.deleteStudent(userId)) {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить студента!");
            return;
        }
    } else if (role == "Преподаватель") {
        if (!db.deleteTeacher(userId)) {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить преподавателя!");
            return;
        }
    } else if (role == "Администратор") {
        if (!db.deleteAdmin(userId)) {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить администратора!");
            return;
        }
    }

    QMessageBox::information(this, "Успех", "Пользователь успешно удален!");
    ui->deleteNameLine->clear();
    ui->deleteSurnameLine->clear();
    ui->deletePatronymicLine->clear();
}
void AdminHome::on_addFacultativ_clicked() {
    // Получение данных из полей формы
    QString nameFacultative = ui->NameFacultativeEdit->text();
    QString conductType = ui->comboBox_2->currentText();
    QString timeFacultative = ui->timeFacultativeEdit->text();
    QString seats = ui->SeatsFacultativeEdit->text();
    QString groupFacultative = ui->GroupFacultativeEdit->text();
    QString nameTeacher = ui->NamePrepodavatilEdit->text();
    QString surnameTeacher = ui->SurnamePrepodavatilEdit->text();
    QString patronymicTeacher = ui->PatronumicPrepodavatilEdit->text();

    if (nameFacultative.isEmpty() || conductType.isEmpty() || timeFacultative.isEmpty() || seats.isEmpty() || groupFacultative.isEmpty() ||
        nameTeacher.isEmpty() || surnameTeacher.isEmpty() || patronymicTeacher.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    // Создание объекта для взаимодействия с базой данных
    CreateDeleteUserDataBase db;

    // Проверка уникальности факультатива
    if (db.isFacultativeExists(nameFacultative, timeFacultative, groupFacultative)) {
        QMessageBox::warning(this, "Ошибка", "Факультатив с таким названием, временем проведения и группой уже существует!");
        return;
    }
    static QRegularExpression seatsRegExp("^[0-9]+$");
    if (!seatsRegExp.match(seats).hasMatch()) {
        QMessageBox::warning(this, "Ошибка", "В поле 'количество свободных мест' можно использовать только цифры!");
        return;
    }
    static QRegularExpression faculRegExp("^[А-Яа-яЁё]+$");
    if (!faculRegExp.match(nameFacultative).hasMatch()) {
        QMessageBox::warning(this, "Ошибка", "Название факультатива должно содержать только русские буквы!");
        return;
    }

    if (db.isFacultativeConductType(nameFacultative, conductType, groupFacultative)) {
        QMessageBox::warning(this, "Ошибка", "Факультатив с таким видом проведения уже существует!");
        return;
    }

    // Получение ID преподавателя
    int teacherId = db.getUserIdFromAutorizactia(nameTeacher, surnameTeacher, patronymicTeacher);
    if (teacherId == -1) {
        QMessageBox::warning(this, "Ошибка", "Преподаватель с указанными ФИО не найден!");
        return;
    }

    // Вставка данных в таблицу Facultatives
    if (!db.insertIntoFacultatives(teacherId, nameFacultative, conductType, seats, timeFacultative, groupFacultative)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить факультатив в таблицу Facultatives!");
        return;
    }

    QMessageBox::information(this, "Успех", "Факультатив успешно добавлен!");
    QTime QTi = QTime(0, 0);
    ui->NameFacultativeEdit->clear();
    ui->timeFacultativeEdit->setTime(QTi);
    ui->SeatsFacultativeEdit->clear();
    ui->GroupFacultativeEdit->clear();
    ui->NamePrepodavatilEdit->clear();
    ui->SurnamePrepodavatilEdit->clear();
    ui->PatronumicPrepodavatilEdit->clear();
}

void AdminHome::on_pushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
    this->close();
}
