#ifndef SQUARE_H
#define SQUARE_H

#include <QString>

class Square
{

public:

    int board[8][8];

    int deltay[8];
    int deltax[8];


    Square();

    Square(int i);

    Square(const Square &q);

    Square getData();

    void setSquare(const Square &q);

    void setChess(int x, int y);

    void clearChess(int x, int y);
    bool getChess(int x, int y);


    bool equals(const Square &q);

    Square bitOr(const  Square &q);
    Square bitAnd(const Square &q);

    bool isEmpty();
    QString toString();

};

#endif // SQUARE_H
