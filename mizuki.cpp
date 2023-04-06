#include "mizuki.h"
#include "ui_mizuki.h"

Mizuki::Mizuki(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Mizuki)
{
    ui->setupUi(this);

    //QApplication::alert(this);任务栏图标闪烁
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);//去掉窗口标题
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    Qt::WindowFlags m_flags = windowFlags();//保持窗口置顶1
    setWindowFlags(m_flags|Qt::WindowStaysOnTopHint);//保持窗口置顶2

    //加载动画资源
    moveGif=new QMovie(":/MizukiGif/move.gif");
    timerGif=new QMovie(":/MizukiGif/timer.gif");
    questionGif=new QMovie(":/MizukiGif/question.gif");
    normalGif=new QMovie(":/MizukiGif/normal.gif");
    musicGif=new QMovie(":/MizukiGif/timer.gif");

    body = new QLabel(this);
    body->setMovie(normalGif);
    //初始body大小
    size=1000;
    body->resize(size,size);
    body->show();

    //初始化按键
    initBody();
    initBtn();
    initSystemTray();

    //timer控制动画切换
    timer = new QTimer;
    timer->start(50);

    connect(timer,&QTimer::timeout,this,
            static_cast<void (Mizuki::*)()>(&Mizuki::gifSet));//gifSet函数重载

    gifSet(body,normalGif);
}

Mizuki::~Mizuki()
{
    delete ui;

    delete closeBtn;
    delete unfoldBtn;
    delete foldBtn;
    delete minBtn;
    delete setBtn;
    delete musicBtn;
    delete timerBtn;
    delete pSystemTray;

    delete setWin;
    delete musicWin;
    delete timerWin;
}



/****按钮属性*****/
//创建按钮，调用reInitBtn初始化大小位置，连接按钮与槽
void Mizuki::initBtn()
{
    //创建按钮，设置按钮样式
    setBtn=new QPushButton(this);
    minBtn=new QPushButton(this);
    unfoldBtn=new QPushButton(this);
    foldBtn=new QPushButton(this);
    closeBtn=new QPushButton(this);
    musicBtn = new QPushButton(this);//音乐按钮
    timerBtn = new QPushButton(this);//计时器按钮
    setBtn->setIcon(QIcon(":/MizukiIcon/buttonIcon/setwin.png"));
    minBtn->setIcon(QIcon(":/MizukiIcon/buttonIcon/del.png"));
    unfoldBtn->setIcon(QIcon(":/MizukiIcon/buttonIcon/unfold.png"));
    foldBtn->setIcon(QIcon(":/MizukiIcon/buttonIcon/fold.png"));
    closeBtn->setIcon(QIcon(":/MizukiIcon/buttonIcon/close.png"));
    musicBtn->setIcon(QIcon(":/MizukiIcon/musicIcon/music.png"));
    timerBtn->setIcon(QIcon(":/MizukiIcon/buttonIcon/timer.png"));

    setStyleSheet("QPushButton{border:4px solid blue;"
                  "background-color:rgb(200,210,255);border-radius: 10px;}"
                  "QPushButton::hover{background-color:rgb(170,200,255);}"
                  "QPushButton:pressed{background-color:rgb(60,70,200);}");

    //初始化按钮参数
    reInitBtn();

    //创建窗口
    setWin=new SetWin;
    musicWin=new MusicWin;
    timerWin=new TimerWin;

    //初始化窗口参数
    setWin->setSize(size);


    //连接按钮与对应槽
    connect(setBtn,&QPushButton::clicked,this,&Mizuki::setBtnPush);
    connect(minBtn,&QPushButton::clicked,this,&Mizuki::minBtnPush);
    connect(unfoldBtn,&QPushButton::clicked,this,&Mizuki::moreBtnPush);
    connect(foldBtn,&QPushButton::clicked,this,&Mizuki::moreBtnPush);
    connect(closeBtn,&QPushButton::clicked,this,&Mizuki::closeBtnPush);
    connect(musicBtn,&QPushButton::clicked,this,&Mizuki::musicBtnPush);
    connect(timerBtn,&QPushButton::clicked,this,&Mizuki::timerBtnPush);

    //初始化控制器
    btnSwitch=0;
    moreBtnSwitch=0;
    foldSwitch=0;
    winSwitch=0;
    moreWinSwitch=0;
    btnSwitcher();
    moreBtnSwitcher();
    winSwitcher();
    moreWinSwitcher();
}

