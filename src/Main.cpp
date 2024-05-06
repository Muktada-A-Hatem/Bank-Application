#include <iostream>
#include "../include/raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "../include/raygui.h"

// We include the windows header in a namespace as to not collide with Raylib
using std::cout, std::endl;
namespace windows{
    #include "windows.h"
}

// include other parts of the program
#include "Core.hpp"
#include "Interface.hpp"
#include "Global.hpp"

// Function definitions
void Setup();
void Run();
void Foo();
#ifdef DEBUG
void Debug();
#endif

// Program's constants
const float WinW = 960.0f;
const float WinH = 480.0f;
const Color WinColor = DARKGRAY;
const char* WinName = "Bank Application - Muktada A. Hatem";

// Main class that holds the data of the accounts
List Notebook;

// All new accounts start with this node than is edited later
Node Starter_Node{0, "EDIT NAME", "07700770077700", "EDIT@EMAIL!", "EDIT PROFESSION!", 25000, 0, "EDIT BANK!"};

long Current_Working_ID = 0;

// UI Elemetns
ItemList Accounts;
EditMenu Name_Menu({500, 35, 440, 50}, 0, "EDIT NAME");
EditMenu Email_Menu({500, 105, 220, 50}, 0, "EDIT@EMAIL!");
EditMenu Phone_Menu({730, 105, 210, 50}, 0, "07700770077700");
EditMenu Profession_Menu({500, 175, 220, 50}, 0, "EDIT PROFESSION!");
EditMenu Bank_Menu({730, 175, 210, 50}, 0, "EDIT BANK!");
EditMenu Deposit_Menu({500, 390, 100, 30}, 0, "AMOUNT");
EditMenu Withdraw_Menu({610, 390, 100, 30}, 0, "AMOUNT");
EditMenu Transfer_ID_Menu({720, 390, 80, 30}, 0, "ID");
EditMenu Transfer_Amount_Menu({810, 390, 130, 30}, 0, "AMOUNT");
EditMenu Annual_Years_Menu({500, 390, 100, 30}, 0, "YEARS");
EditMenu Annual_Percentage_Menu({610, 390, 100, 30}, 0, "%");

int FinanceMode = 0;
string Annual_Interest_Handle = "ANUAL INTEREST";

string Old_Name, Old_Email, Old_Phone, Old_Profession, Old_Bank;

int main(){
    // Load the window
    InitWindow(WinW, WinH, WinName);
    SetTargetFPS(60);
    SetExitKey(KEY_DELETE);

    // Change default font size          
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 2);

    // Load setup function
    Setup();

    while (!WindowShouldClose()){
        ClearBackground(WinColor);
        BeginDrawing();

        // Load the run function every frame
        Run();
        
        // Debug Code
        #ifdef DEBUG
        Debug();
        #endif

        EndDrawing();
    }

    // Close the window properly when exiting
    CloseWindow();
    return 0;
}


void Setup(){
    // This program uses Random number generator
    // To generate a proper number, a random seed is required
    // Here, we just input the current time as a seed
    windows::SYSTEMTIME CurrentTime;
    windows::GetSystemTime(&CurrentTime);
    SetRandomSeed   (((long long)CurrentTime.wYear  * 1000000000000000) + 
                    ((long long)CurrentTime.wMonth  * 1000000000000) + 
                    ((long long)CurrentTime.wDay    * 1000000000) + 
                    ((long long)CurrentTime.wHour   * 1000000) +
                    ((long long)CurrentTime.wMinute * 1000) +
                    ((long long)CurrentTime.wSecond));

    Accounts.SetBounds({0, 0, 480, 480});
    Accounts.SetDepth(10000);
    Accounts.SetTitle("Accounts:");
    Accounts.SetData(&Notebook);
}

