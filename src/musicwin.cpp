#include "musicwin.h"


MusicWin::MusicWin(QWidget *parent)
    : QWidget{parent}
{
    //界面美化
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),50,50);
    setMask(bmp);
    setWindowOpacity(0.95);//设置透明度
    setStyleSheet("background-color:white;");
    //设置窗口状态为窗口置顶，不可调节大小
    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags|Qt::WindowStaysOnTopHint|Qt::MSWindowsFixedSizeDialogHint);
    this->setMinimumSize(600, 600);
    this->setMaximumSize(600, 600);
    this->setWindowIcon(QIcon(":/MizukiPng/musicIcon.png")); //设置窗口图标

    initBtn();//初始化按钮
    initMedia();//初始化媒体
}



void MusicWin::onDurationChanged(qint64 duration)
{
    progress->setMaximum(duration); //设置进度条最大值
    int secs = duration/1000; //全部秒数
    int mins = secs/60;//分
    secs = secs % 60;//秒
    durationTime = QString::asprintf("%d:%d",mins,secs);
    progressLabel->setText(positionTime+"/"+durationTime);
    progressLabel->show();
}

void MusicWin::onPositionChanged(qint64 position)
{
    if(progress->isSliderDown())
        return;//如果手动调整进度条，则不处理
    progress->setSliderPosition(position);
    int secs = position/1000;
    int mins = secs/60;
    secs = secs % 60;
    positionTime = QString::asprintf("%d:%d",mins,secs);
    progressLabel->setText(positionTime+"/"+durationTime);
    progressLabel->show();
}

void MusicWin::progressChangeEvent()
{
        player->setPosition(progress->value());
}


void MusicWin::initBtn()
{
    setStyleSheet("QPushButton{border: none;"
                  "background-color:rgb(255,255,255);border-radius: 35px;}"
                  "QPushButton::hover{background-color:rgb(200,200,200);}"
                  "QPushButton:pressed{background-color:rgb(160,160,160);}");

    next = new QPushButton(this);
    last = new QPushButton(this);
    pause = new QPushButton(this);
    play = new QPushButton(this);
    add = new QPushButton(this);
    clear = new QPushButton(this);
    //图标载入
    next->setIcon(QIcon(":/MizukiIcon/musicIcon/next.png"));
    last->setIcon(QIcon(":/MizukiIcon/musicIcon/last.png"));
    pause->setIcon(QIcon(":/MizukiIcon/musicIcon/pause.png"));
    play->setIcon(QIcon(":/MizukiIcon/musicIcon/play.png"));
    add->setIcon(QIcon(":/MizukiIcon/musicIcon/add.png"));
    clear->setIcon(QIcon(":/MizukiIcon/musicIcon/del.png"));

    //图标大小
    btnSize = 80;
    QSize temp(btnSize,btnSize);
    next->setIconSize(temp);
    last->setIconSize(temp);
    pause->setIconSize(temp);
    play->setIconSize(temp);
    add->setIconSize(temp);
    clear->setIconSize(temp);

    //按钮位置
    int x=60;
    int y=330;
    next->setGeometry(x+215,y,btnSize,btnSize);
    last->setGeometry(x+35,y,btnSize,btnSize);
    pause->setGeometry(x+120,y-5,btnSize+10,btnSize+10);
    play->setGeometry(x+120,y-5,btnSize+10,btnSize+10);
    add->setGeometry(x+300,y+5,btnSize-10,btnSize-10);
    clear->setGeometry(x-35,y+5,btnSize-15,btnSize-15);
    pause->hide();

    //连接按钮信号与对应槽函数
    connect(play,&QPushButton::clicked,this,&MusicWin::playBtnPush);
    connect(pause,&QPushButton::clicked,this,&MusicWin::pauseBtnPush);
    connect(next,&QPushButton::clicked,this,&MusicWin::nextBtnPush);
    connect(last,&QPushButton::clicked,this,&MusicWin::lastBtnPush);
    connect(add,&QPushButton::clicked,this,&MusicWin::addBtnPush);
    connect(clear,&QPushButton::clicked,this,&MusicWin::clearBtnPush);
}

