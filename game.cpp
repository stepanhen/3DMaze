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
    player.posY = 2;  //x and y start position
    player.dirX = 1; 
    player.dirY = 0; //initial direction vector
    player.planeX = 0; 
    player.planeY = -0.66; //the 2d raycaster version of camera plane

    fc = FrameCounter();
}

void Game::Move(Directions dir) {
    int d;
    switch(dir) {
        case FRONT: d = 1; break;
        case BACK: d = -1; break;
        default: break;
    }

    if(player.posX + d*player.dirX * player.moveSpeed >= 0 && player.posX + player.dirX * player.moveSpeed < mapWidth && player.posY >= 0 && player.posY < mapWidth) if(map.map[int(player.posX + d*player.dirX * player.moveSpeed)][int(player.posY)] == false) player.posX += d*player.dirX * player.moveSpeed;
    if(player.posY + d*player.dirY * player.moveSpeed >= 0 && player.posY + player.dirY * player.moveSpeed < mapHeight && player.posX >= 0 && player.posX < mapHeight) if(map.map[int(player.posX)][int(player.posY + d*player.dirY * player.moveSpeed)] == false) player.posY += d*player.dirY * player.moveSpeed;
}
void Game::Turn(Directions dir) {
    int d;
    
    switch(dir) {
        case LEFT: d = 1; break;
        case RIGHT: d = -1; break;
        default: break;
    }

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

    //draw the pixels of the stripe as a vertical line
    for(int i = drawStart; i <= drawEnd; ++i) {
        if(i >= 0 && i <= screenHeight)
            col[i] = color;
    }
    return col;
}

void Game::UpdateScreen(std::vector<int> col, int x) {
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

        //length of ray from one x or y-side to next x or y-side
        //these are derived as:
        //deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
        //deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
        //which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
        //where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
        //unlike (dirX, dirY) is not 1, however this does not matter, only the
        //ratio between deltaDistX and deltaDistY matters, due to the way the DDA
        //stepping further below works. So the values can be computed as below.
        // Division through zero is prevented, even though technically that's not
        // needed in C++ with IEEE 754 floating point values.
        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; //was there a wall hit?
        int side; //was a NS or a EW wall hit?
        //calculate step and initial sideDist
        if(rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (player.posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
        }
        if(rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (player.posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
        }
        //perform DDA
        while(hit == 0)
        {
            //jump to next map square, either in x-direction, or in y-direction
            if(sideDistX < sideDistY)
            {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
            }
            else
            {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
            }
            //Check if ray has hit a wall
            if(map.map[mapX][mapY] > 0) hit = 1;
        }
        //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
        //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
        //This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
        //for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
        //because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
        //steps, but we subtract deltaDist once because one step more into the wall was taken above.
        if(side == 0) perpWallDist = (sideDistX - deltaDistX);
        else          perpWallDist = (sideDistY - deltaDistY);


        //INTERACTIVE ITEMS
        if(x == screenWidth / 2) {
            switch (map.map[mapX][mapY]) {
                case DOOR_LOCKED:
                    if(perpWallDist <= 1 && !keyHeld) {
                        lockedDoorMsg = true;
                    } else if(perpWallDist <= 1 && keyHeld) {
                        unlockedDoorMsg = true;
                        currDoorX = mapX;
                        currDoorY = mapY;
                        //TODO
                    }
                    break;
                case KEY:
                    if(perpWallDist <= 1 && !keyHeld) {
                        keyPicked = true;
                        currKeyX = mapX;
                        currKeyY = mapY;
                    }
                    break;
            }
        }

        //Calculate mapHeight of line to draw on screen
        int lineHeight = (int)(screenHeight / perpWallDist);
        if(lineHeight > screenHeight) lineHeight = screenHeight;

        //calculate lowest and highest pixel to fill in current line
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

        //give x and y sides different brightness
        int bold = 0;
        if(side == 1) {bold = 10;}

        //draw vertical line
        std::vector<int> col = PrepareNewCol(drawStart, drawEnd, color+bold);
        UpdateScreen(col,x);
    }
}

void Game::SetSpeed() {
    player.moveSpeed = fc.frameTime * 0.02; //the constant value is in squares/second
    player.turnSpeed = fc.frameTime * 0.01; //the constant value is in radians/second
}

void Game::SetTime() {
    fc.oldTime = fc.time;
    auto t = clock();
    fc.time = static_cast<double>(t);
    fc.frameTime = (fc.time - fc.oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
}

void Game::PrintScreen() {
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


void Game::Render() {
    RayCast();
    SetTime();
    SetSpeed();
    if(keyPicked && cycleNum <= 15) {
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
    if(lockedDoorMsg && cycleNum <= 15) {
        ih.LockedDoorMsg();
        cycleNum++;
        if(cycleNum == 16) {
            cycleNum = 0;
            lockedDoorMsg = false;
            PrintScreen();
       }
    }
    if(unlockedDoorMsg && cycleNum <= 15) {
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
    refresh();
}


void Game::ShowMap() {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            if((int)player.posX == i && (int)player.posY == j) { 
                mvaddstr(i + 10, j + 10, "X");
            } else {
                const char* c;
                switch(map.map[i*5][j*5]) {
                    case 0: c = "0"; break;
                    case 1: c = "1"; break;
                    case 2: c = "2"; break;
                    case 3: c = "3"; break;
                    case 4: c = "4"; break;
                    default: c = " "; break;
                }
                attron(COLOR_PAIR(4));
                mvaddstr(i + 10, j + 10, c);
                attroff(COLOR_PAIR(4));
                refresh();
            }
        }
    }
}
