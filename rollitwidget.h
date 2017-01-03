#ifndef ROLLITWIDGET_H
#define ROLLITWIDGET_H

#include <QList>
#include <QPoint>
#include <QPixmap>
#include <QWidget>
#include "board.h"
#include <QTcpSocket>
#include <QHostAddress>

#define NOIR 0
#define BLANC 1
#define VIDE 2
#define AUTRE_JOUEUR(j) (1-(j))

#define INFINI 1000000

#define GAGNE 10000
#define NOEUDS_MAX 1000


QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
QT_END_NAMESPACE

class RollitWidget : public QWidget
{
    Q_OBJECT

public:
    RollitWidget(QWidget *parent = 0);
    void clear();
    void clear2();

    void newGame();

signals:
    void up();
    void passEnable();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    int findPiece(const QRect &pieceRect) ;
    const QRect targetSquare(const QPoint &position) ;
    bool putChess(int x, int y);
    bool testPutChess(int x, int y);



    QList<QPixmap> piecePixmaps;
    QList<QRect> pieceRects;
    QList<QPoint> pieceLocations;
    QRect highlightedRect;
public:

    int valid;

    void pass();

    int nb_pions[2];

    int inPlace;
    int gameBeg;

    //int board[8][8];
    RollitIA *ria;

    Square *pla;
    Square *com;

    int com_num;
    int pla_num;
    int com_step_x;
    int com_step_y;
    int pla_step_x;
    int pla_step_y;
    int oldx;
    int oldy;

    int over;

    bool game_over;

    int retourner_ligne(int x, int y, int d, int joueur,int write);

    int dans_plateau(int x, int y);

    int deltax[8];
    int deltay[8];

    int plateau[8][8];

    int compteur_noeuds;

    int essayer_coup(int x, int y, int joueur,int write);
    int evaluation(int joueur);
    int negamax(int joueur, int *mx, int *my, int prof);

    int approfondissement_iteratif(int joueur, int *x, int *y);

    void toString();
    int getValue(int x,int y);

    void clearPlateau();

    void upd();

    int lastx;
    int lasty;

    void setPlayByNetwork(bool bo,QString ip,quint16 p);
    bool playbynetwork;
    int waitColo;
    bool canPlay;
    int colo;

    void envoyer_coup(int x,int y,int colo);

private:
       QTcpSocket *socket; // Représente le serveur
       quint16 tailleMessage;

       bool possible_pos;

private slots:
           void donneesRecues();
           void connecte();
           void deconnecte();
           void erreurSocket(QAbstractSocket::SocketError erreur);

};


#endif // ROLLITWIDGET_H

