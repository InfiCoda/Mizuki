#include "setwin.h"
#include "ui_setwin.h"

SetWin::SetWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetWin)
{
    ui->setupUi(this);

    //界面美化
    QBitmap bmp(this->size());//设置圆角边框
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),50,50);
    setMask(bmp);
    setStyleSheet("background-color:white;");
    Qt::WindowFlags m_flags = windowFlags();//保持窗口置顶1
    setWindowFlags(m_flags|Qt::WindowStaysOnTopHint);//保持窗口置顶2
    //设置窗口图标
    this->setWindowIcon(QIcon(":/MizukiPng/03.png"));

    autoStart();
}

SetWin::~SetWin()
{
    delete ui;
}

void SetWin::autoStart()
{
    //查找注册表，确认当前是否自启动
    isAutoStart = false;

    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                       QSettings::NativeFormat);
    QString appPath = QCoreApplication::applicationFilePath();
    QString newPath = QDir::toNativeSeparators(appPath);
    if (settings.contains("Mizuki") && settings.value("Mizuki").toString() == newPath) {
        isAutoStart = true;
    }
    ui->checkBox->setChecked(isAutoStart);
    //连接checkBox与注册表写入函数
    connect(ui->checkBox, &QCheckBox::stateChanged, [newPath](int state) {
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                           QSettings::NativeFormat);
        if (state == Qt::Checked) {
            settings.setValue("Mizuki", newPath);
        } else {
            settings.remove("Mizuki");
        }
    });
}

void SetWin::setSize(int size)
{
    MizukiSize = size;

    QFont ft;
    ft.setPointSize(20);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::blue);
    //显示尺寸数字
    sizeNum = new QLabel(this);
    sizeNum->move(170,130);
    sizeNum->resize(200,40);
    sizeNum->setFont(ft);
    sizeNum->setPalette(pa);
    sizeNum->setNum(MizukiSize);
    sizeNum->show();

    ui->horizontalSlider->setValue(MizukiSize);
}

void SetWin::on_horizontalSlider_valueChanged(int value)
{
    //改变setWin中的size
    MizukiSize=value;
    sizeNum->setNum(MizukiSize);
}



