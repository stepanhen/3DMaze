#include "visuals.hpp"

#ifdef _WIN32
#include <ncurses\curses.h>
#else
#include <curses.h>
#endif


InfoHandler::InfoHandler() {}
InfoHandler::InfoHandler(int w, int h) { screenWidth = w; screenHeight = h; }
int InfoHandler::GetMsgStartX(int w) {
    return screenWidth / 2 - w / 2;
}
int InfoHandler::GetMsgStartY(int h) {
    return screenHeight / 2 - h / 2;
}
void InfoHandler::KeyFoundMsg() {
    int msgWidth = 56;
    int msgHeigth = 5;
    int msgStartX = GetMsgStartX(msgWidth);
    int msgStartY = GetMsgStartY(msgHeigth);
    mvaddstr(msgStartY, msgStartX,      "   _____ _____ __ __    _____ _____ _____ _____ ____    ");
    mvaddstr(msgStartY + 1, msgStartX,  "  |  |  |   __|  |  |  |   __|     |  |  |   | |    \\   ");
    mvaddstr(msgStartY + 2, msgStartX,  "  |    -|   __|_   _|  |   __|  |  |  |  | | | |  |  |  ");    
    mvaddstr(msgStartY + 3, msgStartX,  "  |__|__|_____| |_|    |__|  |_____|_____|_|___|____/   ");
    mvaddstr(msgStartY + 4, msgStartX,  "                                                        ");
}
void InfoHandler::LockedDoorMsg() {
    int msgWidth = 68;
    int msgHeigth = 5;
    int msgStartX = GetMsgStartX(msgWidth);
    int msgStartY = GetMsgStartY(msgHeigth);
    mvaddstr(msgStartY, msgStartX,     "   __    _____ _____ _____ _____ ____     ____  _____ _____ _____   ");
    mvaddstr(msgStartY + 1, msgStartX, "  |  |  |     |     |  |  |   __|    \\   |    \\|     |     | __  |  ");
    mvaddstr(msgStartY + 2, msgStartX, "  |  |__|  |  |   --|    -|   __|  |  |  |  |  |  |  |  |  |    -|  ");
    mvaddstr(msgStartY + 3, msgStartX, "  |_____|_____|_____|__|__|_____|____/   |____/|_____|_____|__|__|  ");
    mvaddstr(msgStartY + 4, msgStartX, "                                                                    ");
}
void InfoHandler::UnlockedDoorMsg() {
    int msgWidth = 80;
    int msgHeigth = 5;
    int msgStartX = GetMsgStartX(msgWidth);
    int msgStartY = GetMsgStartY(msgHeigth);
    mvaddstr(msgStartY, msgStartX,     "   ____  _____ _____ _____    _____ _____ __    _____ _____ _____ _____ ____    ");
    mvaddstr(msgStartY + 1, msgStartX, "  |    \\|     |     | __  |  |  |  |   | |  |  |     |     |  |  |   __|    \\   ");
    mvaddstr(msgStartY + 2, msgStartX, "  |  |  |  |  |  |  |    -|  |  |  | | | |  |__|  |  |   --|    -|   __|  |  |  ");
    mvaddstr(msgStartY + 3, msgStartX, "  |____/|_____|_____|__|__|  |_____|_|___|_____|_____|_____|__|__|_____|____/   ");
    mvaddstr(msgStartY + 4, msgStartX, "                                                                                ");
}

MainMenu::MainMenu() {}
MainMenu::MainMenu(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    selected = 0;
    inGame = false;
}