//调节按钮大小,坐标
void Mizuki::reInitBtn()
{
    btnSize = size;    
    //限制按钮大小
    if(btnSize < 300)
         btnSize = 300;
    //按钮的坐标参数
    int btnX = this->frameGeometry().width()/2 - btnSize*3/5-5;
    int btnY = this->frameGeometry().height()/2 - btnSize/4;
    int btnWidth = btnSize/5;
    int btnHeight = btnSize/8;

    //按钮坐标
    closeBtn->setGeometry(btnX,btnY,btnWidth,btnHeight);
    minBtn->setGeometry(btnX,btnY + btnSize/6,btnWidth,btnHeight);
    setBtn->setGeometry(btnX,btnY + 2*btnSize/6,btnWidth,btnHeight);
    unfoldBtn->setGeometry(btnX,btnY + 3*btnSize/6,btnWidth,btnHeight);
    foldBtn->setGeometry(btnX,btnY + 3*btnSize/6,btnWidth,btnHeight);
        //more中的按钮
    musicBtn->setGeometry(btnX - btnWidth*1.2,btnY + btnSize/6,btnWidth,btnHeight);
    timerBtn->setGeometry(btnX - btnWidth*1.2,btnY,btnWidth,btnHeight);

    //按钮大小
    QSize temp(btnSize/8,btnSize/8);
    setBtn->setIconSize(temp);
    minBtn->setIconSize(temp);
    unfoldBtn->setIconSize(temp);
    foldBtn->setIconSize(temp);
    closeBtn->setIconSize(temp);
    musicBtn->setIconSize(temp);
    timerBtn->setIconSize(temp);
}



/**文件操作**/
void Mizuki::saveData()
{
    QFile file("./file.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out<<size<<x()<<y();//存储体型、窗口坐标
    file.close();
}

void Mizuki::initBody()
{
    //读取文件的属性
    int coordX,coordY;//桌面坐标
    QFile file("./file.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    if(file.isOpen())
        in>>size>>coordX>>coordY;
    //没有记录就采用默认坐标,体型
    else{
        size = 400;
        coordX = x();
        coordY = y();
    }
    file.close();
    move(coordX,coordY);
}


/****按钮点击事件*****/
//三个窗口相关按钮
void Mizuki::setBtnPush()
{
    if(setWin->isHidden()){
        //移动窗口坐标
        setWin->move(x()+frameGeometry().width()/2
        -btnSize*(btnSwitch+moreBtnSwitch+1.5)/4-setWin->frameGeometry().width(),
        y()+frameGeometry().height()/2-size/5
        -setWin->frameGeometry().height()/2);

        moreBtnSwitch=0;
        moreBtnSwitcher();
        setWin->show();
        musicWin->hide();
        timerWin->hide();

    }
    else
        setWin->hide();
}

void Mizuki::musicBtnPush()
{
    if(musicWin->isHidden()){
        //移动窗口坐标↓
        musicWin->move(x()+frameGeometry().width()/2
        -btnSize*(btnSwitch+moreBtnSwitch+1.5)/4-musicWin->frameGeometry().width(),
        y()+frameGeometry().height()/2-size/5
        -musicWin->frameGeometry().height()/2);

        musicWin->show();
        setWin->hide();
        timerWin->hide();
    }
    else
        musicWin->hide();
}

void Mizuki::timerBtnPush()
{
    if(timerWin->isHidden()){
        timerWin->move(x()+frameGeometry().width()/2
        -btnSize*(btnSwitch+moreBtnSwitch+1.5)/4-timerWin->frameGeometry().width(),
        y()+frameGeometry().height()/2-size/5
        -timerWin->frameGeometry().height()/2);

        timerWin->show();
        musicWin->hide();
        setWin->hide();
    }
    else
        timerWin->hide();
}

//关闭按钮
void Mizuki::closeBtnPush()
{
    setWin->close();
    musicWin->close();
    timerWin->close();
    this->close();
}

//最小化按钮
void Mizuki::minBtnPush()
{
    //关闭全部窗口
    winSwitch=0;
    winSwitcher();
    this->hide();
    //关闭全部按钮
    btnSwitch=0;
    moreBtnSwitch=0;
    btnSwitcher();
    moreBtnSwitcher();
}

//更多按钮
void Mizuki::moreBtnPush()
{
    if(moreBtnSwitch==0)
        moreBtnSwitch=1;
    else
        moreBtnSwitch=0;

    if(foldSwitch){
        unfoldBtn->show();
        foldBtn->hide();
        foldSwitch=0;
    }
    else{
        unfoldBtn->hide();
        foldBtn->show();
        foldSwitch=1;
    }

    moreBtnSwitcher();
    setWin->hide();
}

//托盘按钮
void Mizuki::systemTrayPush()
{
    if(this->isHidden())
        this->show();
}

//系统托盘属性
void Mizuki::initSystemTray()
{
    pSystemTray = new QSystemTrayIcon(this);
    pSystemTray->setIcon(QIcon(":/MizukiPng/tray.png"));
    pSystemTray->setToolTip("Mizuki");
    pSystemTray->show();
    connect(pSystemTray,&QSystemTrayIcon::activated,this,&Mizuki::systemTrayPush);
}


/**按钮窗口控制器**/
//按钮显示控制器,分为两层显示
void Mizuki::btnSwitcher()
{
    setBtn->setVisible(btnSwitch);
    minBtn->setVisible(btnSwitch);
    unfoldBtn->setVisible(btnSwitch);
    foldBtn->setVisible(btnSwitch);
    closeBtn->setVisible(btnSwitch);

}
void Mizuki::moreBtnSwitcher()
{
    musicBtn->setVisible(moreBtnSwitch);
    timerBtn->setVisible(moreBtnSwitch);
}
//窗口显示控制器,同样为两层，主窗口单独操作不写函数
void Mizuki::winSwitcher()
{
    setWin->setVisible(winSwitch);
    musicWin->setVisible(winSwitch);
    timerWin->setVisible(winSwitch);
}
void Mizuki::moreWinSwitcher()
{
    musicWin->setVisible(moreWinSwitch);
    timerWin->setVisible(moreWinSwitch);
    //开关按钮窗口坐标控制
    musicWin->move(x()+frameGeometry().width()/2
    -btnSize*(btnSwitch+moreBtnSwitch+1.5)/4-musicWin->frameGeometry().width(),
    y()+frameGeometry().height()/2-size/5
                   -musicWin->frameGeometry().height()/2);

    timerWin->move(x()+frameGeometry().width()/2
    -btnSize*(btnSwitch+moreBtnSwitch+1.5)/4-timerWin->frameGeometry().width(),
    y()+frameGeometry().height()/2-size/5
                   -timerWin->frameGeometry().height()/2);
}



/****鼠标事件*****/
//鼠标移动事件实现拖动全部窗口,移动时窗口坐标控制
void Mizuki::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)//鼠标左键按下并移动时，实现拖动窗口
    {
        if(!gifSwitch){
            body->setMovie(moveGif);
            gifSet(body,moveGif);
        }
        this->move(event->globalPos()-moveLeftTop);

        musicWin->move(x()+frameGeometry().width()/2
        -btnSize*(btnSwitch+moreBtnSwitch+1.5)/4-musicWin->frameGeometry().width(),
        y()+frameGeometry().height()/2-size/5
        -musicWin->frameGeometry().height()/2);

        setWin->move(x()+frameGeometry().width()/2
        -btnSize*(btnSwitch+moreBtnSwitch+1.5)/4-setWin->frameGeometry().width(),
        y()+frameGeometry().height()/2-size/5
        -setWin->frameGeometry().height()/2);

        timerWin->move(x()+frameGeometry().width()/2
        -btnSize*(btnSwitch+moreBtnSwitch+1.5)/4-timerWin->frameGeometry().width(),
        y()+frameGeometry().height()/2-size/5
        -timerWin->frameGeometry().height()/2);
    }
}
//鼠标松开事件
void Mizuki::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->type() == QEvent::MouseButtonRelease){
        if(!gifSwitch){
            body->setMovie(normalGif);
            gifSet(body,normalGif);
        }
    }
}
//鼠标点击事件
void Mizuki::mousePressEvent(QMouseEvent *event)
{
    //鼠标左击事件，用于控制窗口拖动
    if(event->button() == Qt::LeftButton){
        moveLeftTop = event->pos();
    }


    //鼠标右击事件,用btnSwitch和btnSwitcher控制
    else if(event->button() == Qt::RightButton){
        //按钮开关
        if(btnSwitch){
            btnSwitch=0;
            moreBtnSwitch=0;
        }
        else
            btnSwitch=1;
        btnSwitcher();
        moreBtnSwitcher();
        //窗口开关
        if(winSwitch)
            winSwitch=0;

        winSwitcher();
   }
}



