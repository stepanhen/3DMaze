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

MainMenu::MainMenu() {}
MainMenu::MainMenu(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    selected = 0;
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
    StartButton();
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
    int msgWidth = 129;
    int msgStartY = screenHeight / 2 - 34 / 2;
    int msgStartX = screenWidth / 2 - msgWidth / 2;
    int i = 1;

    mvaddstr(msgStartY, msgStartX, " __  __                                 ");       
    mvaddstr(msgStartY + i++, msgStartX, "|  \\/  |                   _     ");       
    mvaddstr(msgStartY + i++, msgStartX, "| \\  / |  ___ __   __ ___ (_)    ");       
    mvaddstr(msgStartY + i++, msgStartX, "| |\\/| | / _ \\\\ \\ / // _ \\   ");       
    mvaddstr(msgStartY + i++, msgStartX, "| |  | || (_) |\\ V /|  __/ _     ");       
    mvaddstr(msgStartY + i++, msgStartX, "|_|  |_| \\___/  \\_/  \\___|(_)  ");       
    mvaddstr(msgStartY + i++, msgStartX, " _____         _          _            ");
    mvaddstr(msgStartY + i++, msgStartX, "|  __ \\       | |        | |        _ ");
    mvaddstr(msgStartY + i++, msgStartX, "| |__) | ___  | |_  __ _ | |_  ___ (_) ");
    mvaddstr(msgStartY + i++, msgStartX, "|  _  / / _ \\ | __|/ _` || __|/ _ \\ ");
    mvaddstr(msgStartY + i++, msgStartX, "| | \\ \\| (_) || |_| (_| || |_|  __/ _  ");
    mvaddstr(msgStartY + i++, msgStartX, "|_|  \\_\\\\___/  \\__|\\__,_| \\__|\\___|(_)");                           
    mvaddstr(msgStartY + i++, msgStartX, " ______                                                                                                                   _____  ");
    mvaddstr(msgStartY + i++, msgStartX, "|  ____|                                                                                                              _  |  __ \\ ");
    mvaddstr(msgStartY + i++, msgStartX, "| |__    ___   ___  __ _  _ __    ___    __ _   __ _  _ __ ___    ___    ___   _ __   _ __ ___    ___  _ __   _   _  (_) | |__) |");
    mvaddstr(msgStartY + i++, msgStartX, "|  __|  / __| / __|/ _` || '_ \\  / _ \\  / _` | / _` || '_ ` _ \\  / _ \\  / _ \\ | '__| | '_ ` _ \\  / _ \\| '_ \\ | | | |     |  ___/ ");
    mvaddstr(msgStartY + i++, msgStartX, "| |____ \\__ \\| (__| (_| || |_) ||  __/ | (_| || (_| || | | | | ||  __/ | (_) || |    | | | | | ||  __/| | | || |_| |  _  | |     ");
    mvaddstr(msgStartY + i++, msgStartX, "|______||___/ \\___|\\__,_|| .__/  \\___|  \\__, | \\__,_||_| |_| |_| \\___|  \\___/ |_|    |_| |_| |_| \\___||_| |_| \\__,_| (_) |_|     ");
    mvaddstr(msgStartY + i++, msgStartX, "                         | |             __/ |                                                                                   ");
    mvaddstr(msgStartY + i++, msgStartX, "                         |_|            |___/                                                                                    ");

    refresh();
}