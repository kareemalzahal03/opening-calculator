#include <string>
#include <algorithm>
#include <vector>

struct Move_Coords {

    int start_x;
    int start_y;
    int end_x;
    int end_y;

    bool castles_kingside = false;
    bool castles_queenside = false;

    bool operator==(const Move_Coords &n) const {
        return start_x == n.start_x and 
               start_y == n.start_y and
               end_x == n.end_x and
               end_y == n.end_y;
    }
};

struct Position {

    std::string FEN;
    std::string Name;
    bool white_to_move;

    bool operator==(const Position &n) const {
        return FEN == n.FEN and 
        white_to_move == n.white_to_move;
    }
};

class Board {
public:

    struct Position pos;
    char board[8][8]; 

    void Read_Board() {

        std::string new_FEN;
        
        int i = 0;

        while (i < 64) {
            
            int streak = 0;

            while (board[i/8][i%8] == 0) {
                streak++;
                i++;

                if (i%8 == 0)
                    break;
            }
                 
            if (streak) 
                new_FEN += ('0' + streak);
            else {
                new_FEN += board[i/8][i%8];
                i++;
            }
        }
            

        pos.FEN = new_FEN;
    }

    void Read_FEN() {

        int i = 0;

        for (auto c: pos.FEN) {

            if ('1' <= c and c <= '9')

                for (int x = 0; x < c - '0'; ++x) {
                    board[i/8][i%8] = 0;
                    ++i;
                }

            else {
                board[i/8][i%8] = c;
                ++i;
            }
        }
    }

    void Reset_Board() {

        pos.white_to_move = true;
        pos.FEN = "rnbqkbnrpppppppp8888PPPPPPPPRNBQKBNR";
        Read_FEN();
    }

    Board() {

        Reset_Board();
    }

    struct Move_Coords Find_Move_Coords(std::string Move) {

        struct Move_Coords to_return;
            
        if (Move == "O-O" or Move == "O-O-O") {

            to_return.castles_queenside = (Move == "O-O-O");
            to_return.castles_kingside = (Move == "O-O");

            to_return.start_x = 7 * (pos.white_to_move);
            to_return.start_y = 4;
            to_return.end_x = 7 * (pos.white_to_move);
            to_return.end_y = 2 + 4 * (Move == "O-O");

            return to_return;
        }

        Move.erase(remove(Move.begin(), Move.end(), 'x'), Move.end());
        Move.erase(remove(Move.begin(), Move.end(), '+'), Move.end());

        to_return.end_x = 8 - (Move[Move.length()-1] - '0');
        to_return.end_y = (Move[Move.length()-2] - 'a');

        char piece_to_move;

        if ('A' <= Move[0] and Move[0] <= 'Z') {
            piece_to_move = Move[0];
        } else {
            piece_to_move = 'P';
        }
        piece_to_move += (pos.white_to_move==false)*('a'-'A');

        int i = 0;

        while (i < 64) {

            if (board[i/8][i%8] == piece_to_move) {

                if (piece_to_move == 'K' or piece_to_move == 'k' or
                    piece_to_move == 'Q' or piece_to_move == 'q') {
                    break;

                } else if (piece_to_move == 'B' or piece_to_move == 'b') {

                    if ((i/8 + i%8)%2 == (to_return.end_x + to_return.end_y)%2)
                        break;

                } else if (piece_to_move == 'N' or piece_to_move == 'n') {

                    if (Move.length() == 4 and (i%8) == Move[1]-'a') {

                        break;

                    } else {

                    int y_dist = abs(to_return.end_x - i/8);
                    int x_dist = abs(to_return.end_y - i%8);

                    if (y_dist > 0 and x_dist > 0 and y_dist+x_dist == 3)
                        break;

                    }

                } else if (piece_to_move == 'R' or piece_to_move == 'r') {

                    bool legal = true;

                    if (Move.length() == 4 and (i%8) == Move[1]-'a') {

                        break;

                    } else if (to_return.end_x == i/8) {

                        int inc = (to_return.end_y > i%8) ? 1: -1;

                        for (int y = i%8 + inc; y != to_return.end_y; y += inc) {

                            if (board[i/8][y] != 0)
                                legal = false;
                        }

                        if (legal)
                        break;

                    } else {

                        int inc = (to_return.end_x > i/8) ? 1: -1;

                        for (int x = i/8 + inc; x != to_return.end_x; x += inc) {

                            if (board[x][i%8] != 0)
                                legal = false;
                        }

                        if (legal)
                        break;
                    }

                } else if (piece_to_move == 'P' or piece_to_move == 'p') {

                    if (Move.length() == 2 and to_return.end_y == i%8) {

                        break;

                    } else if (Move.length() == 3 and i%8 == Move[0]-'a') {

                        if ((piece_to_move == 'P' and to_return.end_x + 1 == i/8) or
                            (piece_to_move == 'p' and to_return.end_x - 1 == i/8))
                            break;
                    }
                }
            }
            i++;
        }

        to_return.start_x = i/8;
        to_return.start_y = i%8;

        return to_return;
    }

    void Make_Move(struct Move_Coords coords) {

        if (coords.castles_kingside) {

            if (pos.white_to_move) {
                board[7][4]=0;
                board[7][5]='R';
                board[7][6]='K';
                board[7][7]=0;
            } else {
                board[0][4]=0;
                board[0][5]='r';
                board[0][6]='k';
                board[0][7]=0;
            }

        } else if (coords.castles_queenside) {

            if (pos.white_to_move) {
                board[7][0]=0;
                board[7][1]=0;
                board[7][2]='K';
                board[7][3]='R';
                board[7][4]=0;
            } else {
                board[0][0]=0;
                board[0][1]=0;
                board[0][2]='k';
                board[0][3]='r';
                board[0][4]=0;
            }

        } else {

        board[coords.end_x][coords.end_y] = board[coords.start_x][coords.start_y];
        board[coords.start_x][coords.start_y] = 0;
        }

        Read_Board();

        pos.white_to_move = !pos.white_to_move;
        return;
    }

};