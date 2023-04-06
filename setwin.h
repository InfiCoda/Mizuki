#ifndef SETWIN_H
#define SETWIN_H

#include <QWidget>
#include <QBitmap>
#include <QPainter>
#include <QLabel>
#include <QSlider>
#include <QPixmap>
#include <QString>
#include <QIcon>
#include<QPainterPath>
#include<QSettings>
#include<QDir>

namespace Ui {
class SetWin;
}



class SetWin : public QWidget
{
    Q_OBJECT

public:
    explicit SetWin(QWidget *parent = nullptr);
    ~SetWin();
    //外部获取size
    inline int getSize(){return MizukiSize;}
    void autoStart();
    //初始化窗口
    void setSize(int);

private slots:
    //滑动槽函数，改变尺寸大小
    void on_horizontalSlider_valueChanged(int value);

private:
     Ui::SetWin *ui;
    int MizukiSize;//控制大小
    bool isAutoStart;//自启动状态
    QLabel* sizeNum;//槽大小显示

};

#endif // SETWIN_H
