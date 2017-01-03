#include "rollitwidget.h"

#include <QtGui>
#include <QDebug>
#include <QStringList>
#include "QMessageBox"

#include "rollitwidget.h"

RollitWidget::RollitWidget(QWidget *parent)
    : QWidget(parent)
{





    setAcceptDrops(true);
    setMinimumSize(640, 640);
    setMaximumSize(640, 640);
    gameBeg=0;
    playbynetwork=false;

    pla = new Square(0);
    com = new Square(0);
    com_num = 0;
    pla_num = 0;

    oldx = -1;
    oldy = -1;

    over = 0;
    game_over = false;


    lastx=0;
    lasty=0;

    nb_pions[0] = nb_pions[1] = 2;

     deltax[0] = 1;
     deltax[1] =-1;
     deltax[2] = 0;
     deltax[3] = 0;
     deltax[4] = 1;
     deltax[5] = 1;
     deltax[6] = -1;
     deltax[7] =-1;

     deltay[0] = 0;
     deltay[1] = 0;
     deltay[2] = 1;
     deltay[3] = -1;
     deltay[4] = 1;
     deltay[5] = -1;
     deltay[6] = 1;
     deltay[7] =-1;

    ria=new RollitIA();

    possible_pos=false;

    newGame();

}

void RollitWidget::setPlayByNetwork(bool bo,QString iptxt,quint16 p)
{
qDebug()  << "playbynetwork" << bo;
playbynetwork=bo;

socket = new QTcpSocket(this);

QHostAddress ad(iptxt);


socket->connectToHost(ad,p,QIODevice::ReadWrite);
connect(socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));
connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

tailleMessage = 0;



return;


}

void RollitWidget::newGame() {



        com->setChess(3, 4);
        com->setChess(4, 3);
        pla->setChess(3, 3);
        pla->setChess(4, 4);
        com_num = pla_num = 2;
        com_step_x = -1;
        com_step_y = -1;
        pla_step_x = -1;
        pla_step_y = -1;
        oldx = -1;
        oldy = -1;

        for (int i=0;i<8;i++) {

            for (int j=0;j<8;j++) {
            plateau[i][j]=2;
            }
        }

        plateau[3][4] = plateau[4][3] = BLANC;
        plateau[3][3] = plateau[4][4] = NOIR;
        nb_pions[BLANC] = nb_pions[NOIR] = 2;

        game_over = false;
}

void RollitWidget::clear()
{
    pieceLocations.clear();
    piecePixmaps.clear();
    pieceRects.clear();
    highlightedRect = QRect();
    inPlace = 0;
    update();
}

void RollitWidget::clear2()
{


    gameBeg=0;
    playbynetwork=false;

    pla = new Square(0);
    com = new Square(0);
    com_num = 0;
    pla_num = 0;

    oldx = -1;
    oldy = -1;

    over = 0;
    game_over = false;


    lastx=0;
    lasty=0;

    nb_pions[0] = nb_pions[1] = 2;

    newGame();
    update();

}

void RollitWidget::dragEnterEvent(QDragEnterEvent *event)
{


    if (event->mimeData()->hasFormat("image/x-puzzle-piece"))
        event->accept();
    else
        event->ignore();
}

void RollitWidget::dragLeaveEvent(QDragLeaveEvent *event)
{


    QRect updateRect = highlightedRect;
    highlightedRect = QRect();
    update(updateRect);
    event->accept();
}

void RollitWidget::dragMoveEvent(QDragMoveEvent *event)
{



    QRect updateRect = highlightedRect.united(targetSquare(event->pos()));

    if (event->mimeData()->hasFormat("image/x-puzzle-piece")
        && findPiece(targetSquare(event->pos())) == -1) {

        highlightedRect = targetSquare(event->pos());
        event->setDropAction(Qt::MoveAction);
        event->accept();
        possible_pos=true;
    } else {
        highlightedRect = targetSquare(event->pos());
        possible_pos=false;
        //highlightedRect = QRect();
        event->ignore();
    }

    QPoint a=event->pos();

    int n=a.x()/80;
    int m=a.y()/80;


    if((getValue(n,m)==NOIR)||(getValue(n,m)==BLANC)) highlightedRect = QRect();


    update(updateRect);
}

