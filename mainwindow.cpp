#include <QtGui>
#include <stdlib.h>

#include "mainwindow.h"
#include "pieceslist.h"
#include "rollitwidget.h"
//#include "dialognetwork.h"

#include <QMessageBox>

/*
void MainWindow::SeConnecterA()
{


         DialogNetwork * netDial= new DialogNetwork(this);

         connect(netDial, SIGNAL(setBlack(QString,quint16)), this, SLOT(setBlack(QString,quint16)));
         connect(netDial, SIGNAL(setWhite(QString,quint16)), this, SLOT(setWhite(QString,quint16)));



     netDial->show();
     netDial->raise();
     netDial->activateWindow();


}*/
/*
void MainWindow::playnet()
{

    rollitWidget->colo=0;
    SeConnecterA();

    return;
}
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupMenus();
    setupWidgets();

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle(tr("qOthello"));

    piecesList->setFixedWidth(140);
    rollitWidget->setFixedWidth(650);



}

void MainWindow::openImage()
{


    QString fileName = ":/images/pion_noir.png";

    
    

        QPixmap newImage;
        if (!newImage.load(fileName)) {
            QMessageBox::warning(this, tr("Open Image"),
                                  tr("The image file could not be loaded."),
                                  QMessageBox::Cancel);
            return;
        }
        puzzleImage = newImage;
        setupPuzzle();

}



void MainWindow::push()
{

    rollitWidget->pass();


}

void MainWindow::setUp()
{

    QString txt;

    if(rollitWidget->nb_pions[0]+rollitWidget->nb_pions[1]==64)
    {




        txt+=QString("Player :%1\n").arg(rollitWidget->nb_pions[0]);
        txt+=QString("Computer :%1\n").arg(rollitWidget->nb_pions[1]);

        lbl3->setText(QString("%1").arg(rollitWidget->nb_pions[0]));
        lbl4->setText(QString("%1").arg(rollitWidget->nb_pions[1]));

        if (rollitWidget->nb_pions[0]>rollitWidget->nb_pions[1]) txt="Black won the game.";
        if (rollitWidget->nb_pions[0]<rollitWidget->nb_pions[1]) txt="White won the game.";

        QMessageBox msgBox;
        msgBox.setText(txt);


        piecesList->clear();
        lbl5->clear();
        lbl5->setText(txt);

        pass->setEnabled(false);

       // if(ret==QMessageBox::Ok)
        {

    //rollitWidget->nb_pions[0]=0;
    //rollitWidget->nb_pions[1]=0;
    //rollitWidget->clearPlateau();

        }

}
    else  {


        lbl3->setText(QString("%1").arg(rollitWidget->nb_pions[0]));
        lbl4->setText(QString("%1").arg(rollitWidget->nb_pions[1]));


    }


}

void MainWindow::setupPuzzle()
{

    if (piecesList->count()==0) {

    int size = qMin(puzzleImage.width(), puzzleImage.height());
    puzzleImage = puzzleImage.copy((puzzleImage.width() - size)/2,
        (puzzleImage.height() - size)/2, size, size).scaled(80,
            80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

            QPixmap pieceImage = puzzleImage.copy(0, 0, 80, 80);
            piecesList->addPiece(pieceImage, QPoint(0, 0));



    rollitWidget->clear();

    }
}

void MainWindow::setupMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Game"));



    QAction *restartAction = fileMenu->addAction(tr("Start/&Restart"));
    QAction *loadAction = fileMenu->addAction(tr("Load game"));
    QAction *saveAction = fileMenu->addAction(tr("Save game"));
    QAction *takeScreenShot = fileMenu->addAction(tr("Take screenshot"));


    //QAction *connectAction = fileMenu->addAction(tr("&Connect"));

    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcuts(QKeySequence::Quit);


    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveGame()));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadGame()));
    connect(takeScreenShot, SIGNAL(triggered()), this, SLOT(grab()));




    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(restartAction, SIGNAL(triggered()), this, SLOT(playAgain()));

}

void MainWindow::grab()
{

        QString filename = "qothello_"+QDateTime::currentDateTime().toString()+".png";

        QPixmap pixmap(640,640);
        rollitWidget->render(&pixmap, QPoint(), QRegion(0,0,640,640));
        pixmap.save(filename);


}

void MainWindow::saveGame()
{

        QString filename = "qothello_"+QDateTime::currentDateTime().toString()+".sav";

        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);

            for(int i=0;i<8;i++)
              {
                for(int j=0;j<8;j++)
                  {
                        stream << rollitWidget->plateau[i][j];
                  }
              }

            stream << endl;
            file.close();

        }

}

void MainWindow::loadGame()
{

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Savegame"), ".", tr("Savegame (*.sav)"));

    QFile file(fileName);

    playAgain();


    if (file.open(QIODevice::ReadWrite)) {

        /*for Reading line by line from text file*/
        while (!file.atEnd()) {
            QByteArray line = file.readLine();

            int n=0;

            for(int i=0;i<8;i++)
              {
                for(int j=0;j<8;j++)
                  {

                        if(line.at(n)=='0') rollitWidget->nb_pions[0]++;
                        if(line.at(n)=='1') rollitWidget->nb_pions[1]++;

                        if(line.at(n)=='0') rollitWidget->plateau[i][j]=0;
                        if(line.at(n)=='1') rollitWidget->plateau[i][j]=1;
                        if(line.at(n)=='2') rollitWidget->plateau[i][j]=2;

                        n++;



                  }
              }

        }

        rollitWidget->nb_pions[0]--;
        rollitWidget->nb_pions[1]--;
        rollitWidget->nb_pions[0]--;
        rollitWidget->nb_pions[1]--;

        lbl3->setText(QString("%1").arg(rollitWidget->nb_pions[0]));
        lbl4->setText(QString("%1").arg(rollitWidget->nb_pions[1]));

        QRect all(0,0,640,640);
        rollitWidget->update(all);

         QMessageBox msgBox;
         msgBox.setText("the game has been loaded it is your turn");
         msgBox.setInformativeText("you can move the black token");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();

         //QMessageBox("qothello","the game has been loaded it is your turn",QMessageBox::Information);

    }


}


