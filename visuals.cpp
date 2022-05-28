#include "visuals.hpp"
#include <curses.h>


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

MainMenu::MainMenu() {

}
void MainMenu::StartButton(int x, int y) {
    mvaddstr(y, x, " ______   ______  ______   ______  ______");
    mvaddstr(y + 1, x, "/\\  ___\\ /\\__  _\\/\\  __ \\ /\\  == \\/\\__  _\\"); 
    mvaddstr(y + 2, x, "\\ \\___  \\\\/_/\\ \\/\\ \\  __ \\ \\  __<\\/_/\\ \\/");
    mvaddstr(y + 3, x, " \\/\\_____\\  \\ \\_\\ \\ \\_\\ \\_\\ \\_\\ \\_\\ \\ \\_\\");
    mvaddstr(y + 4, x, "  \\/_____/   \\/_/  \\/_/\\/_/ \\/_/ /_/  \\/_/");
}

void MainMenu::HelpButton(int x, int y) {
    mvaddstr(y, x, " __  __   ______   __       ______");
    mvaddstr(y + 1, x, "/\\ \\_\\ \\ /\\  ___\\ /\\ \\     /\\  == \\");
    mvaddstr(y + 2, x, "\\ \\  __ \\\\ \\  __\\ \\ \\ \\____\\ \\  _-/");
    mvaddstr(y + 3, x, " \\ \\_\\ \\_\\\\ \\_____\\\\ \\_____\\\\ \\_\\");
    mvaddstr(y + 4, x, "  \\/_/\\/_/ \\/_____/ \\/_____/ \\/_/");
}

void MainMenu::ExitButton(int x, int y) {
    mvaddstr(y, x, " ______  __  __   __   ______");
    mvaddstr(y + 1, x, "/\\  ___\\/\\_\\_\\_\\ /\\ \\ /\\__  _\\");
    mvaddstr(y + 2, x, "\\ \\  __\\\\/_/\\_\\/_\\ \\ \\\\/_/\\ \\/");
    mvaddstr(y + 3, x, " \\ \\_____\\/\\_\\/\\_\\\\ \\_\\  \\ \\_\\");
    mvaddstr(y + 4, x, "  \\/_____/\\/_/\\/_/ \\/_/   \\/_/");
}

void MainMenu::BackButton(int x, int y) {
    mvaddstr(y, x, " ______  ______  ______  __  __");
    mvaddstr(y + 1, x, "/\\  == \\/\\  __ \\/\\  ___\\/\\ \\/ /");
    mvaddstr(y + 2 , x, "\\ \\  __<\\ \\  __ \\ \\ \\___\\ \\  _'-."); 
    mvaddstr(y + 3, x, " \\ \\_____\\ \\_\\ \\_\\ \\_____\\ \\_\\ \\_\\");
    mvaddstr(y + 4, x, "  \\/_____/\\/_/\\/_/\\/_____/\\/_/\\/_/");
}
