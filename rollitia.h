#ifndef ROLLITIA_H
#define ROLLITIA_H

#include <QObject>
#include <board.h>
#include <square.h>

#define INFINI 1000000		/* evaluation plus grande que toutes
 //                                 les evaluations possibles */
#define GAGNE 10000		/* evaluation pour une partie gagnee */


class RollitIA : public QObject
{

//board b;

public:
    RollitIA();
    void generate_moves();

    int score[8][8];
    Square  * axis[8][8];
    int minmax[20];
    int depth;
    int effect;
    bool exhaustive;
    int Sx;
    int Sy;
    Square * neighbor[][8];

    void setScore();
    void setNeighbor();
    void setAxis();
    bool begin(Square &computer, Square &player, int com_num, int pla_num);
    int getScore(Square &q);

    bool next(int n, Square &myself, Square &opponent, int my_num, int opp_num, int my_score, int opp_score, bool already);
    int Evaluate(Square &com, Square &pla, int com_num, int pla_num, int com_score, int pla_score);




};

#endif // ROLLITIA_H
