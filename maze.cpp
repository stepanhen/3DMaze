#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <curses.h>
#include "game.hpp"
#include "levels.hpp"


int main(void) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    
    start_color();
    init_pair(WALL, COLOR_WHITE, COLOR_WHITE); // NORMAL WALL
    init_pair(DOOR_LOCKED, COLOR_MAGENTA, COLOR_MAGENTA); // DOOR LOCKED
    init_pair(KEY, COLOR_RED, COLOR_RED); // WALL WITH KEY
    init_pair(BACKGROUND, COLOR_BLACK, COLOR_BLACK); // BACKGROUND
    
    Game g = Game(COLS, LINES);

    // keypad(stdscr, true);
    bool done = false;
    while(!done){
        g.Render();
        // g.ShowMap();
        int c = getch();
        switch(c) {
            case 97:
                g.Turn(LEFT);
                break;
            case 100:
                g.Turn(RIGHT);
                break;
            case 115:
                g.Move(BACK);
                break;
            case 119:
                g.Move(FRONT);
                break;
            case 'p':
                endwin();
                done = true;;
                break;
        }
    }
    endwin();
}