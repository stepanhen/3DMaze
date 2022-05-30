#ifndef GAME_HPP_
#define GAME_HPP_

#include <vector>
#include "visuals.hpp"
#include "levels.hpp"

//default size of the map
#define mapWidth 10
#define mapHeight 10

//color bindings
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
        //position of the player
        double posX; 
        double posY; 
        //direction of the player
        double dirX; 
        double dirY; 
        //camera plane of the player
        double planeX; 
        double planeY;
        double moveSpeed;
        double turnSpeed;
};

class FrameCounter { 
    //this class is used for balancing the fps of the game
    public:
        FrameCounter();
        double time; //time of current frame
        double oldTime; //time of previous frame
        double frameTime;
        void SetTime();
};

class Game {
    public:
        Game();
        Game(int x, int y);
        void Render(); //brings together raycasting, printing and game mechanics
        void Move(Directions dir); //moves with player
        void Turn(Directions dir); //rotates the player
        void PrintScreen();
    private:
        FrameCounter fc;
        Player player;
        Map map;
        InfoHandler ih;
        bool keyHeld; //if player holds a key
        bool keyPicked; //if key was picked
        bool lockedDoorMsg; //if locked door message should be drawn
        bool unlockedDoorMsg; //if unlocked door message should be drawn
        bool changeKeyWall; //if key wall should be changed to normal wall
        //position of key wall
        int currKeyX;
        int currKeyY;
        //position of door wall
        int currDoorX;
        int currDoorY;
        int screenHeight;
        int screenWidth;
        int cycleNum;
        int level;
        std::vector<std::vector<int>> screen; //represents what is drawn on the screen
        std::vector<int> PrepareNewCol(int s, int e, int c);
        void UpdateScreen(std::vector<int> c, int x); //gets new column and updates new screen parts
        void RayCast(); //raycasting function
        void SetSpeed(); //sets moving and rotating speed acording fps
        void GameMechanics();
};     


#endif