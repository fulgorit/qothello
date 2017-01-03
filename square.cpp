#include "square.h"
#include <string.h>
#include <QString>
#define MAX 8

Square::Square()
{
    for (int x=0;x<MAX;x++)
    {
        for (int y=0;y<MAX;y++)
        {
           board[x][y]=0;
        }
    }



}

Square::Square(int i)
{
    for (int x=0;x<MAX;x++)
    {
        for (int y=0;y<MAX;y++)
        {
           board[x][y]=i;
        }
    }

}

Square Square::getData()
{

Square q(0);
for (int x=0;x<MAX;x++)
{
    for (int y=0;y<MAX;y++)
    {
       q.board[x][y]=board[x][y];
    }
}

return q;


}

Square::Square(const Square &q) {

    for (int x=0;x<MAX;x++)
    {
        for (int y=0;y<MAX;y++)
        {
           board[x][y]=q.board[x][y];
        }
    }


}



void Square::setSquare(const Square &q) {
                //data = q.data;
                for (int x=0;x<MAX;x++)
                {
                    for (int y=0;y<MAX;y++)
                    {
                       board[x][y]=q.board[x][y];
                    }
                }

        }



void Square::setChess(int x, int y) {
                if ((x > 7) || (x < 0) || (y > 7) || (y < 0)) {
                        return;
                }

                //data |= 1l << ((y << 3) + x);


                board[x][y]=1;
        }

void Square::clearChess(int x, int y) {
                if ((x > 7) || (x < 0) || (y > 7) || (y < 0)) {
                        return;
                }
                //data &= ~(1l << ((y << 3) + x));

                board[x][y]=0;
            }



bool Square::getChess(int x, int y) {
                if ((x > 7) || (x < 0) || (y > 7) || (y < 0)) {
                        return false;
                }

                return board[x][y];

        }

bool Square::equals(const Square & q) {
               // return (q.data == data);

for (int x=0;x<8;x++)
    {
    for (int y=0;y<8;y++)
        {

    if(board[x][y]!=q.board[x][y]) return false;
}}

return true;
}

Square Square::bitOr(const Square & q) {

    Square q2(0);


    for (int x=0;x<MAX;x++)
    {
        for (int y=0;y<MAX;y++)
        {
           q2.board[x][y]|=q.board[x][y];
        }
    }

    return q2;
    // data |= q.data;


        }

Square Square::bitAnd(const Square & q) {

//                data &= q.data;

    Square q2(0);


                for (int x=0;x<MAX;x++)
                {
                    for (int y=0;y<MAX;y++)
                    {
                      q2.board[x][y]&=q.board[x][y];
                    }
                }

return q2;

            }


bool Square::isEmpty() {

    for (int x=0;x<MAX;x++)
    {
        for (int y=0;y<MAX;y++)
        {
           if(board[x][y]!=0) return 0;
        }
    }
return 1;
        }

QString Square::toString() {
                QString str="\n";
                for (int y = 0; y < MAX; y++) {
                        for (int x = 0; x < MAX; x++) {
                                if (getChess(x, y)) {
                                        str += "|X";
                                } else {
                                        str += "| ";
                                }
                        }
                        str += "|\n";
                }
                str += "\n";
                return str;
        }


