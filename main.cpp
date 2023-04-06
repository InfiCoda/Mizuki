#include "mizuki.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mizuki w;
    w.show();
    return a.exec();
}
