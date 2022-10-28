#include "node.cpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <cmath>
#include <iostream>

const float BOARD_SIZE = 800.f;

const float SQUARE_SIZE = BOARD_SIZE / 8.f;

const float WIDTH = BOARD_SIZE;
const float HEIGHT = BOARD_SIZE * (17.f/16.f);

float arrow_width = 30;
float font_size = 20;

class Draw {

    sf::RenderWindow* window;

    std::unordered_map<char, sf::Texture> piece_textures;
    sf::Texture arrow_texture;
    sf::Texture board_texture;

    sf::RectangleShape chess_board;
    sf::RectangleShape highlight;
    sf::Sprite arrow;
    bool square_selected = false;

    sf::Font font;
    sf::Text opening_name;
    sf::Text signature;

    struct Move_Coords current_move;

public:

    Draw(sf::RenderWindow* w) {

        // Link Window

        window = w;

        // Load Piece, Board, Arrow Textures

        std::vector<char> piece_notation = 
        {'K','Q','R','B','N','P','k','q','r','b','n','p'};
        sf::Texture piece_texture;

        for(auto c: piece_notation) {

        std::string filepath = "Resources/";
        
            if (c <= 'Z') {
                filepath += 'w';  filepath += (c + ('a'-'A'));
            } else {
                filepath += 'b';  filepath += c;
            }
            filepath += ".png";

            piece_texture.loadFromFile(filepath.c_str());
            piece_textures[c] = piece_texture;
        }

        board_texture.loadFromFile("Resources/board.png");
        arrow_texture.loadFromFile("Resources/arrow.png");

        // Load Other Textures & Shapes

        chess_board.setSize(sf::Vector2f(WIDTH,WIDTH));
        chess_board.setTexture(&board_texture);

        highlight.setSize(sf::Vector2f(SQUARE_SIZE,SQUARE_SIZE));
        highlight.setFillColor(sf::Color(255,0,0,75));

        arrow.setTexture(arrow_texture);

        font.loadFromFile("Resources/OpenSans-Regular.ttf");

        opening_name.setString("Starting Position");
        opening_name.setFont(font);
        opening_name.setCharacterSize(font_size);
        opening_name.setPosition(sf::Vector2f(
            20, BOARD_SIZE + (HEIGHT-BOARD_SIZE-font_size)/2.f));

        signature.setString("Kareem Alzahal 2022");
        signature.setStyle(sf::Text::Italic);
        signature.setFont(font);
        signature.setCharacterSize(font_size);
        signature.setPosition(sf::Vector2f(
            BOARD_SIZE - signature.getGlobalBounds().width - 20
            , BOARD_SIZE + (HEIGHT-BOARD_SIZE-font_size)/2.f));
    }

    void resize_window(float newheight, float newwidth) {

        sf::View view;
        view.reset(sf::FloatRect(0.f,0.f,WIDTH,HEIGHT));
            
        if (newwidth < newheight*WIDTH/HEIGHT) {
                
            float ratio = newwidth*HEIGHT/WIDTH/newheight;
            view.setViewport(sf::FloatRect(0.f,(1.f-ratio)/2.f,1.f,ratio));  
        } else { 
            float ratio = newheight*WIDTH/HEIGHT/newwidth;
            view.setViewport(sf::FloatRect((1.f-ratio)/2.f,0.f,ratio,1.f));   
        }

        window->setView(view);
    }

    void draw_pieces(Board* b) {

        sf::RectangleShape piece(sf::Vector2f(SQUARE_SIZE,SQUARE_SIZE));

        for (int i = 0; i < 64; i++) {

            if (b->board[i/8][i%8]) {

                piece.setPosition(sf::Vector2f((i%8)*100,(i/8)*100));
                piece.setTexture(&(piece_textures[b->board[i/8][i%8]]));
                window->draw(piece);
            }
        }
    }

    void draw_arrow(Move_Coords m, sf::Color c) {

        int a = m.end_x - m.start_x;
        int b = m.end_y - m.start_y;
        float distance = sqrt(a*a + b*b);
        float angle = acos(a/distance)*(180/M_PI);

        if (b > 0) {angle *= -1;}

        

        arrow.setTextureRect(sf::IntRect(0,0,SQUARE_SIZE*distance,arrow_width));
        arrow.setColor(c);
        arrow.setOrigin(0, arrow_width/2);
        arrow.setPosition(m.end_y*SQUARE_SIZE + SQUARE_SIZE/2,
        m.end_x*SQUARE_SIZE + SQUARE_SIZE/2);
        arrow.setRotation(angle-90);
        
        window->draw(arrow);
    }

    void draw_arrows(Node* n) {

        for (auto m: n->next_nodes) 
            draw_arrow(m.move,sf::Color::Cyan);
        
        for (auto m: n->previous_nodes) 
            draw_arrow(m.move,sf::Color::Red);
    }

    void get_click(sf::Vector2f click_coords, Node* &current_node ) {//, Board* b) {

        int s = SQUARE_SIZE;
        int cx = click_coords.x;
        int cy = click_coords.y;

        if (0 < cx and cx < BOARD_SIZE and 
            0 < cy and cy < BOARD_SIZE) {

            if (square_selected == false) {

                current_move.start_x = cy/s;
                current_move.start_y = cx/s;
                highlight.setPosition(sf::Vector2f(cx/s*s,cy/s*s));
                square_selected = !square_selected;

            } else if (square_selected == true) {

                current_move.end_x = cy/s;
                current_move.end_y = cx/s;
                square_selected = !square_selected;

                // Find If Move is in Node, etc.

                for (auto move_linker: current_node->next_nodes) {

                    if (current_move == move_linker.move) {

                        current_node = move_linker.linked_node;
                        opening_name.setString(current_node->pos.Name);
                        //std::cout<<current_node->pos.Name<<std::endl;
                        return;
                    }
                }

                for (auto move_linker: current_node->previous_nodes) {

                    if (current_move == move_linker.move) {

                        current_node = move_linker.linked_node;
                        opening_name.setString(current_node->pos.Name);
                        //std::cout<<current_node->pos.Name<<std::endl;
                        return;
                    }
                }
            }

        } else {

            square_selected = false;
        }
    }

    void display_window(Board *b, Node* n) {

        window->clear();
        window->draw(chess_board);
        if (square_selected) {window->draw(highlight);}
        draw_pieces(b);
        draw_arrows(n);
        window->draw(opening_name);
        window->draw(signature);
        window->display();
    }

};
