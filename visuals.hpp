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
        void StartButton(int x, int y);
        void HelpButton(int x, int y);
        void ExitButton(int x, int y);
        void BackButton(int x, int y);
};

#endif