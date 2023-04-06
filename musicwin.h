#ifndef MUSICWIN_H
#define MUSICWIN_H

#include <QWidget>
#include <QBitmap>
#include <QPainter>
#include<QPushButton>
#include<QFileDialog>
#include<QPlainTextEdit>
#include<QtMultimedia/QMediaPlayer>
#include<QMediaPlayer>
#include<QtMultimedia/QMediaPlaylist>
#include<QSlider>
#include<QLabel>



class MusicWin : public QWidget
{
    Q_OBJECT
public:
    explicit MusicWin(QWidget *parent = nullptr);
    //动画控制
    inline int musicSwitcher(){return gifSwitch;}
private slots:
    //进度条的槽函数.必须放在私有槽域才可连接信号
    void onDurationChanged(qint64 duration); //文件时长变化，更新当前播放文件名显示
    void onPositionChanged(qint64 position); //当前文件播放位置变化，更新进度显示
private:
    int btnSize;
    int gifSwitch=0;

    QPushButton *next;//切换下一首按钮
    QPushButton *last;//切换上一首按钮
    QPushButton *pause;//暂停播放按钮
    QPushButton *play;//开始播放按钮
    QPushButton *add;//添加歌曲按钮
    QPushButton *clear;//清空歌曲按钮

    QPlainTextEdit *musicName;//显示当前播放音乐名
    QPlainTextEdit *musicList;//显示音乐列表

    QMediaPlayer * player;//多媒体播放器
    QMediaPlaylist * playlist;//播放列表

    QStringList fileList;//文件名列表

    QLabel *progressLabel;

    QSlider * volume;//音量滑动条
    QSlider * progress;//进度滑动条

    //进度条
    QString durationTime;
    QString positionTime;
    void progressChangeEvent();

    //初始化
    void initBtn();//初始化按钮
    void initMedia();//初始化多媒体

    //各按钮事件
    void playBtnPush();
    void pauseBtnPush();
    void nextBtnPush();
    void lastBtnPush();
    void addBtnPush();
    void clearBtnPush();
    //播放事件
    void musicChangeEvent();//切换音乐事件
    void volumeChangeEvent();//调节音量事件
};

#endif // MUSICWIN_H
