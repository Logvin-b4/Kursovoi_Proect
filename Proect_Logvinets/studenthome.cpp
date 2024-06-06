#include "studenthome.h"
#include "mainwindow.h"
#include "ui_studenthome.h"
#include "studentdatabase.h"
#include <QTime>
StudentHome::StudentHome(QWidget *parent, int userId) :
    QMainWindow(parent),
    ui(new Ui::StudentHome),
    userId(userId) // Инициализация userId
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0); // Устанавливаем первую вкладку как текущую

    setWindowTitle("Студент");
    setWindowIcon(QIcon(":/guf/guf.jpg"));

    // Соединяем сигнал выбора элемента в comboBox с соответствующим слотом

    // Используйте userId для загрузки данных студента
    StudentDataBase db;
    if (db.openDatabase("database777.db")) {
        QString name, family, patronymic, address, phone, group;
        if (db.getStudentData(userId, name, family, patronymic, address, phone, group)) {
            ui->NameEdit->setText(name);
            ui->SurnameEdit->setText(family);
            ui->PatronumicEdit->setText(patronymic);
            ui->AdressEdit->setText(address);
            ui->PhoneEdit->setText(phone);
            ui->GroupEdit->setText(group);
            ui->GroupEdit->setReadOnly(true);
            ui->NameEdit->setReadOnly(true);
            ui->SurnameEdit->setReadOnly(true);
            ui->PatronumicEdit->setReadOnly(true);
            ui->AdressEdit->setReadOnly(true);
            ui->PhoneEdit->setReadOnly(true);
        }
    }
    loadAvailableFacultatives();
    loadEnrolledFacultatives();
    connect(ui->comboBoxRegisterFacultativ, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StudentHome::loadFacultativeInfo);
    loadFacultativeInfo();
    connect(ui->comboBoxNewFacultativ, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StudentHome::loadNewFacultativeInfo);
    connect(ui->comboBoxRaspisanieFacultativov, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StudentHome::loadNewFacultativeInfo);
    loadNewFacultativeInfo();
    connect(ui->comboBoxConductFacultativ, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StudentHome::loadNewFacultativeTime);
    loadNewFacultativeTime();
    connect(ui->comboBoxYspevaemost, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StudentHome::loadGradeForFacultative);
    loadGradeForFacultative();


}

StudentHome::~StudentHome() {
    delete ui;
}
void StudentHome::loadAvailableFacultatives() {
    // Получаем группу студента
    QString group = ui->GroupEdit->text(); // Предполагая, что вы используете LineEdit для ввода группы

    // Получаем доступные факультативы для данной группы из базы данных
    QStringList availableFacultatives = studentDb.getAvailableFacultativesForGroup(group, userId);

    // Убираем повторяющиеся имена факультативов
    QSet<QString> uniqueFacultatives(availableFacultatives.begin(), availableFacultatives.end());
    QStringList uniqueFacultativesList;
    for (const QString &facultative : uniqueFacultatives) {
        uniqueFacultativesList.append(facultative);
    }

    // Загружаем факультативы в comboBoxRegisterFacultativ
    ui->comboBoxRegisterFacultativ->clear();
    ui->comboBoxRegisterFacultativ->addItems(uniqueFacultativesList);
    ui->comboBoxNewFacultativ->clear();
    ui->comboBoxNewFacultativ->addItems(uniqueFacultativesList);
}
void StudentHome::loadFacultativeInfo() {
    // Получаем название выбранного факультатива
    QString selectedFacultative = ui->comboBoxRegisterFacultativ->currentText();

    // Получаем группу студента
    QString group = ui->GroupEdit->text();

    // Получаем информацию о факультативе из базы данных
    QString conductPractic = "нет";
    QString conductLekctia = "нет";
    QString conductLab = "нет";

    // Получаем информацию о проведении факультатива из базы данных
    QStringList facultativeInfo = studentDb.getFacultativeInfo(selectedFacultative, group);

    // Проверяем информацию и заполняем соответствующие поля
    for(const QString &info : facultativeInfo) {
        if(info == "Практика") {
            conductPractic = "да";
        } else if(info == "Лекция") {
            conductLekctia = "да";
        } else if(info == "Лабораторная работа") {
            conductLab = "да";
        }
    }

    // Устанавливаем полученную информацию в поля
    ui->InfoConductPracticEdit->setText(conductPractic);
    ui->InfoConductLekctiaEdit->setText(conductLekctia);
    ui->InfoConductLabEdit->setText(conductLab);
    ui->InfoConductPracticEdit->setReadOnly(true);
    ui->InfoConductLekctiaEdit->setReadOnly(true);
    ui->InfoConductLabEdit->setReadOnly(true);

    // Получаем информацию о преподавателе для выбранного факультатива и группы
    QStringList teacherInfo = studentDb.getTeacherInfoForFacultative(selectedFacultative, group);

    // Устанавливаем полученную информацию о преподавателе в соответствующие поля
    if (teacherInfo.size() == 3) {
        ui->InfoPrepodavatelNameEdit->setText(teacherInfo.at(0)); // Имя преподавателя
        ui->InfoPrepodavatelSurnameEdit->setText(teacherInfo.at(1)); // Фамилия преподавателя
        ui->InfoPrepodavatelPatronumicEdit->setText(teacherInfo.at(2)); // Отчество преподавателя
        ui->InfoPrepodavatelNameEdit->setReadOnly(true);
        ui->InfoPrepodavatelSurnameEdit->setReadOnly(true);
        ui->InfoPrepodavatelPatronumicEdit->setReadOnly(true);

    } else {
        // Если размер списка не соответствует ожидаемому, что-то пошло не так
        qDebug() << "Ошибка: неверный формат информации о преподавателе";
    }
}

