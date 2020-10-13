#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTcpServer>
#include <QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_triggered();

    void on_action_B_triggered();

    void on_action_8_triggered();

    void on_action_10_triggered();

    void on_action_13_triggered();

    void on_action_3_triggered();

    void on_pushButton_clicked();

    void on_action_6_triggered();


    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    //void on_pushButton_2_clicked(); //无此按钮

    //void on_pushButton_3_clicked(); //无此按钮
    void on_action_IDV_Backup_triggered();

    //void waitSeconds(int s);
    bool copyFile(QString &fromFIleName, QString &toFileName);
    //long long dirFileSize(QString);
    //bool deleteDir(const QString &path);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
