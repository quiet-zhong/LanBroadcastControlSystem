#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("://app.ico"));
    Widget w;
    w.resize(480,640);
    w.setWindowTitle("多机同步控制器");
    QDesktopWidget dsk;
    w.move(dsk.width()/2 - w.width()/2,dsk.height()/2 - w.height()/2  );//screen center
    w.show();
    return a.exec();
}