void MainWindow::playAgain()
{


    rollitWidget->clear2();
    pass->setEnabled(true);
    openImage();


}

void MainWindow::setupWidgets()
{
    piecesList = new PiecesList;
    rollitWidget = new RollitWidget;

    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);

    lbl1=new QLabel("Black pawn(s):");
    lbl2=new QLabel("White pawn(s):");
    lbl3=new QLabel("0");
    lbl4=new QLabel("0");
    lbl5=new QLabel("Good luck !");

    pass=new QPushButton("Pass");
    pass->setFixedWidth(140);

    connect(pass,SIGNAL(clicked()),this,SLOT(push()));

    QFrame *frame2 = new QFrame;
    QVBoxLayout *frameLayout2 = new QVBoxLayout(frame2);

    frameLayout2->addWidget(piecesList);

    frameLayout2->addWidget(lbl5);

    lbl5->setVisible(true);

    frameLayout2->addWidget(pass);



    frameLayout2->addWidget(lbl1);

    frameLayout2->addWidget(lbl3);

    frameLayout2->addWidget(lbl2);

    frameLayout2->addWidget(lbl4);

    connect(rollitWidget, SIGNAL(up()),this, SLOT(setUp()));



    connect(rollitWidget, SIGNAL(passEnable()),this, SLOT(passPbutEn()));

    frameLayout->addWidget(frame2);
    frameLayout->addWidget(rollitWidget);
    setCentralWidget(frame);


}

void MainWindow::passPbutEn()
{

    pass->setEnabled(true);

}

void MainWindow::setBlack(QString ip,quint16 p)
{
    rollitWidget->setPlayByNetwork(true,ip,p);
    rollitWidget->colo=0;
    QString fileName = ":/images/pion_noir.png";

        QPixmap newImage;
        if (!newImage.load(fileName)) {
            QMessageBox::warning(this, tr("Open Image"),
                                  tr("The image file could not be loaded."),
                                  QMessageBox::Cancel);
            return;
        }
        puzzleImage = newImage;
        setupPuzzle();



}

void MainWindow::setWhite(QString ip,quint16 p)
{
    rollitWidget->setPlayByNetwork(true,ip,p);
    rollitWidget->colo=1;
    QString fileName = ":/images/pion_blanc.png";

        QPixmap newImage;
        if (!newImage.load(fileName)) {
            QMessageBox::warning(this, tr("Open Image"),
                                  tr("The image file could not be loaded."),
                                  QMessageBox::Cancel);
            return;
        }
        puzzleImage = newImage;
        setupPuzzle();

}

