#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "authorizationdatabase.h"
#include "AdminHome.h"
#include "StudentHome.h"
#include "TeacherHome.h"
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Окно авторизации");
    setWindowIcon(QIcon(":/guf/guf.jpg"));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_loginButton_clicked() {
    QString name = ui->nameEdit->text();
    QString family = ui->familyEdit->text();
    QString patronymic = ui->patronymicEdit->text();
    QString password = ui->passwordEdit->text();
    AuthorizationDatabase db;
    if (db.openDatabase("database777.db")) {
        qDebug() << "База данных успешно открыта.";
        QString role = db.getUserRole(name, family, patronymic, password);
        int userId = db.getUserId(name, family, patronymic, password);
        int userIdAd = db.getUserId(name, family, patronymic, password);
        int userIdPr = db.getUserId(name, family, patronymic, password);
        if (role == "Администратор") {
            AdminHome *adminHome = new AdminHome(nullptr, userIdAd);
            adminHome->show();
            this->close();
        } else if (role == "Студент") {
            StudentHome *studentHome = new StudentHome(nullptr, userId);
            studentHome->show();
            this->close();
        } else if (role == "Преподаватель") {
            TeacherHome *teacherHome = new TeacherHome(nullptr, userIdPr);
            teacherHome->show();
            this->close();
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверные данные авторизации");
        }
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных");
    }
}
