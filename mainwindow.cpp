#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include "QMessageBox"
#include <QClipboard>
#include <QFileDialog>
#include <windows.h>
#include <QTcpSocket>
#include <QtNetwork>
#include <qdebug.h>

#include <QCoreApplication>


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


bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist) //拷贝文件夹
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()),
                coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
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

    /*debug*/
    qDebug("%d %d\n",countsor,countbac);
    //qDebug() << countsor << ' ' << countbac << endl;

    if(countsor!=countbac) return false;
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
    clip->setText("Hi！这有一个软件可以备份和还原第五人格的软件！欢迎来下载哦！\n下载链接：https://chiqiao.rthe.net\n作者B站：赤桥");       //设置粘贴板内容

    QMessageBox::information(this,"已复制到剪贴板","已复制到剪贴板，直接粘贴给朋友就可以啦~");                     //提示对话框
    return;
}

quint64 dirFileSize(const QString &path)
{
    QDir dir(path);
    quint64 size = 0;
    //dir.entryInfoList(QDir::Files)返回文件信息
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
    {
        //计算文件大小
        size += fileInfo.size();
    }
    //dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
    foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        //若存在子目录，则递归调用dirFileSize()函数
        size += dirFileSize(path + QDir::separator() + subDir);
    }
    return size;
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
        //qDebug() << "新建目录是否成功" << res;
    }

    QString idv_backup_size="备份文件占用大小：";
    QString idv_video_size="录像文件占用大小：";

    long long backupSizeTemp, videoSizeTemp;
    backupSizeTemp = dirFileSize(idv_backup_dir)/1024/1024;    //备份文件占用大小
    videoSizeTemp = dirFileSize(idv_dir)/1024/1024;    //录像文件占用大小

    QString backupSizeTempString;
    while(backupSizeTemp){
        backupSizeTempString += backupSizeTemp%10+'0';
        backupSizeTemp/=10;
    }
    QString videoSizeTempString;
    while(videoSizeTemp){
        videoSizeTempString += backupSizeTemp%10+'0';
        videoSizeTemp/=10;
    }
    int backupSizeTempStringLen, videoSizeTempStringLen;
    backupSizeTempStringLen = backupSizeTempString.length();
    videoSizeTempStringLen = videoSizeTempString.length();
    while(backupSizeTempStringLen)
    {
        idv_backup_size += backupSizeTempString[backupSizeTempStringLen-1];
        backupSizeTempStringLen--;
    }
    while(videoSizeTempStringLen)
    {
        idv_video_size += videoSizeTempString[videoSizeTempStringLen-1];
        videoSizeTempStringLen--;
    }
    idv_backup_size += " MB";
    idv_video_size += " MB";
    ui->label->setText(idv_backup_size);   //备份文件占用大小
    ui->label_2->setText(idv_video_size); //录像文件占用大小
    return;
}


bool MainWindow::copyFile(QString &fromFIleName, QString &toFileName)
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
    ui->progressBar_copy->setValue(0);
    if(!dir_flag)
    {
        QMessageBox::warning(this,"警告","没有选择客户端路径，请先选择客户端路径后再试","OK");
        return;
    }
    else{
        /*
        //CopyThread::fileCopy(QString fileName)
        //ui->progressBar_copy->setValue(30);
        if(copyFile(idv_dir, idv_backup_dir))
        {
            //waitSeconds(5);

            //ui->progressBar_copy->setValue(100);
            QMessageBox::information(this,"备份","备份成功！","OK");
        }
        else
            QMessageBox::warning(this,"备份","备份失败！","OK");
        */
        //copyDirAndFile(idv_dir, idv_backup_dir);
        copyDirectoryFiles(idv_dir, idv_backup_dir, 0);
        double progress_backup_temp=0;
        while(!if_copy_final(idv_dir, idv_backup_dir))
        {
            if(progress_backup_temp<=97) progress_backup_temp+=0.01;
            ui->progressBar_copy->setValue(progress_backup_temp);
        }
        if(if_copy_final(idv_dir, idv_backup_dir))
        {
            ui->progressBar_copy->setValue(100);
            QMessageBox::information(this,"备份","备份成功！","OK");
        }
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
        copyDirectoryFiles(idv_backup_dir, idv_dir, 0);
        double progress_backup_temp=0;
        while(!if_copy_final(idv_backup_dir, idv_dir))
        {
            if(progress_backup_temp<=97) progress_backup_temp+=0.01;
            ui->progressBar_copy->setValue(progress_backup_temp);
        }
        if(if_copy_final(idv_backup_dir, idv_dir))
        {
            ui->progressBar_copy->setValue(100);
            QMessageBox::information(this,"备份","备份成功！","OK");
        }
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
        //qDebug() << "新建目录是否成功" << res;
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



