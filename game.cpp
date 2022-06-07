#include <vector>
#include <memory>
#include <cmath>
#include <curses.h>
#include <iostream>
#include "game.hpp"
#include "levels.hpp"

Player::Player() {}

Map::Map() {}
Map::Map(int mapNum) {
    if(mapNum == 1) { maps = maps1; }
    else{ maps = maps2; }
    map = maps[0];
    currentMap = 0;
    unlockedDoorMsg = false;
    lockedDoorMsg = false;
    keyPicked = false;
    keyHeld = false;
    changeKeyWall = false;
}

void Map::NextMap() {
    map = maps[++currentMap];
}
 
FrameCounter::FrameCounter() { time = 0; oldTime = 0; }

void FrameCounter::SetTime() {
    oldTime = time;
    auto t = clock();
    time = static_cast<double>(t);
    frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
}

Game::Game() {}
Game::Game(int x,int y) {
    currentMap = std::make_shared<Map>(1);
    offlineMap = std::make_shared<Map>(2);

    screenWidth = x;
    screenHeight = y;
    cycleNum = 0;
    for (int i = 0; i < x; i++) {
        std::vector<int> line(y, 0);
        screen.push_back(std::move(line));
    }
    ih = InfoHandler(x,y);    

    currentPlayer = std::make_shared<Player>();
    offlinePlayer = std::make_shared<Player>();

    currentPlayer -> posX = 2; 
    currentPlayer -> posY = 2;//x and y start position
    currentPlayer -> dirX = 1; 
    currentPlayer -> dirY = 0; //initial direction vector
    currentPlayer -> planeX = 0; 
    currentPlayer -> planeY = -0.66; //fov is 66 degrees, plae must be perpendicular to player direction
    currentPlayer -> wallColor = COLOR_WHITE;

    offlinePlayer -> posX = 2; 
    offlinePlayer -> posY = 2;//x and y start position
    offlinePlayer -> dirX = 1; 
    offlinePlayer -> dirY = 0; //initial direction vector
    offlinePlayer -> planeX = 0; 
    offlinePlayer -> planeY = -0.66; //fov is 66 degrees, plae must be perpendicular to player direction
    offlinePlayer -> wallColor = COLOR_BLUE;

    fc = FrameCounter();
}

void Game::SwitchMaps() {
    auto m = currentMap;
    currentMap = offlineMap;
    offlineMap = m;
}

void Game::SwitchPlayers() {
    auto p= currentPlayer;
    currentPlayer = offlinePlayer;
    offlinePlayer = p;
    init_pair(WALL, currentPlayer -> wallColor, currentPlayer -> wallColor);
    Render();
    PrintScreen();

    SwitchMaps();
}

void Game::Move(Directions dir) {
    int d;
    switch(dir) {
        case FRONT: d = 1; break;
        case BACK: d = -1; break;
        default: break;
    }

    //checks for collisions and sets new currentPlayer position
    if(currentPlayer->posX + d*currentPlayer->dirX * currentPlayer->moveSpeed >= 0 && currentPlayer->posX + currentPlayer->dirX * currentPlayer->moveSpeed < mapWidth && currentPlayer->posY >= 0 && currentPlayer->posY < mapWidth) 
        if(currentMap -> map[int(currentPlayer->posX + d*currentPlayer->dirX * currentPlayer->moveSpeed)][int(currentPlayer->posY)] == false) 
            currentPlayer->posX += d*currentPlayer->dirX * currentPlayer->moveSpeed;
    if(currentPlayer->posY + d*currentPlayer->dirY * currentPlayer->moveSpeed >= 0 && currentPlayer->posY + currentPlayer->dirY * currentPlayer->moveSpeed < mapHeight && currentPlayer->posX >= 0 && currentPlayer->posX < mapHeight) 
        if(currentMap -> map[int(currentPlayer->posX)][int(currentPlayer->posY + d*currentPlayer->dirY * currentPlayer->moveSpeed)] 
            == false) currentPlayer->posY += d*currentPlayer->dirY * currentPlayer->moveSpeed;
}
void Game::Turn(Directions dir) {
    int d;
    
    switch(dir) {
        case LEFT: d = 1; break;
        case RIGHT: d = -1; break;
        default: break;
    }

    //rotates player
    double oldDirX = currentPlayer->dirX;
    currentPlayer->dirX = currentPlayer->dirX * cos(d * currentPlayer->turnSpeed) - currentPlayer->dirY * sin(d * currentPlayer->turnSpeed);
    currentPlayer->dirY = oldDirX * sin(d * currentPlayer->turnSpeed) + currentPlayer->dirY * cos(d * currentPlayer->turnSpeed);
    double oldPlaneX = currentPlayer->planeX;
    currentPlayer->planeX = currentPlayer->planeX * cos(d * currentPlayer->turnSpeed) - currentPlayer->planeY * sin(d * currentPlayer->turnSpeed);
    currentPlayer->planeY = oldPlaneX * sin(d * currentPlayer->turnSpeed) + currentPlayer->planeY * cos(d * currentPlayer->turnSpeed);
}