/*******动画*******/
//设置动画参数,含参为初始化
void Mizuki::gifSet(QLabel *body, QMovie* gif)
{
    gifSwitch=0;
    gifChange=0;
    gif->start();
    gif->setSpeed(70);
    size=setWin->getSize();
    body->resize(size,size);
    bodySize=body->size();
    gif->setScaledSize(bodySize);
}
//不含参函数与timer连接，设置中改变大小时刷新Mizuki
void Mizuki::gifSet()
{
    //定时器模式或音乐模式
    musicGifSwitch=musicWin->musicSwitcher();
    timerGifSwitch=timerWin->timerSwitcher();
    gifSwitch=musicGifSwitch|timerGifSwitch;
    if(gifSwitch){
        if(musicGifSwitch){
            body->setMovie(musicGif);
            gifSet(body,musicGif);
        }
        if(timerGifSwitch){
            body->setMovie(timerGif);
            gifSet(body,timerGif);
        }
    }
    //重新调整qlabel和qmovie大小
    size=setWin->getSize();
    body->resize(size,size);
    bodySize=body->size();
    normalGif->setScaledSize(bodySize);
    body->setScaledContents(true);
    body->move(this->frameGeometry().width()/2 - size/2,
                this->frameGeometry().height()/2 - size/2);
    //常态gif图变化
    if(gifChange<600){
        gifChange++;
    }
    else{
        gifChange=0;
        body->setMovie(questionGif);
        gifSet(body,questionGif);
    }

    reInitBtn();
    saveData();
}