void Run(){
    if (IsKeyReleased(KEY_HOME)) {Foo();}
    if (IsKeyReleased(KEY_PRINT_SCREEN)) {Notebook.Display();}

    // Accounts Panel
    long Running_ID;
    Running_ID = Accounts.Render();

    // Add button pressed (know from return)
    if (Running_ID == 1){
        Starter_Node.Mobile_Number = to_string(GetRandomValue(100, 10000000));
        Accounts.Add_Item(Starter_Node);
    } 
    
    // One of the accounts were selected
    else if (Running_ID > 10000){
        Current_Working_ID = Running_ID;

        Name_Menu.Set_State(1);
        Email_Menu.Set_State(1);
        Phone_Menu.Set_State(1);
        Profession_Menu.Set_State(1);
        Bank_Menu.Set_State(1);
    }
    
    // One of the accounts were deleted
    else if (Running_ID == 2){
        Current_Working_ID = 0;

        Name_Menu.Set_State(0);
        Email_Menu.Set_State(0);
        Phone_Menu.Set_State(0);
        Profession_Menu.Set_State(0);
        Bank_Menu.Set_State(0);
    }


    // Edit Panel
    GuiPanel({480, 0, 480, 240}, "Edit:");

    // // Name Menu
    if (Name_Menu.Get_Ticked() and Current_Working_ID > 10){
        if (Notebook.Edit_Name(Current_Working_ID, Name_Menu.Get_Content()) == -1){
            Name_Menu.Set_Content(Old_Name);   
        }
    }
    Old_Name = Name_Menu.Get_Content();
    Name_Menu.Render();

    // // Email Menu
    if (Email_Menu.Get_Ticked() and Current_Working_ID > 10){
        if (Notebook.Edit_Email(Current_Working_ID, Email_Menu.Get_Content()) == -1){
            Email_Menu.Set_Content(Old_Email);   
        }
    }
    Old_Email = Email_Menu.Get_Content();
    Email_Menu.Render();

    // // Phone Menu
    if (Phone_Menu.Get_Ticked() and Current_Working_ID > 10){
        if (Notebook.Edit_Phone_Number(Current_Working_ID, Phone_Menu.Get_Content()) == -1){
            Phone_Menu.Set_Content(Old_Phone);   
        }
    }
    Old_Phone = Phone_Menu.Get_Content();
    Phone_Menu.Render();

    // // Profession Menu
    if (Profession_Menu.Get_Ticked() and Current_Working_ID > 10){
        if (Notebook.Edit_Profession(Current_Working_ID, Profession_Menu.Get_Content()) == -1){
            Profession_Menu.Set_Content(Old_Profession);   
        }
    }
    Old_Profession = Profession_Menu.Get_Content();
    Profession_Menu.Render();

    // // Bank Menu
    if (Bank_Menu.Get_Ticked() and Current_Working_ID > 10){
        if (Notebook.Edit_Bank_Name(Current_Working_ID, Bank_Menu.Get_Content()) == -1){
            Bank_Menu.Set_Content(Old_Bank);   
        }
    }
    Old_Bank = Bank_Menu.Get_Content();
    Bank_Menu.Render();

    // Always show the selected account
    if (Current_Working_ID > 1000 and !Name_Menu.Get_Ticked()) {Name_Menu.Set_Content(Notebook.Search(Current_Working_ID)->Name);}
    if (Current_Working_ID > 1000 and !Email_Menu.Get_Ticked()) {Email_Menu.Set_Content(Notebook.Search(Current_Working_ID)->Email);}
    if (Current_Working_ID > 1000 and !Phone_Menu.Get_Ticked()) {Phone_Menu.Set_Content(Notebook.Search(Current_Working_ID)->Mobile_Number);}
    if (Current_Working_ID > 1000 and !Profession_Menu.Get_Ticked()) {Profession_Menu.Set_Content(Notebook.Search(Current_Working_ID)->Profession);}
    if (Current_Working_ID > 1000 and !Bank_Menu.Get_Ticked()) {Bank_Menu.Set_Content(Notebook.Search(Current_Working_ID)->Bank_Name);}

    // Finance Panel
    GuiPanel({480, 240, 480, 240}, "Finance:");

    if (Current_Working_ID > 10){
        GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(BLUE));
        GuiButton({500, 275, 440, 60}, to_string(Notebook.Search(Current_Working_ID)->Amount).data());
        GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(DARKGRAY));

        if (GuiButton({500, 350, 100, 30}, "Deposit"))              {FinanceMode = 1;}
        else if (GuiButton({610, 350, 100, 30}, "Withdraw"))        {FinanceMode = 2;}
        else if (GuiButton({720, 350, 220, 30}, "Transfer"))        {FinanceMode = 3;}
        else if (GuiButton({500, 430, 440, 30}, "Annual Interest")) {FinanceMode = 4;}
        
        // Financial editing UI behaviour
        switch(FinanceMode){
            case(1):
                Deposit_Menu.Set_State(1);
                if (Deposit_Menu.Render()){
                    if (Deposit_Menu.Get_Content().find_first_not_of("0123456789") != 0){
                        Notebook.Deposite(Current_Working_ID, atoi(Deposit_Menu.Get_Content().data()));
                        Deposit_Menu.Set_State(0);
                        Withdraw_Menu.Set_Content("AMOUNT");
                        FinanceMode = 0;
                    } else {Error("Wrong Amount");}
                }
                break;
            case(2):
                Withdraw_Menu.Set_State(1);
                if (Withdraw_Menu.Render()){
                    if (Withdraw_Menu.Get_Content().find_first_not_of("0123456789") != 0){
                        Notebook.Withdrawal(Current_Working_ID, atoi(Withdraw_Menu.Get_Content().data()));
                        Withdraw_Menu.Set_State(0);
                        Withdraw_Menu.Set_Content("AMOUNT");
                        FinanceMode = 0;
                    } else {Error("Wrong Amount");}
                }
                break;
            case(3):
                Transfer_Amount_Menu.Set_State(1);
                Transfer_ID_Menu.Set_State(1);
                Transfer_ID_Menu.Render();
                if (Transfer_Amount_Menu.Render()){
                    if (Transfer_Amount_Menu.Get_Content().find_first_not_of("0123456789") != 0){
                        if (Transfer_ID_Menu.Get_Content().find_first_not_of("0123456789") != 0){
                            Notebook.Transfer(Current_Working_ID, atoi(Transfer_ID_Menu.Get_Content().data()), atoi(Transfer_Amount_Menu.Get_Content().data()));
                            Transfer_Amount_Menu.Set_State(0); Transfer_ID_Menu.Set_State(0);
                            Transfer_Amount_Menu.Set_Content("AMOUNT"); Transfer_ID_Menu.Set_Content("ID");
                            FinanceMode = 0;
                        } else
                        Error("Wrong ID");
                    } else
                    Error("Wrong Amount");
                }
                break;
            case(4):
                Annual_Years_Menu.Set_State(1);
                Annual_Percentage_Menu.Set_State(1);
                Annual_Years_Menu.Render();
                if (Annual_Percentage_Menu.Render()){
                    Annual_Interest_Handle = to_string(Notebook.Annual_Interest(
                    Current_Working_ID, atoi(Annual_Years_Menu.Get_Content().data()),
                    atoi(Annual_Percentage_Menu.Get_Content().data()))).data();
                }
                GuiSetStyle(BUTTON, BORDER, ColorToInt(BLUE));
                GuiButton({720, 390, 220, 30}, Annual_Interest_Handle.data());
                GuiSetStyle(BUTTON, BORDER, ColorToInt(DARKGRAY));
                break;
        }

        if (IsKeyReleased(KEY_ESCAPE)) {FinanceMode = 0;}
    }

}