std::vector<int> Game::PrepareNewCol(int drawStart, int drawEnd, int color) {
    std::vector<int> col (screenHeight, 0);

    //draw a vertical line
    for(int i = drawStart; i <= drawEnd; ++i) {
        if(i >= 0 && i <= screenHeight)
            col[i] = color;
    }
    return col;
}

void Game::UpdateScreen(const std::vector<int> & col, int x) {
    //gets a vertical line and updates screen with the new parts of it
    //this screen buffering approach eliminates screen flickering when reloading screen
    int color;
    for(int i = 0; i <= screenHeight; i++) {
        if(screen[x][i] != col[i]) {
            screen[x][i] = col[i];
            const char* c;
            bool bold = false;
            switch(col[i]) {
                case 0: color = 0; c = " "; break;
                case 1: color = 1; c = " "; break;
                case 2: color = 2; c = " "; break;
                case 3: color = 3; c = " "; break;
                case 10: color = 0; c = "\\"; bold = true; break;
                case 11: color = 1; c = "\\"; bold = true; break;
                case 12: color = 2; c = "\\"; bold = true; break;
                case 13: color = 3; c = "\\"; bold = true; break;
            }
            if(bold) attron(A_BOLD);
            attron(COLOR_PAIR(color));
            mvaddstr(i, x, c);
            attroff(COLOR_PAIR(color));  
            if(bold) attroff(A_BOLD);
        }
    }
}

void Game::RayCast() {
    for(int x = 0; x < screenWidth; x++)
    {
        //calculate ray position and direction
        double cameraX = 2 * x / (double)screenWidth - 1; //x-coordinate in camera space
        double rayDirX = currentPlayer->dirX + currentPlayer->planeX * cameraX;
        double rayDirY = currentPlayer->dirY + currentPlayer->planeY * cameraX;
        //which box of the map we're in
        int mapX = int(currentPlayer->posX);
        int mapY = int(currentPlayer->posY);

        //length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        //length of ray from one x or y side to next x or y side
        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

        //ray length
        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; //if ray hit the wall
        int side;
        //calculate step, rays will be moving by squares
        if(rayDirX < 0) {
            stepX = -1;
            sideDistX = (currentPlayer->posX - mapX) * deltaDistX;
        }
        else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - currentPlayer->posX) * deltaDistX;
        }
        if(rayDirY < 0) {
            stepY = -1;
            sideDistY = (currentPlayer->posY - mapY) * deltaDistY;
        }
        else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - currentPlayer->posY) * deltaDistY;
        }
        
        while(hit == 0) {
            //jump to next map square, either in x-direction, or in y-direction
            if(sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            //check if ray has hit a wall
            if(currentMap -> map[mapX][mapY] > 0) hit = 1;
        }
       
        //calculate ray length
        if(side == 0) perpWallDist = (sideDistX - deltaDistX);
        else          perpWallDist = (sideDistY - deltaDistY);


        //interactive items
        if(x == screenWidth / 2) {
            switch (currentMap -> map[mapX][mapY]) {
                case DOOR_LOCKED: //if locked door are in the middle of a screen do:
                    if(perpWallDist <= 1 && !currentMap -> keyHeld) {//player doesn't have a key => print locked door
                        currentMap -> lockedDoorMsg = true;
                    } else if(perpWallDist <= 1 && currentMap -> keyHeld) {//player does have a key => unlock door
                        currentMap -> unlockedDoorMsg = true;
                        currDoorX = mapX;
                        currDoorY = mapY;
                    }
                    break;
                case KEY://if key wall is in the middle of a screen
                    if(perpWallDist <= 1 && !currentMap -> keyHeld) {//picks up the key if the player is close to it
                        currentMap -> keyPicked = true;
                        currKeyX = mapX;
                        currKeyY = mapY;
                    }
                    break;
            }
        }

        //calculate height of line to draw on screen
        int lineHeight = (int)(screenHeight / perpWallDist);
        if(lineHeight > screenHeight) lineHeight = screenHeight;

        //calculate lowest and highest point to fill in current line
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        int drawEnd = lineHeight / 2 + screenHeight / 2;

        //choose wall color
        int color = currentMap -> map[mapX][mapY];
        //give x and y sides different shade
        int bold = 0;
        if(side == 1) {bold = 10;}

        //draw vertical line
        std::vector<int> col = PrepareNewCol(drawStart, drawEnd, color+bold);
        UpdateScreen(col,x);
    }
}

