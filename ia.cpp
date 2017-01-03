#include "ia.h"

IA::IA()
{
}


IA::generate_moves() {

    opp = -1 * self.player # opponent player num

    moves = []
    for i in range_size:
        for j in range_size:
            # find an empty square
            if self.board[i][j] != 0:
                continue
            # look in every direction
            for dir in directions:
                t = tuple_in_dir((i,j), dir)
                # till you find an opponent piece
                if (not tuple_valid(t)) or (self.board[t[0]][t[1]] != opp):
                    continue
                # now, skip all the opponent pieces
                while self.board[t[0]][t[1]] == opp:
                    t = tuple_in_dir(t, dir)
                    if not tuple_valid(t):
                        break
                else:
                    # finally if we get one of our own pieces then
                    # make the move
                    if self.get_color(t) == self.player:
                        moves.append((i,j))
                        # no point looking in any other direction
                        break

    # if we don't have a move and the game is not over then
    # return the None move or "no move."
    if not moves and not self.terminal_test():
        moves = [None]

    return moves

}