void MusicWin::initMedia()
{
    //初始化播放器和播放列表
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);//设置循环模式
    player->setPlaylist(playlist);//获取将播放列表要播放的文件
    player->setVolume(60);//设置音量初始值
    musicName = new QPlainTextEdit(this);
    musicList = new QPlainTextEdit(this);
    //将文本框背景透明化
    QPalette pl = musicName->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(0,0,0,0)));
    musicName->setPalette(pl);
    musicList->setPalette(pl);
    musicList->setGeometry(50,50,350,120);
    musicList->setReadOnly(true);
    musicName->setGeometry(50,180,350,100);
    musicName->setReadOnly(true);
    musicList->setPlainText("播放队列：");
    musicName->setPlainText("当前播放：");
    //当播放文件playlist改变时，改变显示的音乐名
    connect(playlist,&QMediaPlaylist::currentMediaChanged,this,&MusicWin::musicChangeEvent);



    //音量滑动槽样式
    volume = new QSlider(this);
    volume->setMaximum(100);//设置最大值
    volume->setMinimum(0);//设置最小值
    volume->setValue(60);//设置初始值
    volume->setSingleStep(5);//设置步长
    volume->setOrientation(Qt::Horizontal);//水平样式
    volume->setTickPosition(QSlider::TicksAbove);//刻度线
    volume->setGeometry(40,430,330,30);//坐标和尺寸
    volume->setStyleSheet("QSlider::groove:horizontal{height:12px; left:0px; right:0px; border:0px; "
                          "border-radius:6px; background:rgb(242,242,242);}    \
                           QSlider::handle:horizontal{width:24px; background:#1644B0; border-radius:12px; margin:-6px 0px;}  \
                           QSlider::sub-page:horizontal{background:#4C85FB; border:0px; border-radius:6px;}");
    //进度条
    QFont ft;
    ft.setPointSize(15);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::darkBlue);
    progressLabel=new QLabel(this);
    progressLabel->setGeometry(400,280,120,30);
    progressLabel->setFont(ft);
    progressLabel->setPalette(pa);
    progressLabel->setStyleSheet("background-color: transparent;");
    progressLabel->show();
    //进度滑动条样式
    progress = new QSlider(this);
    progress->setMaximum(100);//设置最大值
    progress->setMinimum(0);//设置最小值
    progress->setValue(60);//设置初始值
    progress->setSingleStep(1);//设置步长
    progress->setOrientation(Qt::Horizontal);//水平样式
    progress->setTickPosition(QSlider::TicksAbove);//刻度线
    progress->setGeometry(40,280,330,30);//坐标和尺寸
    progress->setStyleSheet("QSlider::groove:horizontal{height:12px; left:0px; right:0px; border:0px; "
                            "border-radius:6px; background:rgb(242,242,242);}    \
                            QSlider::handle:horizontal{width:24px; background:#1644B0; border-radius:12px; margin:-6px 0px;}  \
                            QSlider::sub-page:horizontal{background:#4C85FB; border:0px; border-radius:6px;}");

    //音量和进度条连接槽函数
    connect(volume,&QSlider::valueChanged,this,&MusicWin::volumeChangeEvent);
    connect(progress,&QSlider::sliderReleased,this,&MusicWin::progressChangeEvent);//用鼠标释放信号连接不会造成值改变信号引起的卡顿
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));
}

void MusicWin::playBtnPush()
{
    if(player->state()==QMediaPlayer::PausedState){
        if(gifSwitch)
            gifSwitch=0;
        else
            gifSwitch=1;
        play->hide();
        pause->show();
        player->play();
    }
}
void MusicWin::pauseBtnPush()
{
    if(gifSwitch)
        gifSwitch=0;
    else
        gifSwitch=1;

    pause->hide();
    play->show();
    player->pause();
}

void MusicWin::nextBtnPush()
{
    if(player->state()!=QMediaPlayer::StoppedState)
        playlist->next();
}
void MusicWin::lastBtnPush()
{
    if(player->state()!=QMediaPlayer::StoppedState)
        playlist->previous();
}


//文件相关
void MusicWin::addBtnPush()
{
    //打开文件夹选择音频文件
    QString curPash = ":/music";//默认文件夹
    QString dlgTitle="选择音乐";
    QString filter="音频文件(*.mp3 *.wav *.wma)mp3文件(*.mp3);;wav文件(*.wav);;wma文件(*.wma);;所有文件(*.*)";
    QStringList fileListTemp = QFileDialog::getOpenFileNames(this,dlgTitle,curPash,filter);
    //空选
    if(fileListTemp.count()<1)
        return;
    //将文件加入播放列表
    for(int i = 0;i<fileListTemp.count();i++){
        QString aFile = fileListTemp.at(i);
        playlist->addMedia(QUrl::fromLocalFile(aFile));
        QFileInfo fileInfo(aFile);
        fileList<<aFile;//将文件路径加入文件名列表
        musicList->appendPlainText(QString::number(fileList.count())+"."+(fileListTemp.at(i).section(QRegExp("[/.]"),-2,-2)));
        //fileListTemp.at(i).section(QRegExp("[/.]"),-2,-2))能对文件路径进行分割，将音乐名分割出来
    }
    //第一次添加，改变动画与按钮
    if(player->state()==QMediaPlayer::StoppedState){
        if(gifSwitch)
            gifSwitch=0;
        else
            gifSwitch=1;

        playlist->setCurrentIndex(0);
        pause->show();
        play->hide();
        player->play();
    }

}
//清空播放列表
void MusicWin::clearBtnPush()
{
    if(player->state()!=QMediaPlayer::StoppedState){
        //先暂停播放器，断开列表与其它的槽的连接
        pauseBtnPush();
        player->stop();
        disconnect(playlist,&QMediaPlaylist::currentMediaChanged,this,&MusicWin::musicChangeEvent);
        playlist->clear();
        fileList.clear();
        musicList->clear();
        musicList->setPlainText("播放队列：");
        connect(playlist,&QMediaPlaylist::currentMediaChanged,this,&MusicWin::musicChangeEvent);
    }
}
//播放槽事件
void MusicWin::musicChangeEvent()
{
    musicName->setPlainText("当前播放：");
    musicName->appendPlainText((fileList.at(playlist->currentIndex()).section(QRegExp("[/.]"),-2,-2)));
}

void MusicWin::volumeChangeEvent()
{
    player->setVolume(volume->value());
}


