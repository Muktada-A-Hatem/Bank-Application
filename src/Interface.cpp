#include <iostream>
#include <cstring>
#include "../include/raylib.h"
#include "../include/raygui.h"
// #include "clipboardxx.hpp"

// We include the windows header in a namespace as to not collide with Raylib
using std::cout, std::endl;
namespace windows{
    #include "windows.h"
}

// include other parts of the program
#include "Interface.hpp"
#include "Global.hpp"

//--------------------------------------------------------------------
//Item List Class
//--------------------------------------------------------------------

void ItemList::SetBounds(Rectangle _Bounds){
    Bounds = _Bounds;
}

void ItemList::SetDepth(int _Depth){
    Depth = _Depth;
}

void ItemList::SetTitle(std::string _Title){
    Title = _Title;
}

int ItemList::SetData(List* Data){
    Items = Data;
    return 0;
}

int ItemList::Add_Item(Node NewUser){
    Items->Add(NewUser);
    return 0;
}

Node* Cursor = NULL; int Count = 0; int Current_Y_Position = 0;
Rectangle CurrentRect_ID;
Rectangle CurrentRect_Name;
Rectangle CurrentRect_X;
int Dint = 0;

long ItemList::Render(){
    ViewBounds.height = Bounds.height + Depth - RAYLIB_HEADER_HEIGHT;
    GuiScrollPanel(Bounds, Title.data(), ViewBounds, &Scroll, &View);
    BeginScissorMode(Bounds.x, Bounds.y + RAYLIB_HEADER_HEIGHT, Bounds.width, Bounds.height);
    ClearBackground(RAYWHITE);

    // Draw all the items

    Count = 0; Cursor = Items->Get_Head();
    
    // Draw the ADD button
    if (GuiButton({ Bounds.x + Horizontal_Distance, 
                    Bounds.y + RAYLIB_HEADER_HEIGHT + Vertical_Distance + Scroll.y +
                    ((Button_Height + Vertical_Distance) * Items->ITEM_COUNT),
                    Bounds.width - (2*Horizontal_Distance) - RAYLIB_SCROLL_WIDTH,
                    Button_Height},
                    "+")) { return 1; }

    while (Cursor != NULL){
        // Only render when items are available on the screen
        Current_Y_Position =    Bounds.y + RAYLIB_HEADER_HEIGHT + Vertical_Distance + Scroll.y + 
                                (Button_Height + Vertical_Distance) * Count; 

        if (Current_Y_Position < 480 and Current_Y_Position > -40){

            CurrentRect_ID = {
                Bounds.x + Horizontal_Distance,
                Bounds.y + RAYLIB_HEADER_HEIGHT + Vertical_Distance + Scroll.y +
                ((Button_Height + Vertical_Distance) * Count),
                Button_ID_Width,
                Button_Height
            };

            CurrentRect_Name = {
                Bounds.x + (Horizontal_Distance*2) + Button_ID_Width,
                Bounds.y + RAYLIB_HEADER_HEIGHT + Vertical_Distance + Scroll.y +
                ((Button_Height + Vertical_Distance) * Count),
                Bounds.width - (4*Horizontal_Distance) - RAYLIB_SCROLL_WIDTH - Button_Height - Button_ID_Width,
                Button_Height
            };

            CurrentRect_X = {
                Bounds.width - Button_Height - (Horizontal_Distance) - RAYLIB_SCROLL_WIDTH,
                Bounds.y + RAYLIB_HEADER_HEIGHT + Vertical_Distance + Scroll.y +
                ((Button_Height + Vertical_Distance) * Count),
                Button_Height,
                Button_Height
            };

            // Draw the ID button (Copies ID to clipboard when selected)
            if (GuiButton(CurrentRect_ID, to_string(Cursor->ID).data())){
                // Copy the ID to clipboard
                char* output = to_string(Cursor->ID).data();
                size_t len = strlen(output) + 1;
                windows::HGLOBAL hMem =  windows::GlobalAlloc(GMEM_MOVEABLE, len);
                memcpy(windows::GlobalLock(hMem), output, len);
                windows::GlobalUnlock(hMem);
                windows::OpenClipboard(0);
                windows::EmptyClipboard();
                windows::SetClipboardData(CF_TEXT, hMem);
                windows::CloseClipboard();
            }

            // Draw the account (Selectes account when selected)
            if (GuiButton(CurrentRect_Name, Cursor->Name.data())) {
                return Cursor->ID;
            }
            
            GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(RED));
            // Draw the X button (Deletes account when selected)
            if (GuiButton(CurrentRect_X, "X")) {
                Items->Remove(Cursor->ID);
                GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(DARKGRAY));
                return 2;
            }
            GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(DARKGRAY));
        }

        Cursor = Cursor->Link;
        Count++;
    }
    
    EndScissorMode();
    return 0;
}

//--------------------------------------------------------------------
//Edit Menu Class
//--------------------------------------------------------------------

EditMenu::EditMenu(Rectangle _Layout, int _Mode, string _Content){
    Layout = _Layout;
    Mode = _Mode;
    Content = _Content;
}

void EditMenu::Set_Layout(Rectangle _Layout){
    Layout = _Layout;
}

void EditMenu::Set_Mode(int _Mode){
    Mode = _Mode;
}

void EditMenu::Set_Content(string _Content){
    Content = _Content;
}

void EditMenu::Set_State(bool _State){
    State = _State;
}

bool EditMenu::Get_Ticked(){
    return Ticked;
}

string EditMenu::Get_Content(){
    return Content;
}

int EditMenu::Render(){
    // Normal Button
    if (Mode == 0 and State){
        if (GuiButton(Layout, Content.data())) {Mode = 1; return 0;}
        else {Ticked = false; return 0;}
    } 

    // Edit Mode
    else if (Mode == 1){
        GuiTextBox(Layout, Content_Buffer, 64, State);

        if (IsKeyDown(KEY_LEFT_CONTROL) and IsKeyPressed(KEY_V)){
                windows::OpenClipboard(windows::GetDesktopWindow());
                windows::HGLOBAL hg = windows::GetClipboardData(CF_TEXT);
                windows::LPCSTR strData =(windows::LPCSTR)windows::GlobalLock(hg);
                windows::GlobalUnlock(hg);
                windows::CloseClipboard();
                Set_Content((string)strData);
                Mode = 0;
                return 1;
        }

        if (IsKeyReleased(KEY_ENTER)) { 
            Ticked = true;
            Content = Content_Buffer;
            fill_n(Content_Buffer, 64, NULL);
            Mode = 0;
            return 1;
            }
        else if (IsKeyReleased(KEY_ESCAPE)) {Mode = 0; return 0; Ticked = false;}
        else { Ticked = false; return 0;}

        // GuiTextInputBox(Layout, Content.data(), Content_Buffer, Content, , 1);
    }

    return -1;
}