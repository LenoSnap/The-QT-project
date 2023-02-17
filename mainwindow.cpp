#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // qRegisterMetaType<MediaObjectInfo>("MediaObjectInfo");
    mediaPlayerInit();
    musicScan();        //扫描音乐
    ui->btn_play->setCheckable(true);
    ui->btn_play->setChecked(false);

    //双击单击歌曲列表
    connect(ui->playlistWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,SLOT(on_playlistWidget_itemDoubleClicked(QListWidgetItem*)));
    //音乐播放状态发生改变
    connect(musicPlayer,SIGNAL(stateChanged(QMediaPlayer::State)),
            this,SLOT(mediaPlayerStateChanged(QMediaPlayer::State)));
    //切歌时，更改当前进度条的时长
    connect(musicPlayer,SIGNAL(durationChanged(qint64)), // durationChanged(qint64 duration) 媒体持续改变信号 duration为时间
            this,SLOT(musicPlayerDurationChanged(qint64)));
    //设置当前进度条为歌曲已播放的时长  同时根据歌曲当前进度发送歌词
    connect(musicPlayer,SIGNAL(positionChanged(qint64)),
            this,SLOT(musicPlayerPositionChanged(qint64)));
    //设置list为当前播放的歌曲
    connect(mediaPlaylist,SIGNAL(currentIndexChanged(int)),
            this,SLOT(mediaCurrentIndexChanged(int)));
    //根据歌曲当前进度发送歌词
//    connect(musicPlayer,SIGNAL(positionChanged(qint64)),
//            this,SLOT(musicPositionSendWord(qint64)));
    connect( this,SIGNAL(SendWord(QVariant)),spw,SLOT(ReadWord(QVariant)));//发送歌词给词条界面

}

MainWindow::~MainWindow()
{
    ConfigSettingFileSet();

    delete ui;
}
void MainWindow::on_btn_path_clicked()
{
   dir=(QFileDialog::getExistingDirectory(this,"请选择音乐文件的目录",QDir::currentPath()));//QDir::homePath()  QDir::rootPath() QDir::tempPath()
//   QDir dirbsolutePath(dir.absolutePath());//absolutePath绝对路径
//   if (dirbsolutePath.exists())

 // qDebug()<<dir.absolutePath();
  musicScan();

}

void MainWindow::on_btn_menu_clicked()
{
    if(listStyle)
    {
        ui->playlistWidget->setStyleSheet("background-color: ;");
    }else{
        ui->playlistWidget->setStyleSheet("background-color:transparent;");
    }
    //rgb(255, 255, 255);

    listStyle=!listStyle;
}


void MainWindow::mediaPlayerInit()
{
    musicPlayer = new QMediaPlayer(this);
    mediaPlaylist = new QMediaPlaylist(this);

//    /* 确保列表是空的 */
//    mediaPlaylist->clear();
    /* 设置音乐播放器的列表为mediaPlaylist */
    musicPlayer->setPlaylist(mediaPlaylist);
    ConfigSettingFileGet();
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);   //设置播放模式，Loop是列循环

    musicPlayer->setVolume(voice); //设置音量
    ui->verticalSlider->setValue(voice);
    ui->playlistWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏滚动条
    ui->playlistWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏滚动条

   // PlayMode();
    ui->btn_ShowWord->setFlat(true);//去除歌词按钮边框
   // ui->btn_ShowWord->setStyleSheet("background-color:transparent;");/*按钮按下后没有那个凹陷*/

}

void MainWindow::musicScan(){
    // QDir dir1("C://Users//john//Music");

   QDir dirbsolutePath(dir.absolutePath());



    /* 如果目录存在 */
    if (dirbsolutePath.exists()) {
        QStringList filter; //过滤器
        filter << "*.mp3";  // 包含所有.mp3后缀的文件

        QFileInfoList files = dirbsolutePath.entryInfoList(filter, QDir::Files);    //获取该目录下的所有文件
       // QFileInfoList files=QFileDialog::getOpenFileNames(this,"请选择加入列表的歌曲",dirbsolutePath,filer);
        for (int i = 0; i < files.count(); i++) {
            MediaObjectInfo info;
            // 去除后续名 使用utf-8编码
            QString fileName = QString::fromUtf8(files.at(i).fileName()
                                                 .replace(".mp3", "").toUtf8().data());
            //info.fileName = fileName.split("-").at(0)+ "\n" + fileName.split("-").at(1); //歌曲名-歌手
             //info.fileName = fileName;  //不必分割
            info.fileName = fileName;
            info.filePath = QString::fromUtf8(files.at(i).filePath().toUtf8().data());

            /* 媒体列表添加歌曲 */
            if (mediaPlaylist->addMedia(QUrl::fromLocalFile(info.filePath))) {
                mediaObjectInfo.append(info);   // 添加到容器数组里储存
                ui->playlistWidget->addItem(info.fileName); // 添加歌曲名字至列表
            } else {
//                qDebug()<<mediaPlaylist->errorString().toUtf8().data()<< endl;
//                qDebug()<< "  Error number:"<< mediaPlaylist->error()<< endl;
                ui->label_currentPlay->setText("该曲目添加失误"+mediaPlaylist->error());
            }
        }
    }else{
        ui->label_currentPlay->setText("空目录！");
    }
}

