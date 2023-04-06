#ifndef MIZUKI_H
#define MIZUKI_H

#include <QMainWindow>
#include<QMouseEvent>
#include<QtGlobal>
#include <QLabel>
#include <QMovie>
#include<QPoint>
#include<QTimer>
#include<QPushButton>
#include<QFile>
#include<QDataStream>
#include<QSystemTrayIcon>
#include <QSettings>
#include <QFileInfo>
#include <QDir>

#include "setwin.h"
#include "musicwin.h"
#include "timerwin.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Mizuki; }
QT_END_NAMESPACE



class Mizuki : public QMainWindow
{
    Q_OBJECT

public:
    Mizuki(QWidget *parent = nullptr);
    ~Mizuki();

    //拖拽事件
    void mousePressEvent(QMouseEvent *event);//鼠标左击和右击事件，控制窗口拖动与显示按钮
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件，同时拖动所有窗口按钮
    void mouseReleaseEvent(QMouseEvent *event);//鼠标松开事件,gif图回归常态

    //动画相关
    void gifSet(QLabel * body,QMovie *gif);//初始化gif图
    void gifSet();//与timer连接，刷新动画状态

    //按钮与窗口相关
    void initBtn();//初始化按钮属性
    void reInitBtn();//调整按钮属性

    //按钮点击事件
    void setBtnPush();//设置按钮
    void minBtnPush();//最小化按钮
    void moreBtnPush();//更多按钮
    void closeBtnPush();//关闭按钮
    void musicBtnPush();//音乐按钮
    void timerBtnPush();//定时器按钮

    //按钮显示控制器
    void btnSwitcher();//控制按钮显示
    void moreBtnSwitcher();//控制更中按钮显示

    //窗口显示控制器
    void winSwitcher();//控制窗口显示
    void moreWinSwitcher();//控制更多窗口显示

    //系统托盘
    void initSystemTray();//初始化托盘属性
    void systemTrayPush();//托盘按钮

    //存储读取体型坐标数据
    void initBody();//读取数据
    void saveData();//存储数据
private:
    Ui::Mizuki *ui;

    //绘图,动画效果相关
    int size;//体型大小
    int btnSize;//按钮大小
    int gifChange;//常态gif图变化
    QSize bodySize;//体型大小
    QLabel *body;
    QMovie *normalGif,*timerGif,*moveGif,*questionGif,*musicGif;
    QTimer *timer;//与gifSet槽函数连接，刷新动画状态

    //按钮与控制器
    int btnSwitch;
    int moreBtnSwitch;
    int winSwitch;
    int moreWinSwitch;
    int foldSwitch;
    int gifSwitch,musicGifSwitch,timerGifSwitch;//用于表示是否处于固定gif状态
    QPoint moveLeftTop;//坐标
    QPushButton *setBtn;//设置
    QPushButton *unfoldBtn;//更多-展开
    QPushButton *foldBtn;//更多-折叠
    QPushButton *minBtn;//最小化
    QPushButton *closeBtn;//关闭
    QPushButton *musicBtn;//音乐
    QPushButton *timerBtn;//计时器

    //窗口
    SetWin *setWin;
    MusicWin *musicWin;
    TimerWin *timerWin;

    //系统托盘图标
    QSystemTrayIcon* pSystemTray;

};
#endif // MIZUKI_H
