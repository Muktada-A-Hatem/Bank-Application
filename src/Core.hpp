#include <iostream>

#ifndef CORE_HPP
#define CORE_HPP

using namespace std;

struct Node{
    long ID                          = 0;
    string Name                      = "";
    string Mobile_Number             = "";
    string Email                     = "";
    string Profession                = "";
    unsigned long Amount             = 0;
    bool Account_Type                = 0;
    string Bank_Name                 = "";
    Node *Link                       = NULL;
};

class List{
private:
    Node *Tail = NULL; Node *Head = NULL;
public:
    long ITEM_COUNT = 0;

    Node* Get_Head();
    Node *Search(long _ID);
    void Display();
    int Validate(Node* TestNode, bool New);
    long Add(Node NewUser);

    int Edit_Name(long _ID, string _Name);
    int Edit_Phone_Number(long _ID, string _Mobile_Number);
    int Edit_Email(long _ID, string _Email);
    int Edit_Profession(long _ID, string _Profession);
    int Edit_Bank_Name(long _ID, string _Bank_Name);
    
    long Remove(long ID_);
    void Deposite(long _ID, long Cash);
    void Withdrawal(long _ID, long Cash);
    void Transfer(long From_ID, long To_ID, long Amount);
    long Annual_Interest(int ID_, int Years, short Percentage);
};
#endif