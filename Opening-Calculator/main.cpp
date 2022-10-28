#include "draw.cpp"
#include <fstream>
#include <iostream>

int main() {

    // LOAD FROM TXT FILE

    Board board;
    std::unordered_map<struct Position, struct Node*, MyHashFunction> pos_map;

    struct Position std_pos = {"rnbqkbnrpppppppp8888PPPPPPPPRNBQKBNR",
    "Starting Position",true};

    pos_map[std_pos] = new struct Node;
    pos_map[std_pos]->pos = std_pos;

    std::fstream openings;
    openings.open("all-openings-sorted.txt", std::ios::in);

    if (openings.is_open()) {

        std::string line;

        while (getline(openings, line)) {
            
            board.Reset_Board();

            std::vector<std::string> line_moves = get_moves(line);

            for (auto move: line_moves) {

                board.Make_Move(board.Find_Move_Coords(move));

                struct Position new_pos = 
                {board.pos.FEN,get_name(line),board.pos.white_to_move};

                if (pos_map.find(new_pos) == pos_map.end()) {

                    pos_map[new_pos] = new struct Node;
                    pos_map[new_pos]->pos = new_pos;
                }
            }
        }

        openings.close();
    }

    // CREATE NODE LINKS

    for (auto element: pos_map) {

        struct Node* node_at = element.second;

        board.pos = node_at->pos;
        board.Read_FEN();

        std::vector<struct Move_Coords> possible_moves = pseudo_legal(board);

        for (auto move: possible_moves) {

            board.pos = node_at->pos;
            board.Read_FEN();

            board.Make_Move(move);

            auto next_element = pos_map.find(board.pos);

            if (next_element != pos_map.end()) {

                node_at->next_nodes.push_back({move, next_element->second});

                next_element->second->previous_nodes.push_back(
                    {reverse_move(move),node_at});
            }
        }
    }

    // INITIALIZE WINDOW

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Openings");
    window.setFramerateLimit(30);

    Draw draw(&window);

    board.Reset_Board();
    struct Node* current_node = pos_map[std_pos];
    struct Move_Coords current_move;

    // EVENT LOOP

    draw.display_window(&board, current_node);

    while (window.isOpen()) {

        sf::Event event;
        if (window.waitEvent(event)) {

            if (event.type == sf::Event::Closed) {

                for (auto element: pos_map)
                    delete element.second;

                window.close();
            }
                
            if (event.type == sf::Event::Resized) {

                draw.resize_window(event.size.height, event.size.width);
                draw.display_window(&board, current_node);
            }

            if (event.type == sf::Event::MouseButtonPressed) {

                sf::Vector2f click_coords = (window.mapPixelToCoords(sf::Vector2i(
                    (event.mouseButton.x), (event.mouseButton.y))));

                draw.get_click(click_coords, current_node);
                board.pos = current_node->pos;
                board.Read_FEN();
                draw.display_window(&board, current_node);
            }  
        }   
    }

    return 0;
}
