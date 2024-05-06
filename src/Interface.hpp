#include <iostream>
#include "../include/raylib.h"

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

// We include the windows header in a namespace as to not collide with Raylib
using std::cout, std::endl;
namespace windows{
    #include "windows.h"
}

#include "Core.hpp"

class ItemList{ 
    private:
        // UI
        Rectangle Bounds = {0, 0, 0, 0}; Vector2 Scroll = {0, 0};
        Rectangle ViewBounds = {0, 0, 0, 0}; int Depth = 0;
        Rectangle View = {0, 0, 0, 0}; std::string Title;
        float Button_Height = 30; float Button_ID_Width = 75;
        float Vertical_Distance = 10; float Horizontal_Distance = 10;

        // Backend
        List* Items;
    public:
        void SetBounds(Rectangle _Bounds);
        void SetDepth(int _Depth);
        void SetTitle(std::string _Title);

        int SetData(List* Data);

        int Add_Item(Node NewUser);
        long Render();
};

class EditMenu{
    private:
    int Mode = 0; Rectangle Layout = {0, 0, 0, 0}; bool Ticked = false;
    float Button_Dimension = 15; string Content; bool State = false;
    char Content_Buffer[64] = {0};
    float Horizontal_Distance = 10;
    public:
    EditMenu(Rectangle _Layout, int _Mode, string _Content);
    void Set_Layout(Rectangle _Layout);
    void Set_Mode(int _Mode);
    void Set_Content(string _Content);
    void Set_State(bool _State);
    bool Get_Ticked();
    string Get_Content();
    int Render();
};

#endif