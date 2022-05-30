# 3DMaze

3DMaze is a maze game in console with raycasting. 
I recommend to play it in a fullscreen console.
Move with WASD keys.

At the start of the game there is a main menu where the player can select either start, help or exit option.
By selecting the **help** option help menu appears and shows the player what keys he is supposed to use.
By selecting the **exit** option the game ends.
By selecting the **start** option the game starts. Player can anytime return to the main menu by pressing **P** and resume the game again by selecting **start**.

The objective of the game is to find keys and unlock doors to get to the next level.

The program consists of four main files. **maze**, **game**, **levels** and **visuals**.

## maze.cpp

The maze.cpp file is the main file that has the main function. The main function contains screen initialization methods and big while loop in which the game runs.

## game.cpp 

This is the main file that takes care of game logic and screen rendering. It contains the following classes:
- **Map** : representation of the game map
- **Player** : describes the players position, his direction, move speed and turn speed
- **FrameCounter** : measures the time spend on processing and helps balance the fps
- **Game** : this is the main class that puts everything together. It has few methods like:
    - **Move** and **Turn** - player moving and turning
    - **PrintScreen** - is used to print the whole screen buffer
    - **UpdateScreen** - is used more than PrintScreen because it prints only the parts that changed and therefore it is more effective and it prevents screen flickering
    - **GameMechanics** - works with interactive items in the game
    - **RayCast** - finally the biggest function that creates the whole *3D look*

The **RayCast** functions creates a ray for every column on the screen. 

Then it calculates the ray position and direction.
CameraX is the x coordinate of the camera plane, right side of the screen will get coordinate 1, the center of the screen gets coordinate 0, and the left side of the screen gets coordinate -1.

```
cameraX = 2 * x / (double)screenWidth - 1;
rayDirX = player.dirX + player.planeX * cameraX;
rayDirY = player.dirY + player.planeY * cameraX;
```

Then it calculates the length of a ray from one x or y side to the next x or y side.
This is a simplified pythagoras formula.

```
deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
```

This part sends the ray and waits for it to hit a wall.

```
while(hit == 0) {
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
            if(map.map[mapX][mapY] > 0) hit = 1;
        }
```

The ray length can be calculated the following way. The length of sideDist already almost equals perpWallDist. In the while loop we went one step further to get inside the wall so we need to subtract deltaDist from it.

```
if(side == 0) perpWallDist = (sideDistX - deltaDistX);
else          perpWallDist = (sideDistY - deltaDistY);
```

Later we just calculate the line height of the vertical line we are going to draw and that is all.

```
int lineHeight = (int)(screenHeight / perpWallDist);
if(lineHeight > screenHeight) lineHeight = screenHeight;
```

## visuals.cpp

This file takes care of printing all the labels and menus in the game. It consists of:
- **InfoHandler** : which shows information about the game progress to the player like key found etc.
- **MainMenu** : this class prints the main menu and holds the information about selected option
- **HelpMenu** : this class prints game instructions

## levels.cpp

This file only contains 2D lists of ints that represent maze for each level. 0 stands for nothing, 1 for wall, 2 for door and 3 for a key.