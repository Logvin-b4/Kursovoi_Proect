#include "teacherhome.h"
#include "ui_teacherhome.h"
#include "teacherdatabase.h"
#include "mainwindow.h"
#include <QMessageBox>
TeacherHome::TeacherHome(QWidget *parent, int userIdPr) :
    QMainWindow(parent),
    ui(new Ui::TeacherHome),
    userIdPr(userIdPr) // Инициализация userId
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0); // Устанавливаем первую вкладку как текущую

    setWindowTitle("Преподаватель");
    setWindowIcon(QIcon(":/guf/guf.jpg"));
    // Используйте userId для загрузки данных студента
    TeacherDataBase db;
    if (db.openDatabase("database777.db")) {
        QString name, family, patronymic;
        if (db.getStudentData(userIdPr, name, family, patronymic)) {
            ui->NamePrEdit->setText(name);
            ui->SurnamePrEdit->setText(family);
            ui->PatronumicPrEdit->setText(patronymic);
            ui->NamePrEdit->setReadOnly(true);
            ui->SurnamePrEdit->setReadOnly(true);
            ui->PatronumicPrEdit->setReadOnly(true);
        }
        // Загрузка данных факультативов
        QVector<QVector<QString>> facultativesData;
        if (db.getFacultatives(userIdPr, facultativesData)) {
            ui->tableWidget->setRowCount(facultativesData.size());
            ui->tableWidget->setColumnCount(4); // Задаем количество столбцов

            QStringList headers;
            headers << "Название" << "Вид проведения" << "Время" << "Группа";
                ui->tableWidget->setHorizontalHeaderLabels(headers);

            for (int row = 0; row < facultativesData.size(); ++row) {
                for (int col = 0; col < facultativesData[row].size(); ++col) {
                    ui->tableWidget->setItem(row, col, new QTableWidgetItem(facultativesData[row][col]));

                }
            }
            ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }
        // Загрузка уникальных названий факультативов в comboBoxGeade
        QStringList facultativeNames;
        if (db.getUniqueFacultativeNames(userIdPr, facultativeNames)) {
            ui->comboBoxGeade->addItems(facultativeNames);
            ui->ZapisStudentcomboBox->addItems(facultativeNames);
        }
    }
}


TeacherHome::~TeacherHome()
{
    delete ui;
}
void TeacherHome::on_pushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
    this->close();
}

void TeacherHome::on_pushGrade_clicked()
{
    QString studentName = ui->NameStudentEdit->text();
    QString studentFamily = ui->SurnameStudentEdit->text();
    QString studentPatronymic = ui->PatronumicStudentEdit->text();
    QString facultative = ui->comboBoxGeade->currentText();
    QString grade = ui->GradeEdit->text();

    if (studentName.isEmpty() || studentFamily.isEmpty() || studentPatronymic.isEmpty() || facultative.isEmpty() || grade.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
        return;
    }

    TeacherDataBase db;
    if (db.openDatabase("database777.db")) {
        if (db.isStudentEnrolledInFacultative(studentName, studentFamily, studentPatronymic, facultative)) {
            static QRegularExpression gradeRegExp("^[1-5]+$");
            if (gradeRegExp.match(grade).hasMatch()) {
                if (db.addGrade(studentName, studentFamily, studentPatronymic, facultative, grade)) {
                    QMessageBox::information(this, "Успех", "Оценка успешно добавлена.");
                    ui->NameStudentEdit->clear();
                    ui->SurnameStudentEdit->clear();
                    ui->PatronumicStudentEdit->clear();
                    ui->GradeEdit->clear();
                } else{
                    QMessageBox::warning(this, "Ошибка", "Ошибка при добавлении оценки.");
                }
            } else {
                QMessageBox::warning(this, "Ошибка", "Оценка может быть только от 1 до 5!");
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Студент не записан на выбранный факультатив.");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Ошибка при подключении к базе данных.");
    }
}

void TeacherHome::on_ViewButton_clicked()
{
    QString studentName = ui->NameStudentViewEdit->text();
    QString studentFamily = ui->SurnameStudentViewEdit->text();
    QString studentPatronymic = ui->PatronumicStudentViewEdit->text();

    if (studentName.isEmpty() || studentFamily.isEmpty() || studentPatronymic.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
        return;
    }

    TeacherDataBase db;
        QVector<QVector<QString>> schedule;
        QVector<QVector<QString>> grades;

        if (db.getStudentSchedule(studentName, studentFamily, studentPatronymic, schedule)) {
            ui->RaspisanietableWidget->setRowCount(schedule.size());
            ui->RaspisanietableWidget->setColumnCount(3); // 3 столбца: название, вид проведения, время

            QStringList scheduleHeaders;
            scheduleHeaders << "Название" << "Вид проведения" << "Время";
            ui->RaspisanietableWidget->setHorizontalHeaderLabels(scheduleHeaders);

            for (int row = 0; row < schedule.size(); ++row) {
                for (int col = 0; col < schedule[row].size(); ++col) {
                    ui->RaspisanietableWidget->setItem(row, col, new QTableWidgetItem(schedule[row][col]));
                }
            }
            ui->RaspisanietableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        } else {
            QMessageBox::warning(this, "Ошибка", "Ошибка при получении расписания студента.");
        }

        if (db.getStudentGrades(studentName, studentFamily, studentPatronymic, grades)) {
            ui->GradetableWidget->setRowCount(grades.size());
            ui->GradetableWidget->setColumnCount(2); // 2 столбца: название факультатива, оценка

            QStringList gradesHeaders;
            gradesHeaders << "Название факультатива" << "Оценка";
            ui->GradetableWidget->setHorizontalHeaderLabels(gradesHeaders);

            for (int row = 0; row < grades.size(); ++row) {
                for (int col = 0; col < grades[row].size(); ++col) {
                    ui->GradetableWidget->setItem(row, col, new QTableWidgetItem(grades[row][col]));
                }
            }
            ui->GradetableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        } else {
            QMessageBox::warning(this, "Ошибка", "Ошибка при получении оценок студента.");
        }
}


void TeacherHome::on_ZapispushButton_clicked()
{
        QString studentName = ui->NameStudentZapisEdit->text();
        QString studentFamily = ui->SurnameStudentZapisEdit->text();
        QString studentPatronymic = ui->PatronumicStudentZapisEdit->text();
        QString selectedFacultative = ui->ZapisStudentcomboBox->currentText();

        if (studentName.isEmpty() || studentFamily.isEmpty() || studentPatronymic.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
            return;
        }

        TeacherDataBase db;
        if (db.openDatabase("database777.db")) {
            if (db.addStudentToFacultative(studentName, studentFamily, studentPatronymic, selectedFacultative)) {
                QMessageBox::information(this, "Успех", "Студент успешно записан на факультатив.");
            } else {
                QMessageBox::warning(this, "Ошибка", "Ошибка при записи студента на факультатив.");
            }
        } else {
            QMessageBox::warning(this, "Ошибка", "Ошибка при подключении к базе данных.");
        }
}