void MainMenu::ResumeButton() {
    int msgWidth = 54;
    if(selected == 0) msgWidth -= 20;
    int msgStartX = screenWidth / 2 - msgWidth / 2;
    int msgStartY = (screenHeight - 15) / 4;

    mvaddstr(msgStartY + 0, msgStartX, " ______  ______  ______  __  __  __    __  ______    ");
    mvaddstr(msgStartY + 1, msgStartX, "/\\  == \\/\\  ___\\/\\  ___\\/\\ \\/\\ \\/\\ \"-./  \\/\\  ___\\   ");
    mvaddstr(msgStartY + 2, msgStartX, "\\ \\  __<\\ \\  __\\\\ \\___  \\ \\ \\_\\ \\ \\ \\-./\\ \\ \\  __\\   ");
    mvaddstr(msgStartY + 3, msgStartX, " \\ \\_\\ \\_\\ \\_____\\/\\_____\\ \\_____\\ \\_\\ \\ \\_\\ \\_____\\ ");
    mvaddstr(msgStartY + 4, msgStartX, "  \\/_/ /_/\\/_____/\\/_____/\\/_____/\\/_/  \\/_/\\/_____/ ");                                                     
}

void MainMenu::StartButton() {
    int msgWidth = 42;
    if(selected == 0) msgWidth -= 20;
    int msgStartX = screenWidth / 2 - msgWidth / 2;
    int msgStartY = (screenHeight - 15) / 4;

    mvaddstr(msgStartY    , msgStartX, " ______   ______  ______   ______  ______ "); 
    mvaddstr(msgStartY + 1, msgStartX, "/\\  ___\\ /\\__  _\\/\\  __ \\ /\\  == \\/\\__  _\\"); 
    mvaddstr(msgStartY + 2, msgStartX, "\\ \\___  \\\\/_/\\ \\/\\ \\  __ \\\\ \\  __<\\/_/\\ \\/"); 
    mvaddstr(msgStartY + 3, msgStartX, " \\/\\_____\\  \\ \\_\\ \\ \\_\\ \\_\\\\ \\_\\ \\_\\ \\ \\_\\"); 
    mvaddstr(msgStartY + 4, msgStartX, "  \\/_____/   \\/_/  \\/_/\\/_/ \\/_/ /_/  \\/_/"); 
}

void MainMenu::HelpButton() {
    int msgWidth = 35;
    if(selected == 1) msgWidth -= 20;
    int msgStartX = screenWidth / 2 - msgWidth / 2;
    int msgStartY = 2*((screenHeight - 15) / 4) + 5;

    mvaddstr(msgStartY,     msgStartX, " __  __   ______   __       ______");
    mvaddstr(msgStartY + 1, msgStartX, "/\\ \\_\\ \\ /\\  ___\\ /\\ \\     /\\  == \\");
    mvaddstr(msgStartY + 2, msgStartX, "\\ \\  __ \\\\ \\  __\\ \\ \\ \\____\\ \\  __/");
    mvaddstr(msgStartY + 3, msgStartX, " \\ \\_\\ \\_\\\\ \\_____\\\\ \\_____\\\\ \\_\\");
    mvaddstr(msgStartY + 4, msgStartX, "  \\/_/\\/_/ \\/_____/ \\/_____/ \\/_/");
}

void MainMenu::ExitButton() {
    int msgWidth = 30;
    if(selected == 2) msgWidth -= 20;
    int msgStartX = screenWidth / 2 - msgWidth / 2;
    int msgStartY = 3*((screenHeight - 15) / 4) + 10;

    mvaddstr(msgStartY,     msgStartX, " ______  __  __   __   ______");
    mvaddstr(msgStartY + 1, msgStartX, "/\\  ___\\/\\_\\_\\_\\ /\\ \\ /\\__  _\\");
    mvaddstr(msgStartY + 2, msgStartX, "\\ \\  __\\\\/_/\\_\\/_\\ \\ \\\\/_/\\ \\/");
    mvaddstr(msgStartY + 3, msgStartX, " \\ \\_____\\/\\_\\/\\_\\\\ \\_\\  \\ \\_\\");
    mvaddstr(msgStartY + 4, msgStartX, "  \\/_____/\\/_/\\/_/ \\/_/   \\/_/");
}

void MainMenu::Down() {
    selected++;
    if (selected > 2) selected = 0;
}

