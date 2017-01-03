#include "rollitia.h"
#include "board.h"
#include "math.h"

RollitIA::RollitIA()
{


for (int i=0;i<8;i++)
    {

    for (int j=0;j<8;j++)
        {

    axis[i][j] = new Square(0);
    neighbor[i][j] = new Square(0);

}
}

                depth = 4;
                setScore();
                setNeighbor();
                setAxis();
}



bool RollitIA::begin(Square &computer, Square &player, int com_num, int pla_num) {
                int x1, y1;
                int iny, inx;
                int x, y;
                int step = 0, changenum = 0;
                int chess_num = 0;
                bool chess = false;
                bool sign = false;
                bool b = false;
                bool count = false;


                // Vecteur vv(*v); // construction par recopie



                Square com(computer);
                Square pla(player);
                Square ret;

                int com_score = getScore(computer);
                int pla_score = getScore(player);

                int b_com_score, b_pla_score;

                minmax[0] = 30000;

                chess_num = com_num + pla_num;

                if (chess_num == 4) {
                        int i = ((int) (qrand() * 999)) % 4;
                        switch (i) {
                        case 0:
                                Sx = 3;
                                Sy = 2;
                                break;
                        case 1:
                                Sx = 2;
                                Sy = 3;
                                break;
                        case 2:
                                Sx = 5;
                                Sy = 4;
                                break;
                        case 3:
                                Sx = 4;
                                Sy = 5;
                                break;
                        }
                        return true;
                }

                if (chess_num >= 52) {
                        depth = 64 - chess_num;
                } else if (chess_num >= 48) {
                        depth = 6;
                } else {
                        depth = 4;

                }
                exhaustive = (chess_num + depth >= 64);

                effect = 100 * (chess_num + depth - 4) / 60;

                for (y = 0; y < 8; y++) {
                        for (x = 0; x < 8; x++) {
                                if (com.getChess(x, y) || pla.getChess(x, y)) {
                                        continue;
                                }

                                ret.setSquare(neighbor[x][y]->getData());


                                /* Square q=ret->bitAnd(pla);

                                if (q.isEmpty()) {
                                        continue;
                                }
                                */

                                b_com_score = com_score;
                                b_pla_score = pla_score;
                                for (iny = 1; iny > -2; iny--) {
                                        for (inx = 1; inx > -2; inx--) {

                                                if (inx == 0 && iny == 0) {
                                                        continue;
                                                }
                                                x1 = x;
                                                y1 = y;
                                                do {
                                                        x1 += inx;
                                                        y1 += iny;
                                                        chess = pla.getChess(x1, y1);
                                                        if (chess) {
                                                                step++;
                                                        }
                                                } while (chess);

                                                if (step > 0 && com.getChess(x1, y1)) {
                                                        count = true;
                                                        changenum += step;
                                                        while (step > 0) {
                                                                y1 -= iny;
                                                                x1 -= inx;
                                                                b_com_score += score[x1][y1];
                                                                b_pla_score -= score[x1][y1];
                                                                com.setChess(x1, y1);
                                                                pla.clearChess(x1, y1);
                                                                step--;
                                                        }
                                                } else {
                                                        step = 0;
                                                }
                                        }
                                }

                                if (!count) {
                                        continue;
                                }

                                com.setChess(x, y);
                                b_com_score += score[x][y];
                                if (depth > 1) {
                                        b = next(1, pla, com, pla_num - changenum, com_num + changenum + 1, b_pla_score, b_com_score, false);
                                } else {
                                        b = true;

                                }
                                if (b) {
                                        Sx = x;
                                        Sy = y;
                                        sign = true;
                                }

                                changenum = 0;
                                count = false;

                                com.setSquare(computer);
                                pla.setSquare(player);
                        }
                }

                return sign;
        }


void RollitIA::setScore() {
                for (int y = 0; y < 8; y++) {
                        for (int x = 0; x < 8; x++) {
                                if (y == 1 || y == 6) {
                                        score[x][y] = -6;
                                } else {
                                        score[x][y] = 0;

                                }
                                if (x == 1 || x == 6) {
                                        score[x][y] -= 6;
                                }
                        }
                }

                score[0][0] = 60;
                score[0][7] = 60;
                score[7][0] = 60;
                score[7][7] = 60;

        }

void RollitIA::setNeighbor() {
                for (int y = 0; y < 8; y++) {
                        for (int x = 0; x < 8; x++) {
                                neighbor[x][y] = new Square();
                                for (int inx = -1; inx <= 1; inx++) {
                                        for (int iny = -1; iny <= 1; iny++) {
                                                if (inx != 0 || iny != 0) {
                                                        if (x + inx >= 0 && x + inx < 8 && y + iny >= 0
                                                                        && y + iny < 8) {
                                                                neighbor[x][y]->setChess(x + inx, y + iny);
                                                        }
                                                }
                                        }
                                }
                        }
                }

        }

int RollitIA::getScore(Square &q) {
                int chess_score = 0;

                for (int y = 0; y < 8; y++) {
                        for (int x = 0; x < 8; x++) {
                                if (q.getChess(x, y)) {
                                        chess_score += score[x][y];
                                }
                        }
                }
                return chess_score;
        }

void RollitIA::setAxis() {
                for (int y = 0; y < 8; y++) {
                        for (int x = 0; x < 8; x++) {
                                axis[x][y] = new Square();
                                for (int m = 0; m < 8; m++) {
                                        for (int n = 0; n < 8; n++) {
                                                if ((m == y) || (n == x) || ((m + n) == (y + x))
                                                                || ((m - n) == (y - x))) {
                                                        axis[x][y]->setChess(n, m);
                                                }
                                        }
                                }
                        }
                }
        }

