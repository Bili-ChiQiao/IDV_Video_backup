#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "virsion.h"
#include <QDesktopServices>
#include <QUrl>
#include "QMessageBox"
#include <QClipboard>
#include <QFileDialog>
#include <windows.h>
#include <QTcpSocket>
#include <QtNetwork>
#include <qdebug.h>
#include <windows.h>
#include <QCoreApplication>
#include <iostream>
#include <thread>
#include <QtConcurrent>
#include <cmath>
#include <cstring>
#include <Urlmon.h>


std::string vis_str;

bool dir_flag, dir_flag_p;
QString idv_dir;
QString idv_backup_dir, idv_backup_dir_p;
QString idv_dir_root;
QString idv_pickup_dir;
bool copyDirAndFile(QFileInfo file, QString path);
bool checkingUpdateResult = 0, whetherCheckingUpdateSucess;

//检查更新

//检查更新

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    checkingUpdates();
    ui->setupUi(this);
    setWindowTitle("第五人格录像备份软件 by赤桥");
    std::cout<<"------------Identity V Video Backup Software by ChiQiao-----------\n";
    std::cout<<"-----------------------------Debug-------------------------------\n";
    std::cout<<"-------------------------------------------Debug-----------------\n";
    std::cout<<"---------------------------------------Debug---------------------\n";
    std::cout<<"-----------------------------------Debug-------------------------\n";
    std::cout<<"--------------------------------Debug----------------------------\n";
    std::cout<<"----------------------------Debug--------------------------------\n";
    std::cout<<"------------------------Debug------------------------------------\n";
    std::cout<<"--------------------Debug----------------------------------------\n";
    std::cout<<"----------------Debug--------------------------------------------\n";
    std::cout<<"-----------------------------Debug-------------------------------\n";

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::checkingUpdates()  //检查更新
{
    std::cout<<"Checking Update..."<<std::endl;
    system("wget http://ftp6427026.host122.sanfengyun.cn/vis.txt");
    FILE *fp = freopen("vis.txt","r",stdin);
    std::cin>>vis_str;
    fclose(fp);
    system("cls");
    system("del vis.txt");
    if(vis_str.length()) {
        //检查到新版本
        whetherCheckingUpdateSucess = 1;    //记录查询版本成功
        std::cout<<"This Version: "<<this_vis_str<<std::endl;
        std::cout<<"Newest Version: "<<vis_str<<std::endl;
        if(vis_str==this_vis_str)
        {
            std::cout<<"This software is the latest version!"<<std::endl;
            checkingUpdateResult = 1;
        }
        else{
            std::cout<<"This software is not the latest version!"<<std::endl;
        }
    } else {
        std::cout<<"This Version: "<<this_vis_str<<std::endl;
        std::cout<<"Unable to search for version information. Please check network connection!"<<std::endl;
    }

}

bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist) //拷贝文件夹
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists()){    /*< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /*< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()),
                coverFileIfExist))
                return false;
        }
        else{            /*< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            // 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
        }
    }
    return true;
}

bool if_copy_final(QString pathsor, QString pathback)        //复制是否结束
{
    QString pathbac = pathback;
    //pathbac += 'video';

    QDir *sordir = new QDir(pathsor);   //sourse
    QStringList filtersor;      //sourse
    QList<QFileInfo> *fileInfosor=new QList<QFileInfo>(sordir->entryInfoList(filtersor)); //sourse
    int countsor=fileInfosor->count(); //sourse

    QDir *bacdir = new QDir(pathbac);   //backup
    QStringList filterbac;      //backup
    QList<QFileInfo> *fileInfobac=new QList<QFileInfo>(bacdir->entryInfoList(filterbac)); //backup
    int countbac=fileInfobac->count(); //backup

    printf("%d %d\n",countsor,countbac);    //debug

    if(countsor<countbac) return false;
    else return true;
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
    clip->setText("Hi！这有一个软件可以备份和还原第五人格录像的软件！欢迎来下载哦！\n下载链接：https://chiqiao.rthe.net/download.html\n作者B站：赤桥");       //设置粘贴板内容

    QMessageBox::information(this,"已复制到剪贴板","已复制到剪贴板，直接粘贴给朋友就可以啦~");                     //提示对话框
    return;
}

QString idv_backup_size, idv_video_size;
QString backupSizeTempString;
QString videoSizeTempString;
char videoch, backupch;
char unit;
qint64 curSize;

quint64 du(const QString &path)
{
    QDir dir(path); //这个类可以提供文件的目录和类容

    qint64 size = 0;
    //得到文件目录下的所有的文件和目录
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        size += fileInfo.size();    //把所有文件的大小加起来
    }

    //得到所有子目录下文件的大小
    //列出目录列表，不列出特殊的条目，“.”和".."
    foreach(QString subDir, dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot))
    {
        //路径+当前系统下的分割符+目录名
        size += du(path + QDir::separator() + subDir);    //递归调用，遍历所有目录，深度优先
    }

    unit = 'B';
    curSize = size;    //这个目录的大小

    if(curSize > 1024)
    {
        //当前的大小比1024个字节还大，上面计数是按字节大小得到的fileInfo.size()
        curSize /= 1024;  //除
        unit = 'K';   //KB

        if(curSize > 1024)
        {
            //还大
            curSize /= 1024;
            unit = 'M';   //MB

            if(curSize > 1024)
            {
                curSize /= 1024;
                unit = 'G';   //GB
            } //if
        }   //if
    } //if
    std::cout<<curSize<<unit<<"\t"<<qPrintable(path)<<std::endl;
    return size;
}

void pathSize()
{
    idv_backup_size="备份文件占用大小：";
    idv_video_size="录像文件占用大小：";
    backupSizeTempString="";
    videoSizeTempString="";

    int backupSizeTempStringLen, videoSizeTempStringLen;

    du(idv_backup_dir);    //备份文件占用大小
    if(curSize){
        while(curSize){
            backupSizeTempString += curSize%10+'0';
            curSize/=10;
        }
        backupSizeTempStringLen = backupSizeTempString.length();
        while(backupSizeTempStringLen)
        {
            idv_backup_size += backupSizeTempString[backupSizeTempStringLen-1];
            backupSizeTempStringLen--;
        }
    }
    else idv_backup_size += '0';
    backupch = unit;

    du(idv_dir);    //录像文件占用大小
    if(curSize){
        while(curSize){
            videoSizeTempString += curSize%10+'0';
            curSize/=10;
        }
        videoSizeTempStringLen = videoSizeTempString.length();
        while(videoSizeTempStringLen)
        {
            idv_video_size += videoSizeTempString[videoSizeTempStringLen-1];
            videoSizeTempStringLen--;
        }
    }
    else idv_backup_size += '0';
    videoch = unit;
    idv_backup_size += backupch;
    idv_video_size += videoch;
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
    idv_backup_dir += "/chiqiao_backup/video";

    //判断文件夹是否存在
    QDir *photo = new QDir;
    bool exist = photo->exists(idv_dir);

    if(!exist)
    {
        QMessageBox::warning(this,tr("选择路径"),tr("当前路径不存在录像文件夹！请检查是否选择正确文件夹"));
        MainWindow::on_pushButton_clicked();
        return;
    }

    QDir dirrr;
    if (!dirrr.exists(idv_backup_dir))
    {
        dirrr.mkpath(idv_backup_dir);
    }
    idv_backup_size="";
    idv_video_size="";
    pathSize();

    ui->label->setText(idv_backup_size);   //备份文件占用大小
    ui->label_2->setText(idv_video_size); //录像文件占用大小
    return;
}


bool MainWindow::copyFile(QString path1, QString path2, bool flaggg)
{
    copyDirectoryFiles(path1, path2, 0);
    double progress_backup_temp=0, time_temp=0;
    while(!if_copy_final(idv_dir, idv_backup_dir))
    {
        if(time_temp==10000){
            if(!flaggg) QMessageBox::warning(this,"备份","请求超时，备份失败！","OK");
            else QMessageBox::warning(this,"还原","请求超时，还原失败！","OK");
            return false;
        }
        if(progress_backup_temp<=97 && time_temp<=1000) progress_backup_temp+=0.01;
        else if(progress_backup_temp<=97) progress_backup_temp+=0.001;
        ui->progressBar_copy->setValue(progress_backup_temp);
        time_temp++;
    }
    if(if_copy_final(idv_dir, idv_backup_dir))
    {
        ui->progressBar_copy->setValue(100);
        if(!flaggg) QMessageBox::information(this,"备份","备份成功！","OK");
        else QMessageBox::information(this,"还原","还原成功！","OK");
    }
    return true;
}


void MainWindow::on_action_6_triggered()    //如何使用
{
    QDesktopServices::openUrl(QUrl(QString("https://chiqiao.rthe.net/howtouse.html")));
    return;
}



void MainWindow::on_pushButton_4_clicked()  //备份所有录像
{
    ui->progressBar_copy->setValue(0);
    if(!dir_flag)
    {
        QMessageBox::warning(this,"警告","没有选择客户端路径，请先选择客户端路径后再试","OK");
        return;
    }
    else{
        copyFile(idv_dir, idv_backup_dir, 0);
    }

    idv_backup_size="";
    idv_video_size="";
    pathSize();

    ui->label->setText(idv_backup_size);   //备份文件占用大小
    ui->label_2->setText(idv_video_size); //录像文件占用大小
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
        copyFile(idv_backup_dir, idv_dir, 1);
    }
    idv_backup_size="";
    idv_video_size="";
    pathSize();

    ui->label->setText(idv_backup_size);   //备份文件占用大小
    ui->label_2->setText(idv_video_size); //录像文件占用大小
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
        }
        return;
    }
    idv_backup_size="";
    idv_video_size="";
    pathSize();

    ui->label->setText(idv_backup_size);   //备份文件占用大小
    ui->label_2->setText(idv_video_size); //录像文件占用大小
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
        //qDebug() << "新建目录是否成功" << res;
    }
    idv_backup_size="";
    idv_video_size="";
    pathSize();

    ui->label->setText(idv_backup_size);   //备份文件占用大小
    ui->label_2->setText(idv_video_size); //录像文件占用大小
    return;
}

void MainWindow::on_action_IDV_Backup_triggered()
{
    //https://chiqiao.rthe.net/
    QDesktopServices::openUrl(QUrl(QString("https://chiqiao.rthe.net/download.html")));
}


//Windows
//Android



qint64 curSize_p;
char unit_p;
char videoch_p, backupch_p;
QString str_sor_dir_phone, idv_backup_size_p, idv_video_size_p, backupSizeTempString_p, videoSizeTempString_p;




void pathSize_p()
{
    idv_backup_size_p="备份文件占用大小：";
    idv_video_size_p="录像文件占用大小：";
    backupSizeTempString_p="";
    videoSizeTempString_p="";

    int backupSizeTempStringLen_p;

    du(idv_backup_dir_p);    //备份文件占用大小
    if(curSize_p){
        while(curSize_p){
            backupSizeTempString_p += curSize_p%10+'0';
            curSize_p/=10;
        }
        backupSizeTempStringLen_p = backupSizeTempString_p.length();
        while(backupSizeTempStringLen_p)
        {
            idv_backup_size_p += backupSizeTempString_p[backupSizeTempStringLen_p-1];
            backupSizeTempStringLen_p--;
        }
    }
    else idv_backup_size_p += '0';
    backupch_p = unit_p;
    idv_backup_size_p += backupch_p;
    return;
}


void MainWindow::on_pushButton_2_clicked()
{
    QtConcurrent::run([]()
    {
        system("adb devices");
    });
    QMessageBox msgBoxPhoneConnection;
    msgBoxPhoneConnection.setText("请确认控制台输出中“List of devices attached”下方是否有设备连接");
    msgBoxPhoneConnection.setInformativeText("如果有请按下“Ok”，否则请确认手机是否连接或USB调试是否打开");
    msgBoxPhoneConnection.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBoxPhoneConnection.setDefaultButton(QMessageBox::Ok);
    if(msgBoxPhoneConnection.exec()==QMessageBox::Cancel) return;
    //QMessageBox::information(this,"请确认","请确认控制台输出中“List of devices attached”下方是否有设备连接\n如果有请继续，否则请确认手机是否连接或USB调试是否打开","OK");
    ui->pushButton_3->setEnabled(true);
    return;
}

void MainWindow::on_pushButton_3_clicked()
{
    idv_backup_dir_p = QFileDialog :: getExistingDirectory (0,"请选择备份文件存储路径",".");
    int idv_backup_dir_p_len = idv_backup_dir_p.length();
    if(!idv_backup_dir_p_len)
        return;
    idv_backup_dir_p += "/chiqiao_backup";
    QDir dirrr_p;
    if (!dirrr_p.exists(idv_backup_dir_p))
    {
        dirrr_p.mkpath(idv_backup_dir_p);
        //qDebug() << "新建目录是否成功" << res;
    }
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_11->setEnabled(true);
    ui->pushButton_12->setEnabled(true);
    ui->label->setText(idv_backup_size_p);   //备份文件占用大小
    return;
}

void MainWindow::on_pushButton_9_clicked()      //备份
{
    QString idv_dir_p_temp;
    idv_dir_p_temp = "C:/cqbu/" ;
    //判断文件夹是否存在
    QDir dirrr;
    if (!dirrr.exists(idv_dir_p_temp))
    {
        dirrr.mkpath(idv_dir_p_temp);
    }
    //判断文件夹是否存在

    switch (ui->comboBox->currentIndex()) {
    case 0 : {
        //安卓官服
        system("adb pull storage/emulated/0/Android/data/com.netease.dwrg/files/netease/dwrg.common/Documents/video C:/cqbu/");
        break;
    }
    case 1: {
        //B服
        system("adb pull storage/emulated/0/Android/data/com.netease.dwrg.bili/files/netease/dwrg.common/Documents/video C:/cqbu/");
        break;
    }
    case 2: {
        //vivo服
        system("adb pull storage/emulated/0/Android/data/com.netease.dwrg5.vivo/files/netease/dwrg.common/Documents/video C:/cqbu/");
        break;
    }
    case 3: {
        //小米服
        system("adb pull storage/emulated/0/Android/data/com.netease.dwrg.mi/files/netease/dwrg.common/Documents/video C:/cqbu/");
        break;
    }
    case 4: {
        //华为服
        system("adb pull storage/emulated/0/Android/data/com.netease.dwrg.huawei/files/netease/dwrg.common/Documents/video C:/cqbu/");
        break;
    }
    case 5: {
        //应用宝服
        system("adb pull storage/emulated/0/Android/data/com.tencent.tmgp.dwrg/files/netease/dwrg.common/Documents/video C:/cqbu/");
        break;
    }
    case 6: {
        //魅族服
        system("adb pull storage/emulated/0/Android/data/com.netease.dwrg.mz/files/netease/dwrg.common/Documents/video C:/cqbu/");
        break;
    }
    case 7: {
        //OPPO 服    com.netease.dwrg.nearme.gamecenter
        system("adb pull storage/emulated/0/Android/data/com.netease.dwrg.nearme.gamecenter/files/netease/dwrg.common/Documents/video C:/cqbu/");
        break;
    }
    case 8: {
        //其他渠道服
        QMessageBox msgBox;
        msgBox.setText("反馈");
        msgBox.setInformativeText("我需要对其他渠道服的适配！请联系软件作者，作者会在后续版本进行适配！\n需要打开反馈网页吗？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        if(msgBox.exec()==QMessageBox::Cancel) return;
        else{
            QDesktopServices::openUrl(QUrl(QString("https://www.baidu.com/s?ie=UTF-8&wd=feedback")));
            return;
        }
        //请联系软件作者，会在后续版本进行适配！
        break;
    }
    default :
        QMessageBox::warning(this,tr("渠道服"),tr("当前为测试版，并未对渠道服第五人格进行适配，请期待后续版本!"));
        return;
        break;
    }

    ui->progressBar_copy->setValue(0);
    copyFile(idv_dir_p_temp, idv_backup_dir_p, 0);
    QString path = idv_dir_p_temp;
    QDir dir(path);
    dir.removeRecursively();
    idv_backup_size="";
    idv_video_size="录像文件占用大小：（手机内录像文件大小无法查看）";
    pathSize_p();
    ui->label->setText(idv_backup_size_p);   //备份文件占用大小
}

void MainWindow::on_pushButton_10_clicked()     //还原
{
    QString idv_dir_p_temp;
    idv_dir_p_temp = "C:/cqbu/" ;
    //判断文件夹是否存在
    QDir dirrr;
    if (!dirrr.exists(idv_dir_p_temp))
    {
        dirrr.mkpath(idv_dir_p_temp);
    }
    //判断文件夹是否存在
    ui->progressBar_copy->setValue(0);
    copyFile(idv_backup_dir_p,idv_dir_p_temp , 0);

    switch (ui->comboBox->currentIndex()) {
    case 0 : {
        //官服
        system("adb push C:/cqbu/ storage/emulated/0/Android/data/com.netease.dwrg/files/netease/dwrg.common/Documents/video");
        break;
    }
    case 1 : {
        //B服
        system("adb push C:/cqbu/ storage/emulated/0/Android/data/com.netease.dwrg.bili/files/netease/dwrg.common/Documents/video");
        break;
    }
    case 2 : {
        //vivo服
        system("adb push C:/cqbu/ storage/emulated/0/Android/data/com.netease.dwrg5.vivo/files/netease/dwrg.common/Documents/video");
        break;
    }
    case 3 : {
        //小米服
        system("adb push C:/cqbu/ storage/emulated/0/Android/data/com.netease.dwrg.mi/files/netease/dwrg.common/Documents/video");
        break;
    }
    case 4 : {
        //华为服
        system("adb push C:/cqbu/ storage/emulated/0/Android/data/com.netease.dwrg.huawei/files/netease/dwrg.common/Documents/video");
        break;
    }
    case 5 : {
        //应用宝服 com.tencent.tmgp.dwrg
        system("adb push C:/cqbu/ storage/emulated/0/Android/data/com.tencent.tmgp.dwrg/files/netease/dwrg.common/Documents/video");
        break;
    }
    case 6 : {
        //魅族服
        system("adb push C:/cqbu/ storage/emulated/0/Android/data/com.netease.dwrg.mz/files/netease/dwrg.common/Documents/video");
        break;
    }
    case 7 : {
        //OPPO 服    com.netease.dwrg.nearme.gamecenter
        system("adb push C:/cqbu/ storage/emulated/0/Android/data/com.netease.dwrg.nearme.gamecenter/files/netease/dwrg.common/Documents/video");
        break;
    }
    case 8 :{
        //其他渠道服
        QMessageBox msgBox;
        msgBox.setText("反馈");
        msgBox.setInformativeText("我需要对其他渠道服的适配！请联系软件作者，作者会在后续版本进行适配！\n需要打开反馈网页吗？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        if(msgBox.exec()==QMessageBox::Cancel) return;
        else{
            QDesktopServices::openUrl(QUrl(QString("https://www.baidu.com/s?ie=UTF-8&wd=feedback")));
            return;
        }
        //请联系软件作者，会在后续版本进行适配！
        break;
    }
    default :
        QMessageBox::warning(this,tr("渠道服"),tr("当前为测试版，并未对部分渠道服第五人格进行适配，请期待后续版本!"));
        return;
        break;
    }
    QString path = idv_dir_p_temp;
    QDir dir(path);
    dir.removeRecursively();
    idv_backup_size="";
    idv_video_size="录像文件占用大小：（手机内录像文件大小无法查看）";
    pathSize_p();
    ui->label->setText(idv_backup_size_p);   //备份文件占用大小
}

void MainWindow::on_pushButton_11_clicked()     //删除录像
{
    QMessageBox::warning(this,tr("操作失败！"),tr("无法在电脑上完成删除录像的操作，请在手机客户端上自行删除录像！"));
    return;
}

void MainWindow::on_pushButton_12_clicked()     //删除备份文件
{
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确实要删除吗?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if(msgBox.exec()==QMessageBox::Cancel) return;
    else{
        //检查是否存在备份文件夹
        QDir dirrr_p_exi;
        if (!dirrr_p_exi.exists(idv_backup_dir_p))
        {
            //备份文件夹不存在！
            QMessageBox::warning(this,tr("错误"),tr("当前选择的路径没有备份文件，请确认选择的是否正确！"));
            return;
        }

        QString path = idv_backup_dir_p;
        QDir dir(path);
        dir.removeRecursively();
        QMessageBox::information(this,"删除","删除成功！","OK");
        return;
    }
    QDir dirrr_p;
    if (!dirrr_p.exists(idv_backup_dir_p))
    {
        dirrr_p.mkpath(idv_backup_dir_p);
    }
    idv_backup_size="";
    idv_video_size="";
    pathSize();
    ui->label->setText(idv_backup_size);   //备份文件占用大小
    ui->label_2->setText(idv_video_size); //录像文件占用大小
    return;
}

void MainWindow::on_pushButton_5_clicked()  //清屏
{
    system("cls");
}

void MainWindow::on_pushButton_13_clicked() //ip地址
{
    system("ipconfig /all");
}

void doNotPushMe()
{

    //return;
}

void MainWindow::on_pushButton_14_clicked() //别点我
{
    QtConcurrent::run([]()
    {
        HWND hwnd= GetForegroundWindow();
        RECT r;
        GetWindowRect(hwnd,&r);
        int x=r.left,y=r.top, cnt;
        float i=0;
        while(1)
        {
            i+=0.02;
            MoveWindow(hwnd,x+sin(i)*500,y+cos(i)*500,1000,400,true);
            cnt++;
            Sleep(10);
        }
    });
    return;
}


void MainWindow::on_commandLinkButton_clicked()
{
    if(!whetherCheckingUpdateSucess) {
        //无法查询版本信息
        QMessageBox::warning(this,"检查更新","无法查询版本信息，请检查网络连接！","OK");
        return;
    }
    if(checkingUpdateResult==1) {
        //是最新版本！
        QMessageBox::information(this,"检查更新","软件是最新版本！","OK");
        return;
    }
    else {
        QString msg;
        QMessageBox msgBox;
        msgBox.setText("软件更新                                            ");
        msgBox.setInformativeText("软件不是最新版本！需要下载最新版本吗?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        if(msgBox.exec()==QMessageBox::Cancel) return;
        else{
            QDesktopServices::openUrl(QUrl(QString("https://chiqiao.rthe.net/download.html")));
            return;
        }
    }
}
