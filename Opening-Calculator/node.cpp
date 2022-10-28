#include "pseudo.cpp"

std::vector<std::string> get_moves(std::string line) {

    std::vector<std::string> moves;

    int index = 0;

    while (line[index] != '|') {++index;}
    index+=2;

    while (index < line.length()) {

        int front = index;

        while (index < line.length() and line[index] != ' ') {++index;}

        if (!('1' <= line[front] and line[front] <= '9'))
            moves.push_back(line.substr(front, index - front));

        index++;
    }

    return moves;
}

std::string get_name(std::string line) {

    int index = 0;

    while (line[index] != '|') {++index;}
    index--;

    return line.substr(0,index);
}

struct Node_Linker {

    struct Move_Coords move;
    struct Node* linked_node;
};

struct Node {

    std::vector<struct Node_Linker> previous_nodes;

    struct Position pos;

    std::vector<struct Node_Linker> next_nodes;
};

class MyHashFunction {
public:
    size_t operator()(const Position& n) const {
        return (std::hash<std::string>()(n.FEN)) ^ 
        (std::hash<bool>()(n.white_to_move));
    }
};

struct Move_Coords reverse_move(struct Move_Coords m) {

    return {m.end_x,m.end_y,m.start_x,m.start_y
    ,m.castles_kingside,m.castles_queenside};
}