void Game::SetSpeed() {
    fc.SetTime();
    currentPlayer->moveSpeed = fc.frameTime * 0.03; //the constant value is in squares/second
    currentPlayer->turnSpeed = fc.frameTime * 0.02; //the constant value is in radians/second
}

void Game::PrintScreen() {
    //prints the whole screen
    for(int i = 0; i < screenWidth; i++) {
        for(int j = 0; j < screenHeight; j++) {
            const char* c;
            int color;
            bool bold = false;
            switch(screen[i][j]) {
                case 0: color = 0; c = " "; break;
                case 1: color = 1; c = " "; break;
                case 2: color = 2; c = " "; break;
                case 3: color = 3; c = " "; break;
                case 10: color = 0; c = "\\"; bold = true; break;
                case 11: color = 1; c = "\\"; bold = true; break;
                case 12: color = 2; c = "\\"; bold = true; break;
                case 13: color = 3; c = "\\"; bold = true; break;
            }
            if(bold) attron(A_BOLD);
            attron(COLOR_PAIR(color));
            mvaddstr(j, i, c);
            attroff(COLOR_PAIR(color));  
            if(bold) attroff(A_BOLD);
        }
    }
}

void Game::GameMechanics() {
    if(currentMap -> keyPicked && cycleNum <= 15) {//if key was picked show keyfound message for some time
        ih.KeyFoundMsg();
        currentMap -> keyHeld = true;
        cycleNum++;
        if(cycleNum == 16) {
            cycleNum = 0;
            currentMap -> map[currKeyX][currKeyY] = WALL;
            if(currentMap -> currentMap == 2) {
                currentMap -> map[2][3] = BACKGROUND; //OPEN HIDDEN DOOR
            }
            currentMap -> keyPicked = false;
            PrintScreen();
       }
    }
    if(currentMap -> lockedDoorMsg && cycleNum <= 15) {//show locked door msg for some time
        ih.LockedDoorMsg();
        cycleNum++;
        if(cycleNum == 16) {
            cycleNum = 0;
            currentMap -> lockedDoorMsg = false;
            PrintScreen();
       }
    }
    if(currentMap -> unlockedDoorMsg && cycleNum <= 15) {// show unlocked door msg for some time and change the map
        ih.UnlockedDoorMsg();
        cycleNum++;
        if(cycleNum == 3) {
            currentMap -> NextMap();
        }
        if(cycleNum == 16) {
            cycleNum = 0;
            currentMap -> keyHeld = false;
            currentMap -> unlockedDoorMsg = false;
            PrintScreen();
        }
    }
}


void Game::Render() {
    RayCast();
    SetSpeed();
    GameMechanics();
    refresh();
}
/*
    TODO:
        - Add buttons that activate traps, open walls, create walls
        - Add restart button
*/