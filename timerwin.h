#ifndef TIMERWIN_H
#define TIMERWIN_H

#include <QWidget>
#include <QDialog>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtGui>
#include <QLCDNumber>
#include <QTime>
#include <QTimer>



class TimerWin : public QWidget
{
    Q_OBJECT
public:
    explicit TimerWin(QWidget *parent = nullptr);
    inline int timerSwitcher(){return gifSwitch;}
    void initWin();

signals:

public slots:
    void onTimeout();
    void resetTime();
    void start();
    void pause();
    void reset();
    //设置时间按钮槽
    void clickHourUpButton();
    void clickHourDownButton();
    void clickMinuteUpButton();
    void clickMinuteDownButton();
    void clickSecondUpButton();
    void clickSecondDownButton();

private:
    int minute=0,second=0,hour=0;
    int gifSwitch=0;
    int btnSize;
    bool timerOn=false;

    QLCDNumber *lcd;
    QTimer *timer;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout1;
    QHBoxLayout *hLayout2;
    QHBoxLayout *hLayout3;
    //开启与重置按钮
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *resetButton;
    //设置时间按钮
    QPushButton *hourUpButton;
    QPushButton *hourDownButton;
    QPushButton *minuteUpButton;
    QPushButton *minuteDownButton;
    QPushButton *secondUpButton;
    QPushButton *secondDownButton;

};

#endif // TIMERWIN_H