void RollitWidget::dropEvent(QDropEvent *event)
{



    if (event->mimeData()->hasFormat("image/x-puzzle-piece")
        && findPiece(targetSquare(event->pos())) == -1) {


        if (gameBeg==0) gameBeg++;
        QByteArray pieceData = event->mimeData()->data("image/x-puzzle-piece");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QRect square = targetSquare(event->pos());
        QRect all(0,0,640,640);

        QPoint cent=square.center();

        int x=cent.x()/80;
        int y=cent.y()/80;



        if(playbynetwork) {






            essayer_coup(x,y,colo,1);  update();

            envoyer_coup(x,y,colo);

            canPlay=false;

        }


        else {



            essayer_coup(x,y,NOIR,1);


        update();



        int xx=x;
        int yy=y;



        approfondissement_iteratif(BLANC, &xx, &yy);
                if (!essayer_coup(xx, yy, BLANC,1)) {


                }

        lastx=xx;
        lasty=yy;

        update();

        toString();

        }




        QPixmap pixmap;
        QPoint location;
        dataStream >> pixmap >> location;

        pieceLocations.append(location);
        piecePixmaps.append(pixmap);
        pieceRects.append(square);

        highlightedRect = QRect();
        update(square);
        update(all);

        event->setDropAction(Qt::MoveAction);
        event->accept();



        if (location == QPoint(square.x()/80, square.y()/80)) {
        //    inPlace++;
        }
    } else {
        update();
        highlightedRect = QRect();
        event->ignore();
    }




}

void RollitWidget::envoyer_coup(int x,int y,int colo) {

       QByteArray paquet;
       QDataStream out(&paquet, QIODevice::WriteOnly);

       // On prépare le paquet à envoyer

       QString messageAEnvoyer =  QString("PUT|%1|%2|%3|").arg(x).arg(y).arg(colo);



       out << (quint16) 0;
       out << messageAEnvoyer;
       out.device()->seek(0);
       out << (quint16) (paquet.size() - sizeof(quint16));

       if (socket->isWritable()) socket->write(paquet); // On envoie le paquet

       //qDebug() << messageAEnvoyer;


}

void RollitWidget::pass()
{


    int xx=lastx;
    int yy=lasty;

    approfondissement_iteratif(BLANC, &xx, &yy);
            if (!essayer_coup(xx, yy, BLANC,1)) {
                QMessageBox msgBox;

                QString text="The player pass his turn and the computer pass also his turn\n";

                text=text.toUtf8();

                msgBox.setText(text);
                msgBox.exec();
            }

    update();

    lastx=xx;
    lasty=yy;


}

int RollitWidget::findPiece(const QRect &pieceRect)
{

    QPoint x1=pieceRect.topLeft();

    if(playbynetwork) {

        if (canPlay) {

        int nb=essayer_coup(x1.x()/80,x1.y()/80,colo,0);





        if (0<nb) return -1;
            else return 1;

     } else return 1;

    } else {
    int nb=essayer_coup(x1.x()/80,x1.y()/80,NOIR,0);

    if (0<nb) return -1;
        else return 1;

}


}



void RollitWidget::clearPlateau()
{

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {

            plateau[j][i]=2;

        }
    }

    update();
}


void RollitWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0,0,QPixmap(":/images/checker.png"));

    if (highlightedRect.isValid()) {

        if(possible_pos) painter.setBrush(QColor("#27D2C2"));

        else painter.setBrush(QColor("#DD0000"));

           painter.setPen(Qt::NoPen);
           //painter.drawRect(highlightedRect.adjusted(0, 0, -1, -1));
           painter.drawRect(highlightedRect);
       }



    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {

            if(getValue(j,i)==NOIR) painter.drawPixmap(0+j*80,0+i*80,QPixmap(":/images/pion_noir.png"));
            if(getValue(j,i)==BLANC) painter.drawPixmap(0+j*80,0+i*80,QPixmap(":/images/pion_blanc.png"));


        }
    }

    emit up();


    painter.end();
}

const QRect RollitWidget::targetSquare(const QPoint &position)
{
    return QRect(position.x()/80 * 80, position.y()/80 * 80, 80, 80);
}

 bool RollitWidget::putChess(int x, int y) {




     Square ccom(com->getData());
     Square ppla(pla->getData());

                int x1, y1;
                //int change_num = 0;
                bool sign = false;
                int step;

                if (x < 0 || x > 7) {
                        return false;
                }
                if (y < 0 || y > 7) {
                        return false;
                }

                if (ppla.getChess(x, y) || ccom.getChess(x, y)) {
                        return false;
                }

                Square ret(ria->neighbor[x][y]->getData());


                if(ret.bitAnd(ccom).isEmpty())
                {
                                        return false;
                                }




                for (int inx = -1; inx < 2; inx++) {
                        for (int iny = -1; iny < 2; iny++) {

                                if (inx == 0 && iny == 0) {
                                        continue;
                                }

                                x1 = x + inx;
                                y1 = iny + y;
                                step = 0;

                                while (ccom.getChess(x1, y1)) {
                                        x1 += inx;
                                        y1 += iny;
                                        step++;
                                }

                                if (step <= 0 || !ppla.getChess(x1, y1)) {
                                        continue;
                                }

                                while (step > 0) {
                                        sign = true;
                                        x1 -= inx;
                                        y1 -= iny;
                                        ccom.clearChess(x1, y1);
                                        com->clearChess(x1, y1);
                                        com_num--;
                                        ppla.setChess(x1, y1);
                                        pla->setChess(x1, y1);
                                        pla_num++;
                                        step--;
                                }
                        }
                }


                if (sign) {
                        ppla.setChess(x, y);
                        pla_num++;
                        oldx = oldy = -1;
                        pla->setChess(x,y);




                        update();



                } else {
                        return false;
                }



                ria->begin(ccom, ppla, com_num, pla_num);

                x = ria->Sx;
                y = ria->Sy;



                ret.setSquare(ria->neighbor[x][y]->getData());
                sign = false;


                if (ret.bitAnd(ppla.getData()).isEmpty()) {

                        return true;
                }



                for (int inx = -1; inx < 2; inx++) {
                        for (int iny = -1; iny < 2; iny++) {

                                if (inx == 0 && iny == 0) {
                                        continue;
                                }

                                x1 = x + inx;
                                y1 = iny + y;

                                step = 0;

                                while (ppla.getChess(x1, y1)) {

                                        x1 += inx;
                                        y1 += iny;
                                        step++;
                                }

                                if (step <= 0 || (!ccom.getChess(x1, y1))) {
                                        continue;
                                }

                                while (step > 0) {
                                        sign = true;
                                        x1 -= inx;
                                        y1 -= iny;
                                        ppla.clearChess(x1, y1);
                                        pla->clearChess(x1,y1);
                                        pla_num--;
                                        ccom.setChess(x1,y1);
                                        com->setChess(x1,y1);
                                        com_num++;
                                        step--;

                                }
                        }
                }

                if (sign) {
                        ccom.setChess(x, y);
                        com_num++;
                        com_step_x = x;
                        com_step_y = y;

                        QTimer::singleShot(200, this, SLOT(upd()));





                }

                com->setSquare(ccom.getData());
                pla->setSquare(ppla.getData());


                return true;
        }

 void RollitWidget::upd(void){

     update();

     return;

 }


 bool RollitWidget::testPutChess(int x, int y) {
                int x1, y1;
                int step;

                if (x > 7 || x < 0) {
                        return false;
                }
                if (y > 7 || y < 0) {
                        return false;
                }

                if (pla->getChess(x, y) || com->getChess(x, y)) {
                        return false;
                }





                Square ret(ria->neighbor[x][y]->getData());


                for (int inx = -1; inx < 2; inx++) {
                        for (int iny = -1; iny < 2; iny++) {
                                if (inx == 0 && iny == 0) {
                                        continue;
                                }

                                x1 = x + inx;
                                y1 = iny + y;
                                step = 0;

                                while (com->getChess(x1, y1)) {
                                        x1 += inx;
                                        y1 += iny;
                                        step++;
                                }

                                if (step <= 0 || !pla->getChess(x1, y1)) {
                                        continue;
                                }
                                return true;
                        }
                }

                return false;

        }

 int RollitWidget::retourner_ligne(int x, int y, int d, int joueur,int write)
 {
   int x2, y2;
   int nb_retournes = 0;

   /* determniner le nb de pions a retourner */
   x2 = x+deltax[d];
   y2 = y+deltay[d];
   while (1) {
     if (!dans_plateau(x2, y2) || plateau[x2][y2] == VIDE)
       return 0;
     if (plateau[x2][y2] == joueur)
       break;
     nb_retournes++;
     x2 += deltax[d];
     y2 += deltay[d];
   }

   /* recommencer en retournant les pions */
   if ((nb_retournes > 0)&&(write)) {
     x2 = x+deltax[d];
     y2 = y+deltay[d];
     while (plateau[x2][y2] != joueur) {
       plateau[x2][y2] = joueur;
       x2 += deltax[d];
       y2 += deltay[d];
     }
   }
   return nb_retournes;
 }

 int RollitWidget::dans_plateau(int x, int y)
 {
   return (x >= 0 && x < 8 && y >= 0 && y < 8);
 }

 int RollitWidget::essayer_coup(int x, int y, int joueur,int write)
 {
   int d;
   int nb_retournes = 0;

   if (!dans_plateau(x, y) || plateau[x][y] != VIDE)
     return 0;

   for (d = 0; d < 8; d++)
     nb_retournes += retourner_ligne(x, y, d, joueur,write);

   if (nb_retournes == 0)
     return 0;
   if (write) {
   plateau[x][y] = joueur;
   nb_pions[joueur] += 1 + nb_retournes;
   nb_pions[AUTRE_JOUEUR(joueur)] -= nb_retournes;
   }

   return nb_retournes;
 }

 int RollitWidget::evaluation(int joueur)
 {
   return nb_pions[joueur] - nb_pions[AUTRE_JOUEUR(joueur)];
 }


 int RollitWidget::negamax(int joueur, int *mx, int *my, int prof)
 {
   int x, y;
   int eval, e;
   int mx2, my2;
   int plateau2[8][8];		/* pour sauvegarder la position
                                    initiale */
   int nb_pions2[2];

   compteur_noeuds++;

   if (nb_pions[NOIR] + nb_pions[BLANC] == 64) /* fin de la partie */
     return GAGNE*(nb_pions[joueur] - nb_pions[AUTRE_JOUEUR(joueur)]);

   if (prof <= 0)
     return evaluation(joueur);

   /* on cherche tous les coups pour joueur et on renvoit le max des opposes
      des evaluations aux noeuds fils */
   memcpy(plateau2, plateau, sizeof(plateau));
   memcpy(nb_pions2, nb_pions, sizeof(nb_pions));
   eval = -INFINI;
   for (x = 0; x < 8; x++)
     for (y = 0; y < 8; y++)
       if (essayer_coup(x, y, joueur,1)) {
         e = -negamax(AUTRE_JOUEUR(joueur), &mx2, &my2, prof-1);
         if (e > eval) {
           *mx = x; *my = y;
           eval = e;
         }
         memcpy(plateau, plateau2, sizeof(plateau));



         memcpy(nb_pions, nb_pions2, sizeof(nb_pions));
       }
   return eval;
 }

 int RollitWidget::approfondissement_iteratif(int joueur, int *x, int *y)
 {
   int prof, eval;

   //tableau IA
   // printf("prof\tnoeuds\teval\tcoup\n");
   for (prof = 1; ; prof++) {
     compteur_noeuds = 0;
     eval = negamax(joueur, x, y, prof);
     // printf("%d\t%d\t%d\t%c%d\n", prof, compteur_noeuds, eval, 'a'+*x, *y+1);
     if (abs(eval) >= GAGNE || compteur_noeuds > NOEUDS_MAX)
       return eval;
   }
 }

 void RollitWidget::toString()
 {
     QString str="\n";
     for (int i = 0; i < 8; i++) {
             for (int j = 0; j < 8; j++) {
                     if (plateau[j][i]==0) {
                             str += "|*";
                     }

                     if (plateau[j][i]==1) {
                            str += "|X";
                        }

                     if (plateau[j][i]==2) {

                             str += "| ";
                     }
             }
             str += "|\n";
     }
     str += "\n";




     str += "NOIR :"+ QString("%1").arg(nb_pions[NOIR])+" // BLANC :"+ QString("%1").arg(nb_pions[BLANC])+"\n";


 }

 int RollitWidget::getValue(int x, int y)
 {

     return plateau[x][y];

 }

 void RollitWidget::donneesRecues()
 {
     /* Même principe que lorsque le serveur reçoit un paquet :
     On essaie de récupérer la taille du message
     Une fois qu'on l'a, on attend d'avoir reçu le message entier (en se basant sur la taille annoncée tailleMessage)
     */
     QDataStream in(socket);

     if (tailleMessage == 0)
     {
         if (socket->bytesAvailable() < (int)sizeof(quint16))
              return;

         in >> tailleMessage;
     }

     if (socket->bytesAvailable() < tailleMessage)
         return;


     // Si on arrive jusqu'à cette ligne, on peut récupérer le message entier
     QString messageRecu;
     in >> messageRecu;

    // // qDebug() << messageRecu;
     // On affiche le message sur la zone de Chat
     //listeMessages->append(messageRecu);

     QStringList strl=messageRecu.split("|");
     int xx=strl.at(1).toInt();
     int yy=strl.at(2).toInt();
     int colo=strl.at(3).toInt();

    essayer_coup(xx,yy,colo,1);

    if (colo==waitColo) canPlay=true;
    //lbl5->setVisible(false);


     //plateau[xx][yy] = colo;
     update();




     // On remet la taille du message à 0 pour pouvoir recevoir de futurs messages
     tailleMessage = 0;



 }

 // Ce slot est appelé lorsque la connexion au serveur a réussi
 void RollitWidget::connecte()
 {

     if (colo==0) { waitColo=1; canPlay=false; }
     if (colo==1) { waitColo=0; canPlay=true; }

     QMessageBox msgBox;
     msgBox.setText("Le joueur blanc commence.");
     msgBox.exec();



 }

 // Ce slot est appelé lorsqu'on est déconnecté du serveur
 void RollitWidget::deconnecte()
 {

 }

 // Ce slot est appelé lorsqu'il y a une erreur
 void RollitWidget::erreurSocket(QAbstractSocket::SocketError erreur)
 {
     switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
     {
         case QAbstractSocket::HostNotFoundError:
            // listeMessages->append(tr("<em>ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.</em>"));
             break;
         case QAbstractSocket::ConnectionRefusedError:
            // listeMessages->append(tr("<em>ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.</em>"));
             break;
         case QAbstractSocket::RemoteHostClosedError:
            // listeMessages->append(tr("<em>ERREUR : le serveur a coupé la connexion.</em>"));
             break;
         default:
            // listeMessages->append(tr("<em>ERREUR : ") + socket->errorString() + tr("</em>"));
             break;
     }


 }
