#include <vector>
#include <cmath>
#include <curses.h>
#include <iostream>
#include "game.hpp"

Player::Player() {}

Map::Map() {
    ChangeMap(level1);
}
void Map::ChangeMap(std::vector<std::vector<int>> maze) {
    for(int i = 0; i < mapWidth; i++) {
        for(int j = 0; j < mapHeight; j++) {
            map[i][j] = maze[i][j];
        }
    }
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
    unlockedDoorMsg = false;
    lockedDoorMsg = false;
    keyPicked = false;
    keyHeld = false;
    changeKeyWall = false;
    screenWidth = x;
    screenHeight = y;
    cycleNum = 0;
    level = 1;
    for (int i = 0; i < x; i++) {
        std::vector<int> line;
        for(int j = 0; j < y; j++) {
            line.push_back(0);
        }
        screen.push_back(line);
    }
    ih = InfoHandler(x,y);
    player.posX = 2; 
    player.posY = 2;//x and y start position
    player.dirX = 1; 
    player.dirY = 0; //initial direction vector
    player.planeX = 0; 
    player.planeY = -0.66; //fov is 66 degrees, plae must be perpendicular to player direction

    fc = FrameCounter();
}

void Game::Move(Directions dir) {
    int d;
    switch(dir) {
        case FRONT: d = 1; break;
        case BACK: d = -1; break;
        default: break;
    }

    //checks for collisions and sets new player position
    if(player.posX + d*player.dirX * player.moveSpeed >= 0 && player.posX + player.dirX * player.moveSpeed < mapWidth && player.posY >= 0 && player.posY < mapWidth) 
        if(map.map[int(player.posX + d*player.dirX * player.moveSpeed)][int(player.posY)] == false) 
            player.posX += d*player.dirX * player.moveSpeed;
    if(player.posY + d*player.dirY * player.moveSpeed >= 0 && player.posY + player.dirY * player.moveSpeed < mapHeight && player.posX >= 0 && player.posX < mapHeight) 
        if(map.map[int(player.posX)][int(player.posY + d*player.dirY * player.moveSpeed)] 
            == false) player.posY += d*player.dirY * player.moveSpeed;
}
void Game::Turn(Directions dir) {
    int d;
    
    switch(dir) {
        case LEFT: d = 1; break;
        case RIGHT: d = -1; break;
        default: break;
    }

    //rotates player
    double oldDirX = player.dirX;
    player.dirX = player.dirX * cos(d * player.turnSpeed) - player.dirY * sin(d * player.turnSpeed);
    player.dirY = oldDirX * sin(d * player.turnSpeed) + player.dirY * cos(d * player.turnSpeed);
    double oldPlaneX = player.planeX;
    player.planeX = player.planeX * cos(d * player.turnSpeed) - player.planeY * sin(d * player.turnSpeed);
    player.planeY = oldPlaneX * sin(d * player.turnSpeed) + player.planeY * cos(d * player.turnSpeed);
}

std::vector<int> Game::PrepareNewCol(int drawStart, int drawEnd, int color) {
    std::vector<int> col;
    for(int j = 0; j <= screenHeight; j++) {
        col.push_back(0);
    }

    //draw a vertical line
    for(int i = drawStart; i <= drawEnd; ++i) {
        if(i >= 0 && i <= screenHeight)
            col[i] = color;
    }
    return col;
}

void Game::UpdateScreen(std::vector<int> col, int x) {
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
        double rayDirX = player.dirX + player.planeX * cameraX;
        double rayDirY = player.dirY + player.planeY * cameraX;
        //which box of the map we're in
        int mapX = int(player.posX);
        int mapY = int(player.posY);

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
            sideDistX = (player.posX - mapX) * deltaDistX;
        }
        else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
        }
        if(rayDirY < 0) {
            stepY = -1;
            sideDistY = (player.posY - mapY) * deltaDistY;
        }
        else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
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
            if(map.map[mapX][mapY] > 0) hit = 1;
        }
       
        //calculate ray length
        if(side == 0) perpWallDist = (sideDistX - deltaDistX);
        else          perpWallDist = (sideDistY - deltaDistY);


        //interactive items
        if(x == screenWidth / 2) {
            switch (map.map[mapX][mapY]) {
                case DOOR_LOCKED: //if locked door are in the middle of a screen do:
                    if(perpWallDist <= 1 && !keyHeld) {//player doesn't have a key => print locked door
                        lockedDoorMsg = true;
                    } else if(perpWallDist <= 1 && keyHeld) {//player does have a key => unlock door
                        unlockedDoorMsg = true;
                        currDoorX = mapX;
                        currDoorY = mapY;
                    }
                    break;
                case KEY://if key wall is in the middle of a screen
                    if(perpWallDist <= 1 && !keyHeld) {//picks up the key if the player is close to it
                        keyPicked = true;
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
        int color;
        switch(map.map[mapX][mapY])
        {
            case 1:  color = 1; break; //red
            case 2:  color = 2; break; //blue
            case 3:  color = 3; break; //green
            default: color = 0; break; //black
        }

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
    player.moveSpeed = fc.frameTime * 0.02; //the constant value is in squares/second
    player.turnSpeed = fc.frameTime * 0.01; //the constant value is in radians/second
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
    if(keyPicked && cycleNum <= 15) {//if key was picked show keyfound message for some time
        ih.KeyFoundMsg();
        cycleNum++;
        if(cycleNum == 16) {
            cycleNum = 0;
            keyHeld = true;
            map.map[currKeyX][currKeyY] = WALL;
            if(level == 3) {
                map.map[2][3] = BACKGROUND; //OPEN HIDDEN DOOR
            }
            keyPicked = false;
            PrintScreen();
       }
    }
    if(lockedDoorMsg && cycleNum <= 15) {//show locked door msg for some time
        ih.LockedDoorMsg();
        cycleNum++;
        if(cycleNum == 16) {
            cycleNum = 0;
            lockedDoorMsg = false;
            PrintScreen();
       }
    }
    if(unlockedDoorMsg && cycleNum <= 15) {// show unlocked door msg for some time and change the map
        ih.UnlockedDoorMsg();
        cycleNum++;
        if(cycleNum == 3) {
            level++;
            if(level == 2) {
                map.ChangeMap(level2);
            } else if(level == 3) {
                map.ChangeMap(level3);
            }
        }
        if(cycleNum == 16) {
            cycleNum = 0;
            keyHeld = false;
            unlockedDoorMsg = false;
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
