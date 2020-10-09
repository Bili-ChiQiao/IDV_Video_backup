#include "mainwindow.h"
#include "copythread.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include "QMessageBox"
#include <QClipboard>
#include <QFileDialog>
#include <windows.h>
#include <QTcpSocket>
#include <QtNetwork>

#include <QCoreApplication>


//#include <djDraw.h>

bool dir_flag;
QString idv_dir;
QString idv_backup_dir;
QString idv_dir_root;
QString idv_pickup_dir;
bool copyDirAndFile(QFileInfo file, QString path);

//检查更新

//检查更新

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()      //菜单->退出
{
    exit(0);
    return;
}

void MainWindow::on_action_B_triggered()    //关于->B站
{
    QDesktopServices::openUrl(QUrl(QString("https://space.bilibili.com/442499600")));
    //checkUpdateFromChiQiao();
    return;
}

void MainWindow::on_action_8_triggered()    //关于->BiliLive
{
    QDesktopServices::openUrl(QUrl(QString("https://live.bilibili.com/21563266")));
    return;
}


void MainWindow::on_action_10_triggered()   //关于->qq群
{
    QDesktopServices::openUrl(QUrl(QString("https://jq.qq.com/?_wv=1027&k=5jiCDDd")));
    return;
}

void MainWindow::on_action_13_triggered()   //关于->关于作者
{
    QMessageBox::information(this,"关于作者","谢谢使用我的软件！\n作者：赤桥\nBilibili直播间：21563266   欢迎常来哦！\n感谢您的支持，我会继续努力的！\n欢迎来关注我的B站，有很多有意思的视频哦~~~");
    return;
}

void MainWindow::on_action_3_triggered()        //向朋友推荐这个软件
{
    QClipboard *clip=QApplication::clipboard();
    clip->setText("Hi！这有一个软件可以备份和还原第五人格的软件！欢迎来下载哦！\n下载链接：https://chiqiao.rthe.net\n作者B站：赤桥");       //设置粘贴板内容

    QMessageBox::information(this,"已复制到剪贴板","已复制到剪贴板，直接粘贴给朋友就可以啦~");                     //提示对话框
    return;
}

void MainWindow::on_pushButton_clicked()    //PushBottom    选择路径
{
    QString str_sor_dir = QFileDialog :: getExistingDirectory (0,"请选择客户端所在文件夹",".");
    idv_dir = str_sor_dir;
    idv_dir_root = str_sor_dir;
    idv_backup_dir = str_sor_dir;
    idv_pickup_dir = str_sor_dir;
    int a=str_sor_dir.length();
    if(!a) return;
    dir_flag++;
    idv_dir += "/Documents/video";
    idv_pickup_dir += "/Documents/";
    idv_backup_dir += "/chiqiao_backup/";

    //判断文件夹是否存在
    QDir *photo = new QDir;
    bool exist = photo->exists(idv_dir);

    if(!exist)
    {
        QMessageBox::warning(this,tr("创建文件夹"),tr("当前路径不存在录像文件夹！请检查是否选择正确文件夹"));
        MainWindow::on_pushButton_clicked();
        return;
    }


    QDir dirrr;
    if (!dirrr.exists(idv_backup_dir))
    {
        dirrr.mkpath(idv_backup_dir);
        //qDebug() << "新建目录是否成功" << res;
    }

    QString idv_backup_size="备份文件占用大小：123";
    QString idv_video_size="录像文件占用大小：123";
    ui->label->setText(idv_backup_size);
    ui->label_2->setText(idv_video_size);
    return;
}


bool MainWindow::copyFile(const QString &fromFIleName, const QString &toFileName)
{
    char* byteTemp = new char[4096];//字节数组
    int fileSize = 0;
    int totalCopySize = 0;
    QFile tofile;
    ui->progressBar_copy->setValue(0);
    tofile.setFileName(toFileName);
    QDataStream out(&tofile);
    out.setVersion(QDataStream::Qt_4_8);

    QFile fromfile;
    fromfile.setFileName(fromFIleName);
    if(!fromfile.open(QIODevice::ReadOnly)){
        qDebug() << "open fromfile failed！！！";
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        return false;
    }
    fileSize = fromfile.size();
    QDataStream in(&fromfile);

    in.setVersion(QDataStream::Qt_4_8);
    ui->progressBar_copy->setRange(0, fileSize);
    while(!in.atEnd())
    {
        int readSize = 0;
        //读入字节数组,返回读取的字节数量，如果小于4096，则到了文件尾
        readSize = in.readRawData(byteTemp, 4096);
        out.writeRawData(byteTemp, readSize);
        totalCopySize += readSize;
        ui->progressBar_copy->setValue(totalCopySize);
    }
    if(totalCopySize == fileSize){
        tofile.setPermissions(QFile::ExeUser);
        return true;
    }
    else
        return false;
}


