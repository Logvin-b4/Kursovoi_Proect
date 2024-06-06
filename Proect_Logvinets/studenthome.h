#ifndef STUDENTHOME_H
#define STUDENTHOME_H

#include <QMainWindow>
#include <QStringList> // Для QStringList
#include "studentdatabase.h" // Включаем заголовочный файл StudentDataBase
namespace Ui {
class StudentHome;
}

class StudentHome : public QMainWindow {
    Q_OBJECT

public:
    explicit StudentHome(QWidget *parent = nullptr, int userId = -1);
    void loadAvailableFacultatives();
    void loadEnrolledFacultatives();

    ~StudentHome();

private slots:
    void loadFacultativeInfo();
    void loadNewFacultativeInfo();
    void loadNewFacultativeTime();
    void loadGradeForFacultative();
    void on_pushButton_clicked();


    void on_Zapisatsya_clicked();

    void on_Perezapisatsya_clicked();

private:
    Ui::StudentHome *ui;
    int userId; // Добавлено поле для хранения userId
    StudentDataBase studentDb;

};

#endif // STUDENTHOME_H
