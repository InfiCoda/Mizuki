#include "timerwin.h"



TimerWin::TimerWin(QWidget *parent)
    : QWidget(parent)
{
    //界面美化
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),50,50);
    setMask(bmp);
    setStyleSheet("background-color:white;");
    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags|Qt::WindowStaysOnTopHint|Qt::MSWindowsFixedSizeDialogHint);
    this->setMinimumSize(500, 400);
    this->setMaximumSize(500, 400);
     this->setWindowIcon(QIcon(":/MizukiPng/timer.png"));

    //计时器
    timer=new QTimer();
    timer->setInterval(1000);
    timer->start();

    initWin();
    resetTime();
}

void TimerWin::initWin()
{
    lcd=new QLCDNumber();
    lcd->setDigitCount(8);//lcd显示字数
    lcd->setMode(QLCDNumber::Dec);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    lcd->setStyleSheet("background-color:transparent;");
    //按钮与槽
    startButton=new QPushButton("Start");
    pauseButton=new QPushButton("Pause");
    resetButton=new QPushButton("Reset");

    hourUpButton=new QPushButton;
    hourDownButton=new QPushButton;
    minuteUpButton=new QPushButton;
    minuteDownButton=new QPushButton;
    secondUpButton=new QPushButton;
    secondDownButton=new QPushButton;

    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(start()));
    connect(pauseButton,SIGNAL(clicked(bool)),this,SLOT(pause()));
    connect(resetButton,SIGNAL(clicked(bool)),this,SLOT(reset()));

    connect(hourUpButton,SIGNAL(clicked(bool)),this,SLOT(clickHourUpButton()));
    connect(hourDownButton,SIGNAL(clicked(bool)),this,SLOT(clickHourDownButton()));
    connect(minuteUpButton,SIGNAL(clicked(bool)),this,SLOT(clickMinuteUpButton()));
    connect(minuteDownButton,SIGNAL(clicked(bool)),this,SLOT(clickMinuteDownButton()));
    connect(secondUpButton,SIGNAL(clicked(bool)),this,SLOT(clickSecondUpButton()));
    connect(secondDownButton,SIGNAL(clicked(bool)),this,SLOT(clickSecondDownButton()));

    //按钮样式
    hourUpButton->setIcon(QIcon(":/MizukiIcon/buttonIcon/up.png"));
    minuteUpButton->setIcon(QIcon(":/MizukiIcon/buttonIcon/up.png"));
    secondUpButton->setIcon(QIcon(":/MizukiIcon/buttonIcon/up.png"));
    hourDownButton->setIcon(QIcon(":/MizukiIcon/buttonIcon/down.png"));
    minuteDownButton->setIcon(QIcon(":/MizukiIcon/buttonIcon/down.png"));
    secondDownButton->setIcon(QIcon(":/MizukiIcon/buttonIcon/down.png"));

    btnSize = 25;
    QSize temp(btnSize,btnSize);
    hourUpButton->setIconSize(temp);
    minuteUpButton->setIconSize(temp);
    secondUpButton->setIconSize(temp);
    hourDownButton->setIconSize(temp);
    minuteDownButton->setIconSize(temp);
    secondDownButton->setIconSize(temp);

    //竖直垂直布局
    vLayout = new QVBoxLayout();
    hLayout1 = new QHBoxLayout();
    hLayout2 = new QHBoxLayout();
    hLayout3 = new QHBoxLayout();

    hLayout1->addWidget(hourUpButton);
    hLayout1->addWidget(minuteUpButton);
    hLayout1->addWidget(secondUpButton);
    vLayout->addLayout(hLayout1);

    vLayout->addWidget(lcd);

    hLayout2->addWidget(hourDownButton);
    hLayout2->addWidget(minuteDownButton);
    hLayout2->addWidget(secondDownButton);
    vLayout->addLayout(hLayout2);

    hLayout3->addWidget(startButton);
    hLayout3->addWidget(pauseButton);
    hLayout3->addWidget(resetButton);
    vLayout->addLayout(hLayout3);

    this->setLayout(vLayout);
    this->resize(500,300);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
}

void TimerWin::onTimeout(){
    QString s_hour=QString::number(hour);
    QString s_minute=QString::number(minute);
    QString s_second=QString::number(second);

    if (s_hour.length()==1)   s_hour="0"+s_hour;
    if (s_minute.length()==1)   s_minute="0"+s_minute;
    if (s_second.length()==1)    s_second="0"+s_second;
    QString disp=s_hour+":"+s_minute+":"+s_second;
    lcd->display(disp);
    //倒计时逻辑
    if((hour!=0)|(minute!=0)|(second!=0)){
        if(minute==0&&hour!=0){
            minute=59;
            hour--;
        }
        if(second==0&&minute!=0){
            second=59;
            minute--;}
        else
            second--;
    }
    //倒计时结束
    else{
        gifSwitch=0;
        this->show();
        lcd->setStyleSheet("background-color:red");
    }
}

void TimerWin::resetTime()
{
    QString s_hour=QString::number(hour);
    QString s_minute=QString::number(minute);
    QString s_second=QString::number(second);

    if (s_hour.length()==1)   s_hour="0"+s_hour;
    if (s_minute.length()==1)   s_minute="0"+s_minute;
    if (s_second.length()==1)    s_second="0"+s_second;
    QString disp=s_hour+":"+s_minute+":"+s_second;
    lcd->display(disp);
}

void TimerWin::start(){
    //gif控制器,计时状态控制器
    gifSwitch=1;
    if(!timerOn)
        connect(timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    timerOn=true;

    this->lcd->setStyleSheet("background-color:white");
}

void TimerWin::pause()
{
    gifSwitch=0;
    timerOn=false;
    this->lcd->setStyleSheet("background-color:white");
    disconnect(timer,SIGNAL(timeout()),this,SLOT(onTimeout()));  
}

void TimerWin::reset()
{
    pause();

    this->hour=0;
    this->minute=0;
    this->second=0;
    resetTime();
}

void TimerWin::clickHourUpButton()
{
    if(timerOn)
        return;
    if(hour<24)
        this->hour++;
    else
        this->hour=0;
    resetTime();
}

void TimerWin::clickHourDownButton()
{
    if(timerOn)
        return;
    if(hour>0)
        this->hour--;
    else
        this->hour=23;
    resetTime();
}

void TimerWin::clickMinuteUpButton()
{
    if(timerOn)
        return;
    if(minute<59)
        this->minute++;
    else
        this->minute=0;
    resetTime();
}

void TimerWin::clickMinuteDownButton()
{
    if(timerOn)
        return;
    if(minute>0)
        this->minute--;
    else
        this->minute=59;
    resetTime();
}

void TimerWin::clickSecondUpButton()
{
    if(timerOn)
        return;
    if(second<59)
        this->second++;
    else
        this->second=0;
    resetTime();
}

void TimerWin::clickSecondDownButton()
{
    if(timerOn)
        return;
    if(second>0)
        this->second--;
    else
        this->second=59;
    resetTime();
}