void MainWindow::on_playlistWidget_itemDoubleClicked(QListWidgetItem *item)
{
    musicPlayer->stop();
    mediaPlaylist->setCurrentIndex(ui->playlistWidget->row(item));
    musicPlayer->play();
}

void MainWindow::mediaPlayerStateChanged(QMediaPlayer::State state){
    if(state == QMediaPlayer::PlayingState){
        ui->btn_play->setChecked(true);   //切换图标为暂停
    }else{
        ui->btn_play->setChecked(false);  //切换图标为播放
    }
}

void MainWindow::musicPlayerDurationChanged(qint64 duration){
     int second  = duration / 1000;
     int minute = second / 60;


     ui->horizontalSlider->setRange(0, second);

     second %= 60;

     QString mediaDuration;
     mediaDuration.clear();

     if (minute >= 10)
         mediaDuration = QString::number(minute, 10);
     else
         mediaDuration = "0" + QString::number(minute, 10);

     if (second >= 10)
         mediaDuration = mediaDuration
                 + ":" + QString::number(second, 10);
     else
         mediaDuration = mediaDuration
                 + ":0" + QString::number(second, 10);

     /* 显示媒体总长度时间 */
     ui->label_durationtime->setText(mediaDuration);
 }
void MainWindow::musicPlayerPositionChanged(qint64 position){

    if (!ui->horizontalSlider->isSliderDown())  //如果没有被按下
        ui->horizontalSlider->setValue(position/1000);  //设置进度
    static int num=5;
    WordObjectInfo infoWord;
    QVariant variant;

    qint64 wordposition=0;
    int second  = position / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaPosition;
    mediaPosition.clear();

    if (minute >= 10)
        mediaPosition = QString::number(minute, 10);
    else
        mediaPosition = "0" + QString::number(minute, 10);

    if (second >= 10)
        mediaPosition = mediaPosition
                + ":" + QString::number(second, 10);
    else
        mediaPosition = mediaPosition
                + ":0" + QString::number(second, 10);

    /* 显示现在播放的时间 */
    ui->label_positiontime->setText(mediaPosition);
    if(isWord){
        qint64 compete=mediaPosition.mid(0,2).toInt()*60+mediaPosition.mid(3,2).toInt();
          // qDebug()<<Music_lrc[5];
          wordposition=Music_lrc[num].mid(1,2).toInt()*60+Music_lrc[num].mid(4,2).toInt();
     // qDebug()<<"wordposition="<<wordposition;
     // qDebug()<<compete;
        for(int i=num;i<Music_Cnt;i++)//遍历
            {
                 wordposition=Music_lrc[i].mid(1,2).toInt()*60+Music_lrc[i].mid(4,2).toInt();
                  if(wordposition==compete)
                  {
                     infoWord.previousWord=Music_lrc[i-1];
                     infoWord.currentWord=Music_lrc[i];
                     infoWord.nextWord=Music_lrc[i+1];
                     //qDebug()<<Music_lrc[i-1];
                    // qDebug()<<Music_lrc[i];
                    // qDebug()<<Music_lrc[i+1];
                     variant.setValue(infoWord);
                     emit SendWord(variant);
                     num++;
     //                qDebug()<<position;
     //               // break;
                  }

            }

    }else{
        infoWord.previousWord="无歌词";
        infoWord.currentWord="无歌词";
        infoWord.nextWord="无歌词";
        emit SendWord(variant);
    }



}

void MainWindow::mediaCurrentIndexChanged(int index){

    if(-1 == index)
        return;

    ui->playlistWidget->setCurrentRow(index);   //设置当前播放的项目
    QString SongName= ui->playlistWidget->item(index)->text();

    ui->label_currentPlay->setText(SongName);
    QString currentplay=dir.absoluteFilePath(SongName+".lrc");//"qq音乐歌词文件后缀"
   // qDebug()<<currentplay;
    Read_paly_LRC(currentplay);

}

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    musicPlayer->setVolume(position);
}


