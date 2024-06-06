#ifndef ADMINHOME_H
#define ADMINHOME_H

#include <QMainWindow>

namespace Ui {
class AdminHome;
}

class AdminHome : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminHome(QWidget *parent = nullptr, int userIdAd = -1);
    ~AdminHome();



private slots:

    void on_pushButton_clicked();
    void on_RegisterButton_clicked();
    void on_deleteButton_clicked();
    void on_addFacultativ_clicked();

private:
    Ui::AdminHome *ui;
    int userIdAd; // Добавлено поле для хранения userId
};

#endif // ADMINHOME_H
