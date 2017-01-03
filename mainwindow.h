
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPixmap>
#include <QtWidgets>
#include "rollitia.h"
#include <QtGui>
#include <QHostAddress>

class PiecesList;
class RollitWidget;
QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    //void openImage(const QString &path = QString());
    void openImage();
    void setupPuzzle();
    void passPbutEn();
    void saveGame();
    void loadGame();
    void grab();

private slots:
    void setUp();
    void push();
    //void playnet();
    void setWhite(QString a,quint16 p);
    void setBlack(QString a,quint16 p);
    void playAgain();


private:
    void setupMenus();
    void setupWidgets();



public:

    QPixmap puzzleImage;
    PiecesList *piecesList;
    RollitWidget *rollitWidget;

    QLabel* lbl1;
    QLabel* lbl2;

    QLabel* lbl3;
    QLabel* lbl4;

    QLabel* lbl5;

    QPushButton* pass;


 //      QTcpSocket *socket; // Représente le serveur
 //      quint16 tailleMessage;



};

#endif