void Foo(){
    cout << endl << "--------------------------------------------------------------------";
    cout << endl << "--------------------------------------------------------------------" << endl;

    List Testbook;

    Node Test{0, "Testing Name", "1010101", "Testing@Email.com", "A Test", 25000, 0, "Testing Bank"};
    Node SecondTest{0, "Second Testing Name", "2020202", "SecondTesting@Email.com", "A Second Test", 129481, 0, "Second Bank"};

    cout << endl << "ADD" << endl;
    cout << "FIRST ID: " << (Test.ID = Testbook.Add(Test)) << endl;
    cout << "SECOND ID: " << (SecondTest.ID = Testbook.Add(SecondTest)) << endl;
    Testbook.Display();

    cout << endl << "EDIT" << endl;
    Testbook.Edit_Name(Test.ID, "Edited Testing Name");
    Testbook.Edit_Email(Test.ID, "Edited@Email.com");
    Testbook.Edit_Phone_Number(Test.ID, "3030303");
    Testbook.Edit_Profession(Test.ID, "Edited Profession");
    Testbook.Display();

    cout << endl << "DEPOSIT 10000" << endl;

    Testbook.Deposite(Test.ID, 10000);
    Testbook.Display();

    cout << endl << "WIHTDRAW 5000"<< endl;

    Testbook.Withdrawal(Test.ID, 5000);
    Testbook.Display();

    cout << endl << "TRANSFER 10000" << endl;

    Testbook.Transfer(Test.ID, SecondTest.ID, 10000);

    cout << endl << "ANNUAL INTEREST OF TWO YEARS WITH 10%" << endl;

    cout << Testbook.Annual_Interest(Test.ID, 2, 10) << endl;
    cout << Testbook.Annual_Interest(SecondTest.ID, 2, 10) << endl;

    cout << endl << "REMOVE SECOND NODE" << endl;

    Testbook.Remove(SecondTest.ID);
    Testbook.Display();

    cout << endl << "--------------------------------------------------------------------";
    cout << endl << "--------------------------------------------------------------------" << endl;
    
}

// Debug functions
#ifdef DEBUG
void Debug(){
    cout << "MOUSE X, Y: " << GetMousePosition().x << ", " << GetMousePosition().y << endl;
    DrawFPS(0, 0);
}
#endif