void StudentHome::loadNewFacultativeInfo() {
    // Получаем название выбранного факультатива
    QString selectedFacultative = ui->comboBoxNewFacultativ->currentText();
    QString selectedFacultativeStat = ui->comboBoxRaspisanieFacultativov->currentText();
    // Получаем группу студента
    QString group = ui->GroupEdit->text();

    // Получаем информацию о факультативе из базы данных
    QString conductPractic = "нет";
    QString conductLekctia = "нет";
    QString conductLab = "нет";

    // Получаем информацию о проведении факультатива из базы данных
    QStringList facultativeInfo = studentDb.getFacultativeInfo(selectedFacultative, group);
    QStringList facultativeInfoStat = studentDb.getFacultativeInfo(selectedFacultativeStat, group);

    // Проверяем информацию и заполняем соответствующие поля
    for(const QString &info : facultativeInfo) {
        if(info == "Практика") {
            conductPractic = "да";
        } else if(info == "Лекция") {
            conductLekctia = "да";
        } else if(info == "Лабораторная работа") {
            conductLab = "да";
        }
    }

    // Устанавливаем полученную информацию в поля
    ui->InfoPerezapisPracEdit->setText(conductPractic);
    ui->InfoPerezapisLekctiaEdit->setText(conductLekctia);
    ui->InfoPerezapisLabEdit->setText(conductLab);
    ui->InfoPerezapisPracEdit->setReadOnly(true);
    ui->InfoPerezapisLekctiaEdit->setReadOnly(true);
    ui->InfoPerezapisLabEdit->setReadOnly(true);
    ui->comboBoxConductFacultativ->clear();
    ui->comboBoxConductFacultativ->addItems(facultativeInfoStat);

}
void StudentHome::loadNewFacultativeTime() {
    QString group = ui->GroupEdit->text();
    QString selectedFacultativeStat = ui->comboBoxRaspisanieFacultativov->currentText();
    QString selectedConductType = ui->comboBoxConductFacultativ->currentText();
    QString facultativeTime = studentDb.getFacultativeTime(selectedFacultativeStat, selectedConductType, group);

    ui->TimeEdit->setText(facultativeTime);
    ui->TimeEdit->setReadOnly(true);
}
void StudentHome::loadGradeForFacultative() {
    QString selectedFacultative = ui->comboBoxYspevaemost->currentText();
    QString grade = studentDb.getGradeForFacultative(userId, selectedFacultative);
    ui->GradeEdit->setText(grade);
    ui->GradeEdit->setReadOnly(true);
}

void StudentHome::on_pushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
    this->close();
}

void StudentHome::on_Zapisatsya_clicked()
{

    // Получаем выбранный факультатив
    QString selectedFacultative = ui->comboBoxRegisterFacultativ->currentText();

    // Проверяем, что выбран факультатив
    if (selectedFacultative.isEmpty()) {
        qDebug() << "Выберите факультатив для записи";
        return;
    }

    // Вызываем метод добавления студента на факультатив из класса StudentDataBase
    if (studentDb.addStudentToFacultative(userId, selectedFacultative)) {
        qDebug() << "Студент успешно записан на факультатив";
        // Можно добавить код для обновления интерфейса или других действий после успешной записи
    } else {
        qDebug() << "Ошибка при записи студента на факультатив";
        // Можно добавить код для обработки ошибки
    }
    loadAvailableFacultatives();
    loadEnrolledFacultatives();
    loadNewFacultativeInfo();
    loadNewFacultativeTime();
}
void StudentHome::loadEnrolledFacultatives() {
    // Получаем факультативы, на которые записан студент
    QStringList enrolledFacultatives = studentDb.getEnrolledFacultatives(userId);

    // Очищаем combobox перед добавлением новых данных
    ui->comboBoxOldFaciulttativ->clear();
    ui->comboBoxRaspisanieFacultativov->clear();
    ui->comboBoxYspevaemost->clear();


    // Добавляем уникальные имена факультативов в combobox
    ui->comboBoxOldFaciulttativ->addItems(enrolledFacultatives);
    ui->comboBoxRaspisanieFacultativov->addItems(enrolledFacultatives);
    ui->comboBoxYspevaemost->addItems(enrolledFacultatives);
    loadGradeForFacultative();

}

void StudentHome::on_Perezapisatsya_clicked()
{
    // Получаем выбранный факультатив
    QString selectedFacultative = ui->comboBoxNewFacultativ->currentText();
    QString selectedoldFacultative = ui->comboBoxOldFaciulttativ->currentText();
    // Проверяем, что выбран факультатив
    if (selectedFacultative.isEmpty()) {
        qDebug() << "Выберите факультатив для записи";
        return;
    }
    if (selectedoldFacultative.isEmpty()) {
        qDebug() << "Выберите факультатив для перезаписи";
        return;
    }

    // Вызываем метод добавления студента на факультатив из класса StudentDataBase
    if (studentDb.addStudentToFacultative(userId, selectedFacultative)) {
        qDebug() << "Студент успешно записан на факультатив";
        // Можно добавить код для обновления интерфейса или других действий после успешной записи
    } else {
        qDebug() << "Ошибка при записи студента на факультатив";
        // Можно добавить код для обработки ошибки
    }
    // Вызываем метод удаления студента с факультатива из класса StudentDataBase
    if (studentDb.deleteStudentToFacultative(userId, selectedoldFacultative)) {
        qDebug() << "Студент успешно удален с факультатива";
        // Можно добавить код для обновления интерфейса или других действий после успешной записи
    } else {
        qDebug() << "Ошибка при удалении студента с факультатива";
        // Можно добавить код для обработки ошибки
    }
    loadAvailableFacultatives();
    loadEnrolledFacultatives();
}