void MainWindow::on_action_6_triggered()    //如何使用
{
    QDesktopServices::openUrl(QUrl(QString("https://space.bilibili.com/442499600")));
    return;
}

void MainWindow::on_pushButton_4_clicked()  //备份所有录像
{
    //ui->progressBar_copy->setValue(0);
    if(!dir_flag)
    {
        QMessageBox::warning(this,"警告","没有选择客户端路径，请先选择客户端路径后再试","OK");
        return;
    }
    else{
        //CopyThread::fileCopy(QString fileName)
        //ui->progressBar_copy->setValue(30);
        if(copyDirAndFile(idv_dir, idv_backup_dir))
        {
            //waitSeconds(5);

            //ui->progressBar_copy->setValue(100);
            QMessageBox::information(this,"备份","备份成功！","OK");
        }
        else
            QMessageBox::warning(this,"备份","备份失败！","OK");
    }

    return;
}

void MainWindow::on_pushButton_6_clicked()  //还原所有录像
{
    if(!dir_flag)
    {
        QMessageBox::warning(this,"警告","没有选择客户端路径，请先选择客户端路径后再试","OK");
        return;
    }
    else{
        QString huanYuan_dir;
        huanYuan_dir = idv_backup_dir;
        huanYuan_dir += "/video/";
        if(copyFile(huanYuan_dir, idv_pickup_dir))
            QMessageBox::information(this,"还原","还原成功！","OK");
        else
            QMessageBox::warning(this,"还原","还原失败！","OK");
    }
    return;
}

void MainWindow::on_pushButton_7_clicked()  //删除所有录像
{
    if(!dir_flag)
    {
        QMessageBox::warning(this,"警告","没有选择客户端路径，请先选择客户端路径后再试","OK");
        return;
    }
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确实要删除吗?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if(msgBox.exec()==QMessageBox::Cancel) return;
    else{
        //deleteDir(idv_dir);
        QString path = idv_dir;
        QDir dir(path);
        dir.removeRecursively();
        QMessageBox::information(this,"删除","删除成功！","OK");
        QDir dirrr;
        if (!dirrr.exists(idv_dir))
        {
            dirrr.mkpath(idv_dir);
            //qDebug() << "新建目录是否成功" << res;
        }
        return;
    }
    return;
}

void MainWindow::on_pushButton_8_clicked()  //删除所有备份文件
{
    if(!dir_flag)
    {
        QMessageBox::warning(this,"警告","没有选择客户端路径，请先选择客户端路径后再试","OK");
        return;
    }
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确实要删除吗?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if(msgBox.exec()==QMessageBox::Cancel) return;
    else{
        //deleteDir(idv_dir);
        QString path = idv_backup_dir;
        QDir dir(path);
        dir.removeRecursively();
        QMessageBox::information(this,"删除","删除成功！","OK");
        return;
    }
    QDir dirrr;
    if (!dirrr.exists(idv_backup_dir))
    {
        dirrr.mkpath(idv_backup_dir);
        //qDebug() << "新建目录成功";
    }
    return;
}

void MainWindow::on_action_IDV_Backup_triggered()
{
    //https://chiqiao.rthe.net/
    QDesktopServices::openUrl(QUrl(QString("https://chiqiao.rthe.net/")));
}


bool copyDirAndFile(QFileInfo file, QString path)
{
    //
    if (file.isDir()){
        QDir dir (file.filePath());
        foreach(auto fileName, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries))
            copyDirAndFile (fileName, path + dir.dirName () + "/");
    }
    QFile temp;
    QDir dir(path);
    if(!dir.exists())
        dir.mkpath (path);
    temp.copy(file.absoluteFilePath(), path + file.fileName());
    temp.close();
    //Sleep(10000);
    return true;
}



void MainWindow::on_action_2_triggered()
{
    QDesktopServices::openUrl(QUrl(QString("https://chiqiao.rthe.net/feedback.html")));
    return;
}