void MainWindow::on_btn_previous_clicked()
{
   // musicPlayer->stop();
    if (0 == mediaPlaylist->mediaCount())   //播放列表为空
        return;

    /* 列表上一个 */
    mediaPlaylist->previous();
   // musicPlayer->play();
}

void MainWindow::on_btn_play_clicked()
{
    switch(musicPlayer->state()){
        case QMediaPlayer::StoppedState:    //暂停状态
            musicPlayer->play();    //媒体播放
            break;
        case QMediaPlayer::PlayingState:
            musicPlayer->pause();
            break;
        case QMediaPlayer::PausedState:
            musicPlayer->play();
            break;
    }
}

void MainWindow::on_btn_next_clicked()
{
    musicPlayer->stop();
    if (0 == mediaPlaylist->mediaCount())   //播放列表为空
        return;

    /* 列表下一个 */
    mediaPlaylist->next();
    musicPlayer->play();
}

void MainWindow::on_btn_list_clicked()
{
    PlayMode();
}
void MainWindow::PlayMode()
{
    switch(mediaPlaylist->playbackMode()){
        case QMediaPlaylist::Loop:
            mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);   //单曲循环
            ui->btn_list->setStyleSheet("border-image:url(:/images/btn_listcircle5.png)");
            break;
        case QMediaPlaylist::CurrentItemInLoop:
            mediaPlaylist->setPlaybackMode(QMediaPlaylist::Random);   //随机
            ui->btn_list->setStyleSheet(
                        "QPushButton{border-image:url(:/images/btn_listcircle3.png);}"
                        "QPushButton:hover{border-image:url(:/images/btn_listcircle4.png);}");
            break;
        case QMediaPlaylist::Random:
            mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);   //列表循环
            ui->btn_list->setStyleSheet(
                        "QPushButton{border-image:url(:/images/btn_listcircle1.png);}"
                        "QPushButton:hover{border-image:url(:/images/btn_listcircle2.png);}");
            break;
        default:break;
    }
}

void MainWindow::on_btn_ShowWord_clicked()
{
    if(playWord){

        ui->btn_ShowWord->setStyleSheet("QPushButton#btn_ShowWord{background-color:transparent;color: rgb(0, 85, 255);}");

        spw->show();
       // spw->setGeometry(130,800);
}
    else{
        ui->btn_ShowWord->setStyleSheet("QPushButton#btn_ShowWord{background-color:transparent;color: ;}");

        spw->hide();
    }
    playWord=!playWord;
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    // 设置媒体播放的位置
    musicPlayer->setPosition(ui->horizontalSlider->value() * 1000);
}
void MainWindow::Read_paly_LRC(const QString &path)
{
    QFile flie_data(path);
    Music_Cnt=0;
   // Music_lrc[].clear();
   if(flie_data.open(QIODevice::ReadOnly)==true)
   {
       isWord=true;
         //qDebug()<<"打开文件成功";
         while(!flie_data.atEnd())
         {
              QString buf=QString::fromLocal8Bit(flie_data.readLine());
              Music_lrc[Music_Cnt++]=buf;
              //qDebug()<<buf;
         }

   }else{
       isWord=false;
      // qDebug()<<"无歌词";
   }
   flie_data.close();
    Music_lrc[Music_Cnt].clear();
//         qDebug()<<Music_lrc[7].mid(1,2);
//           qDebug()<<Music_lrc[7].mid(4,2);
//            qDebug()<<Music_lrc[7].mid(7,2);
}
void MainWindow::ConfigSettingFileGet(){
    QSettings setting("../MusicPlayer_pro/set.ini",QSettings::IniFormat);//设置配置文件的目录和位置，如果有，则不动，没有，会自动创建

    if(setting.contains(tr("AboutInit/Dir"))&&setting.contains(tr("AboutInit/Volume")))//如果已经存在这个文件，那就进行读取

    {

    QString Dir=setting.value("AboutInit/Dir").toString();//将读取出的数据进行使用

    int Volume=setting.value("AboutInit/Volume").toInt();

    dir=Dir.toLocal8Bit();
    voice=Volume;
    qDebug()<<"已设置setting";

    }

}

void MainWindow::ConfigSettingFileSet(){

    //qDebug()<<dir.absolutePath().toLocal8Bit();
    QSettings setting("../MusicPlayer_pro/set.ini",QSettings::IniFormat);//设置配置文件的目录和位置，如果有，则不动，没有，会自动创建
    setting.beginGroup(tr("AboutInit"));//节点开始
    setting.setValue("Dir",dir.absolutePath().toLocal8Bit());
    setting.setValue("Volume",voice);//设置key和value，也就是参数和值
    setting.endGroup();//节点结束


}
