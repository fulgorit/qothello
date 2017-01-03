#ifndef BOARD_H
#define BOARD_H

#include <QString>
#include <rollitia.h>

class board
{
public:
int board_tab[8][8];
int board_tab_free[8][8];



public:
    board();
    void processDiag(int x,int y);
    void ProcessHV(int x,int y);
    int getValue(QString str);
    int getValue(int x,int y);
    void invert(QString str);
    QString place(int x,int y);
    void setFreePos(QString str);
    void playIA(void);
   // QString parse(QString combinaison);



};

#endif // BOARD_H
