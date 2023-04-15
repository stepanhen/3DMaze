#include <iostream>
#include <curses.h>
#include "game.hpp"
#include "levels.hpp"
#include "visuals.hpp"


int main(void) {
    // Initialize screen
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(WALL, COLOR_WHITE, COLOR_WHITE); // NORMAL WALL
    init_pair(DOOR_LOCKED, COLOR_MAGENTA, COLOR_MAGENTA); // DOOR LOCKED
    init_pair(KEY, COLOR_RED, COLOR_RED); // WALL WITH KEY
    init_pair(BACKGROUND, COLOR_BLACK, COLOR_BLACK); // BACKGROUND
    init_pair(BUTTON, COLOR_GREEN, COLOR_GREEN); // BUTTON
    init_pair(WALL_UP, COLOR_YELLOW, COLOR_YELLOW); // MOVABLE WALL
    
    Game g = Game(COLS, LINES);
    MainMenu m = MainMenu(COLS, LINES);
    HelpMenu h = HelpMenu(COLS, LINES);

    //Main loop
    while(true) {
        //Print main menu and wait till user selects what he wants
        m.PrintMenu();
        bool done = false;
        while(!done) {
            int c = getch();
            switch(c) {
                case 'w':
                    m.Up();
                    m.PrintMenu();
                    break;
                case 's':
                    m.Down();
                    m.PrintMenu();
                    break;
                case '\n':
                    clear();
                    done = true;;
                    break;
            }
        }
        //User selected exit
        if(m.selected == 2) { endwin(); return 1;}
        //User selected help
        else if(m.selected == 1) {
            //Print help menu
            h.PrintMenu();
            while(true) {
                int c = getch();
                if(c == 'p'){
                    clear();
                    break;;
                }
            }
        }
        //User selected start
        if(m.selected == 0) {
            //Start or resume the game
            m.inGame = true;
            g.PrintScreen();
            bool pause = false;
            while(!pause){
                g.Render();
                int c = getch();
                switch(c) {
                    case 'a':
                        g.Turn(LEFT);
                        break;
                    case 'd':
                        g.Turn(RIGHT);
                        break;
                    case 's':
                        g.Move(BACK);
                        break;
                    case 'w' :
                        g.Move(FRONT);
                        break;
                    case 'p':
                        pause = true;;
                        break;
                    case 'n':
                        g.SwitchPlayers();
                        break;
                    case 'e':
                        if(g.buttonClickExpected) {
                            g.ButtonReact();
                            g.buttonClickExpected = false;
                        }
                        break;
                } 
            }
        }
    }
}