#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidget>
#include <QDir>
#include <QMediaObject>
#include <QFileDialog>
#include "showplayword.h"
#include <QVariant>
#include<QMetaType>
#include<QSettings>

namespace Ui {
class MainWindow;
}

/* 媒体信息结构体 */
struct MediaObjectInfo {
    QString fileName;   // 用于保存歌曲文件名
    QString filePath;   //用于保存歌曲文件路径
};

/* 歌词信息结构体 */
struct WordObjectInfo {
    QString previousWord;
    QString currentWord;
    QString nextWord;
};

Q_DECLARE_METATYPE(WordObjectInfo);//注册该结构体类型

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mediaPlayerInit();

    void musicScan();

    void PlayMode();

    void Read_paly_LRC(const QString &path);

    void ConfigSettingFileSet();
    void ConfigSettingFileGet();

private slots:
    void on_btn_menu_clicked();

    void on_btn_path_clicked();






    void on_playlistWidget_itemDoubleClicked(QListWidgetItem *item);//双击歌单

    void mediaPlayerStateChanged(QMediaPlayer::State state); //歌曲状态改变时，播放按钮跟着变

    void musicPlayerDurationChanged(qint64 duration);   //曲目时长

    void musicPlayerPositionChanged(qint64 position);  //设置当前进度条为歌曲已播放的时长

    void mediaCurrentIndexChanged(int index);           //设置list为当前播放的歌曲

    void on_verticalSlider_sliderMoved(int position);

   // void musicPositionSendWord(qint64 position);//通过歌曲进度发送歌词


    void on_btn_previous_clicked();

    void on_btn_play_clicked();

    void on_btn_next_clicked();

    void on_btn_list_clicked();

    void on_btn_ShowWord_clicked();

    void on_horizontalSlider_sliderReleased();

private:
    Ui::MainWindow *ui;

    QMediaPlayer *musicPlayer=nullptr;
    QMediaPlaylist *mediaPlaylist=nullptr;
    /* 媒体信息存储 */
    QVector<MediaObjectInfo> mediaObjectInfo;

    bool listStyle=false;
    bool playWord=false;
    bool isWord=false;

      QDir dir;
      int voice=50;

      int Music_Cnt=0;//记录歌词行数
     QString Music_lrc[1024];

      ShowPlayWord *spw=new ShowPlayWord();
signals:

    void SendWord(QVariant variant);

};

#endif // MAINWINDOW_H
