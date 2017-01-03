#include <QtGui>
#include "mainwindow.h"
#include <QDateTime>
#include <QThread>
#include <QSplashScreen>

 
class I : public QThread
{
public:
        static void sleep(unsigned long secs) {
                QThread::sleep(secs);
        }
};
 
int main(int argc, char *argv[])
{
 
    Q_INIT_RESOURCE(qrollit);

    QApplication app(argc, argv);
    QPixmap pixmap(":/images/ot.png");
    QSplashScreen splash(pixmap);

    splash.show();
    MainWindow mainWin;
    
    I::sleep(5);               // splash is shown for 5 seconds
    mainWin.show();

    splash.finish(&mainWin);
    return app.exec();
}