void MainMenu::Up() {
    selected--;
    if (selected < 0) selected = 2;
    
}

void MainMenu::PrintMenu() {
    clear();
    if (inGame) {
        ResumeButton();
    } else {
        StartButton();
    }
    HelpButton();
    ExitButton();
    refresh();
}

HelpMenu::HelpMenu() {}
HelpMenu::HelpMenu(int w, int h) {
    screenWidth = w;
    screenHeight = h;
}

void HelpMenu::PrintMenu() {
    int start = 2;
    init_pair(10, COLOR_BLUE, COLOR_BLUE);
    init_pair(11, COLOR_BLACK, COLOR_WHITE);
    mvaddstr(start, 10, "WALL TYPES:");

    start += 2;
    attron(COLOR_PAIR(1));
    mvaddstr(start, 10, "   ");
    attroff(COLOR_PAIR(1));
    mvaddstr(start, 14, "First player default wall.");

    start += 2;
    attron(COLOR_PAIR(10));
    mvaddstr(start, 10, "   ");
    attroff(COLOR_PAIR(10));
    mvaddstr(start, 14, "Second player default wall.");

    start += 2;
    attron(COLOR_PAIR(2));
    mvaddstr(start, 10, "   ");
    attroff(COLOR_PAIR(2)); 
    mvaddstr(start, 14, "Locked door. Unlocking them will change the map to a higher level.");

    start += 2;
    attron(COLOR_PAIR(3));
    mvaddstr(start, 10, "   ");
    attroff(COLOR_PAIR(3)); 
    mvaddstr(start, 14, "Wall containing a key to locked doors.");

    start += 2;
    attron(COLOR_PAIR(5));
    mvaddstr(start, 10, "   ");
    attroff(COLOR_PAIR(5)); 
    mvaddstr(start, 14, "Movable wall which can appear or disappear and player can walk through.");

    start += 2;
    attron(COLOR_PAIR(4));
    mvaddstr(start, 10, "   ");
    attroff(COLOR_PAIR(4)); 
    mvaddstr(start, 14, "Wall with button which can move with movable walls. (BUTTONS ONLY WORK AT THE SAME LEVEL!)");

    start += 2;
    mvaddstr(start, 10, "CONTROLS:");

    start += 2;
    attron(COLOR_PAIR(11));
    mvaddstr(start, 10, " W ");
    attroff(COLOR_PAIR(11));
    mvaddstr(start, 14, "Move forward.");
    
    start += 2;
    attron(COLOR_PAIR(11));
    mvaddstr(start, 10, " S ");
    attroff(COLOR_PAIR(11));
    mvaddstr(start, 14, "Move back.");

    start += 2;
    attron(COLOR_PAIR(11));
    mvaddstr(start, 10, " A ");
    attroff(COLOR_PAIR(11));
    mvaddstr(start, 14, "Turn left.");

    start += 2;
    attron(COLOR_PAIR(11));
    mvaddstr(start, 10, " D ");
    attroff(COLOR_PAIR(11));
    mvaddstr(start, 14, "Turn right.");

    start += 2;
    attron(COLOR_PAIR(11));
    mvaddstr(start, 10, " P ");
    attroff(COLOR_PAIR(11));
    mvaddstr(start, 14, "Open menu.");

    start += 2;
    attron(COLOR_PAIR(11));
    mvaddstr(start, 10, " N ");
    attroff(COLOR_PAIR(11));
    mvaddstr(start, 14, "Switch players.");

    start += 2;
    attron(COLOR_PAIR(11));
    mvaddstr(start, 10, " E ");
    attroff(COLOR_PAIR(11));
    mvaddstr(start, 14, "Activate button. (PLAYER MUST BE AT MOST ONE SQUARE FROM BUTTON)");

    start += 2;
    attron(COLOR_PAIR(11));
    mvaddstr(start, 10, " Enter ");
    attroff(COLOR_PAIR(11));
    mvaddstr(start, 18, "Confirm.");
    refresh();
}