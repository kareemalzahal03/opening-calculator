#include "board.cpp"

bool add_move(struct Move_Coords c, Board* b, std::vector<struct Move_Coords>& m) {

if (0 <= c.end_x and c.end_x < 8 and 0 <= c.end_y and c.end_y < 8 and
    (b->board[c.end_x][c.end_y] == 0 or 
    ((b->board[c.end_x][c.end_y] <= 'Z') != b->pos.white_to_move))) {

        m.push_back({c.start_x,c.start_y,c.end_x,c.end_y,false,false});
        return true;
    }

    return false;
}

std::vector<struct Move_Coords> pseudo_legal(Board b) {

    std::vector<struct Move_Coords> m;

    int i = 0;

    while (i < 64) {

        char p = b.board[i/8][i%8];

        if (p > 0 and p <= 'Z' == b.pos.white_to_move) {

            struct Move_Coords c = {i/8,i%8,0,0,false,false};

            if (p == 'K' or p == 'k') {

                c.end_x = c.start_x + 1;  c.end_y = c.start_y + 0;
                    add_move(c,&b,m);
                c.end_x = c.start_x + 1;  c.end_y = c.start_y - 1;
                    add_move(c,&b,m);
                c.end_x = c.start_x + 0;  c.end_y = c.start_y - 1;
                    add_move(c,&b,m);
                c.end_x = c.start_x - 1;  c.end_y = c.start_y - 1;
                    add_move(c,&b,m);
                c.end_x = c.start_x - 1;  c.end_y = c.start_y + 0;
                    add_move(c,&b,m);
                c.end_x = c.start_x - 1;  c.end_y = c.start_y + 1;
                    add_move(c,&b,m);
                c.end_x = c.start_x + 0;  c.end_y = c.start_y + 1;
                    add_move(c,&b,m);
                c.end_x = c.start_x + 1;  c.end_y = c.start_y + 1;
                    add_move(c,&b,m);

                if (i%8 == 4 and (i/8 == 0 or i/8 == 7)) {

                    if (b.board[i/8][5]==0 and b.board[i/8][6]==0 and
                    (b.board[i/8][7]=='R' or b.board[i/8][7]=='r')) {

                        m.push_back({i/8,4,i/8,6,true,false});
                    }

                    if (b.board[i/8][3]==0 and b.board[i/8][2]==0 and
                        b.board[i/8][1]==0 and
                    (b.board[i/8][0]=='R' or b.board[i/8][0]=='r')) {

                        m.push_back({i/8,4,i/8,2,false,true});
                    }
                }

            } else if (p == 'Q' or p == 'q') {

                // Diagonal

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x++;  c.end_y++;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x--;  c.end_y++;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x++;  c.end_y--;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x--;  c.end_y--;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                // Sideways

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x++;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x--; 
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_y++;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_y--;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

            } else if (p == 'B' or p == 'b') {

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x++;  c.end_y++;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x--;  c.end_y++;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x++;  c.end_y--;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x--;  c.end_y--;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

            } else if (p == 'N' or p == 'n') {

                add_move({(i/8),(i%8),(i/8)+2,(i%8)+1,false,false},&b,m);
                add_move({(i/8),(i%8),(i/8)+2,(i%8)-1,false,false},&b,m);
                add_move({(i/8),(i%8),(i/8)-2,(i%8)+1,false,false},&b,m);
                add_move({(i/8),(i%8),(i/8)-2,(i%8)-1,false,false},&b,m);
                add_move({(i/8),(i%8),(i/8)+1,(i%8)+2,false,false},&b,m);
                add_move({(i/8),(i%8),(i/8)-1,(i%8)+2,false,false},&b,m);
                add_move({(i/8),(i%8),(i/8)+1,(i%8)-2,false,false},&b,m);
                add_move({(i/8),(i%8),(i/8)-1,(i%8)-2,false,false},&b,m);

            } else if (p == 'R' or p == 'r') {

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x++;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_x--; 
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_y++;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

                c.end_x = c.start_x;  c.end_y = c.start_y;

                do {  c.end_y--;
                } while (add_move(c,&b,m) and b.board[c.end_x][c.end_y] == 0);

            } else if (p == 'P' or p == 'p') {

                if (p == 'p') {

                    if (b.board[(i/8)+1][i%8]==0) {
                        m.push_back({i/8,i%8,(i/8)+1,i%8,false,false}); }

                    if ((i/8)==1 and b.board[2][i%8]==0 and b.board[3][i%8]==0) {
                        m.push_back({1,i%8,3,i%8,false,false}); }

                    if ((i%8) <= 6 and b.board[(i/8)+1][(i%8)+1] != 0 and 
                    (b.board[(i/8)+1][(i%8)+1] <= 'Z') != b.pos.white_to_move) {

                        m.push_back({i/8,i%8,(i/8)+1,(i%8)+1,false,false}); 
                    }

                    if ((i%8) >= 1 and b.board[(i/8)+1][(i%8)-1] != 0 and 
                    (b.board[(i/8)+1][(i%8)-1] <= 'Z') != b.pos.white_to_move) {

                        m.push_back({i/8,i%8,(i/8)+1,(i%8)-1,false,false}); 
                    }

                } else if (p == 'P') {

                    if (b.board[(i/8)-1][i%8]==0) {
                        m.push_back({i/8,i%8,(i/8)-1,i%8,false,false}); }

                    if ((i/8)==6 and b.board[5][i%8]==0 and b.board[4][i%8]==0) {
                        m.push_back({6,i%8,4,i%8,false,false}); }

                    if ((i%8) <= 6 and b.board[(i/8)-1][(i%8)+1] != 0 and 
                    (b.board[(i/8)-1][(i%8)+1] <= 'Z') != b.pos.white_to_move) {

                        m.push_back({i/8,i%8,(i/8)-1,(i%8)+1,false,false}); 
                    }

                    if ((i%8) >= 1 and b.board[(i/8)-1][(i%8)-1] != 0 and 
                    (b.board[(i/8)-1][(i%8)-1] <= 'Z') != b.pos.white_to_move) {

                        m.push_back({i/8,i%8,(i/8)-1,(i%8)-1,false,false}); 
                    }
                }
            }

            //std::cout<<p<<": "<<m.size()<<std::endl;
            
        }
        i++;
    }

    return m;
}
