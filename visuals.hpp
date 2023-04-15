#ifndef VISUALS_HPP_
#define VISUALS_HPP_

class InfoHandler {
    public:
        InfoHandler();
        InfoHandler(int w, int h);
        int screenWidth;
        int screenHeight;
        void KeyFoundMsg(); 
        void LockedDoorMsg();
        void UnlockedDoorMsg();
        int GetMsgStartX(int w);
        int GetMsgStartY(int h);
};

class MainMenu {
    public:
        MainMenu();
        MainMenu(int w, int h);
        int selected;
        bool inGame;
        int screenWidth;
        int screenHeight;
        void PrintMenu();
        void StartButton();
        void ResumeButton();
        void HelpButton();
        void ExitButton();
        void Down();
        void Up();
};

class HelpMenu {
    public:
        HelpMenu();
        HelpMenu(int w, int h);
        int screenWidth;
        int screenHeight;
        void PrintMenu();
};

#endif