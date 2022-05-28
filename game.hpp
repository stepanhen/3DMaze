#ifndef GAME_HPP_
#define GAME_HPP_

#include <vector>
#include "visuals.hpp"
#include "levels.hpp"

#define mapWidth 10
#define mapHeight 10

#define BACKGROUND 0
#define WALL 1
#define DOOR_LOCKED 2
#define KEY 3

enum Directions {
    FRONT,
    BACK,
    LEFT,
    RIGHT
};

class Map {
    public:
        Map();
        int map[mapWidth][mapHeight];
        void ChangeMap(std::vector<std::vector<int>> maze);
};

class Player {
    public:
        Player();
        double posX;
        double posY;
        double dirX;
        double dirY;
        double planeX;
        double planeY;
        double moveSpeed;
        double turnSpeed;
};

class FrameCounter {
    public:
        FrameCounter();
        double time; //time of current frame
        double oldTime; //time of previous frame
        double frameTime;
};

class Game {
    public:
        Game();
        Game(int x, int y);
        void Render();
        void Move(Directions dir);
        void Turn(Directions dir);
        void ShowMap();
    private:
        FrameCounter fc;
        Player player;
        Map map;
        InfoHandler ih;
        bool keyHeld;
        bool keyPicked;
        bool lockedDoorMsg;
        bool unlockedDoorMsg;
        bool changeKeyWall;
        int currKeyX;
        int currKeyY;
        int currDoorX;
        int currDoorY;
        int screenHeight;
        int screenWidth;
        int cycleNum;
        int level;
        std::vector<std::vector<int>> screen;
        std::vector<int> PrepareNewCol(int s, int e, int c);
        void UpdateScreen(std::vector<int> c, int x);
        void RayCast();
        void SetTime();
        void SetSpeed();
        void PrintScreen();
};     


#endif