bool RollitIA::next(int n, Square &myself, Square &opponent, int my_num, int opp_num, int my_score, int opp_score, bool already) {
                int x1, y1;
                int iny, inx;
                int x, y;
                int step = 0, changenum = 0;
                bool chess = false;
                bool sign = false;
                bool b = false;
                bool count = false;
                int b_my_score;
                int b_opp_score;
                Square my(myself);
                Square opp(opponent);
                Square ret;

                minmax[n] = 2147483647;
                //30000;
                //my(myself);

                for (y = 0; y < 8; y++) {
                        for (x = 0; x < 8; x++) {

                                if (my.getChess(x, y) || opp.getChess(x, y)) {
                                        continue;
                                }


                                 Square s(*neighbor[x][y]);


                                ret.setSquare(s);


                                ret.bitAnd(opp);

                                if (ret.isEmpty()) {
                                        continue;
                                }

                                b_my_score = my_score;
                                b_opp_score = opp_score;

                                for (iny = 1; iny > -2; iny--) {
                                        for (inx = 1; inx > -2; inx--) {

                                                if ((inx == 0) && (iny == 0)) {
                                                        continue;
                                                }
                                                x1 = x;
                                                y1 = y;
                                                do {
                                                        x1 += inx;
                                                        y1 += iny;
                                                        chess = opp.getChess(x1, y1);
                                                        if (chess) {
                                                                step++;
                                                        }
                                                } while (chess);

                                                if (step <= 0) {
                                                        continue;
                                                }
                                                if (!my.getChess(x1, y1)) {
                                                        step = 0;
                                                        continue;
                                                }

                                                count = true;
                                                changenum += step;
                                                while (step > 0) {
                                                        y1 -= iny;
                                                        x1 -= inx;
                                                        b_my_score += score[x1][y1];
                                                        b_opp_score -= score[x1][y1];
                                                        my.setChess(x1, y1);
                                                        opp.clearChess(x1, y1);
                                                        step--;
                                                }
                                        }
                                }

                                if (!count) {
                                        continue;
                                }

                                my.setChess(x, y);
                                b_my_score += score[x][y];
                                if (n == depth - 1) {
                                        int val = Evaluate(opp, my, opp_num - changenum, my_num + changenum + 1, b_opp_score, b_my_score);

                                        if (val < minmax[n]) {
                                                minmax[n] = val;
                                                if (minmax[n] <= -minmax[n - 1]) {
                                                        return false;
                                                }
                                        }
                                        sign = true;
                                } else {
                                        b = next(n + 1, opp, my, opp_num - changenum, my_num
                                                        + changenum + 1, b_opp_score, b_my_score, false);
                                        if (b) {
                                                if (minmax[n] <= -minmax[n - 1]) {
                                                        return false;
                                                }
                                                sign = true;
                                        }
                                }

                                changenum = 0;
                                count = false;
                                my.setSquare(myself);
                                opp.setSquare(opponent);
                        }
                }

                if (!sign) {
                        if (already) {
                                int _final;
                                int base = 20000;
                                //int base=2147483647;

                                if (exhaustive) {
                                        base = 0;

                                }
                                if ((n % 2) == 0) {
                                        _final = my_num - opp_num;
                                        if (_final > 0) {
                                                minmax[n] = -(base + _final);
                                        } else if (_final < 0) {
                                                minmax[n] = base - _final;
                                        } else {
                                                minmax[n] = 0;
                                        }
                                } else {
                                        _final = opp_num - my_num;
                                        if (_final > 0) {
                                                minmax[n] = base + _final;
                                        } else if (_final < 0) {
                                                minmax[n] = -base + _final;
                                        } else {
                                                minmax[n] = 0;
                                        }
                                }

                                minmax[n - 1] = -minmax[n];
                                return false;
                        } else {
                                depth++;
                                if (n == depth - 1) {
                                        int val = Evaluate(opp, my, opp_num, my_num, opp_score,my_score);
                                        minmax[n] = val;
                                        depth--;
                                        if (minmax[n] <= -minmax[n - 1]) {
                                                return false;
                                        }
                                } else {
                                        b = next(n + 1, opp, my, opp_num, my_num, opp_score,my_score, true);
                                        depth--;
                                        if (minmax[n] <= -minmax[n - 1]) {
                                                return false;
                                        }

                                }
                        }
                }

                minmax[n - 1] = -minmax[n];
                return true;
        }

int RollitIA::Evaluate(Square &com, Square &pla, int com_num, int pla_num, int com_score, int pla_score) {

                int x, y;
                int pla_axis = 0;
                int com_axis = 0;
                Square ret;
                Square board(com);
                board.bitOr(pla);

                Square aaxis;

                if (exhaustive) {
                        return com_num - pla_num;
                }

                if (com_num + pla_num > 44) {
                        for (x = 0; x < 8; x++) {
                                for (y = 0; y < 8; y++) {

                                        aaxis.setSquare(axis[x][y]->getData());

                                        if (pla.getChess(x, y)) {
                                                ret.setSquare(board);

                                                Square rret(ret);
                                                rret.bitAnd(aaxis);


                                                if (rret.equals(aaxis)) {
                                                        pla_axis++;
                                                }
                                        } else if (com.getChess(x, y)) {
                                                ret.setSquare(board);

                                                Square rret(ret);
                                                rret.bitAnd(aaxis);

                                                if (rret.equals(aaxis)) {
                                                        com_axis++;
                                                }
                                        }
                                }
                        }
                }

                return effect * (com_num - pla_num) + (100 - effect)
                                * ((com_score - pla_score) + (com_axis - pla_axis) * 4);
        }

