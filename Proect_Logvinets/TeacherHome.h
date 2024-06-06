#ifndef TEACHERHOME_H
#define TEACHERHOME_H

#include <QMainWindow>
#include <QStringList> // Для QStringList
#include "teacherdatabase.h"
namespace Ui {
class TeacherHome;
}

class TeacherHome : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeacherHome(QWidget *parent = nullptr, int userIdPr = -1);

    ~TeacherHome();
private slots:
    void on_pushButton_clicked();

    void on_pushGrade_clicked();

    void on_ViewButton_clicked();

    void on_ZapispushButton_clicked();

private:
    Ui::TeacherHome *ui;
    int userIdPr; // Добавлено поле для хранения userIdPr
    TeacherDataBase teacherDb;
};

#endif // TEACHERHOME_H
