#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct orderItems
{
    int id;
    string name;
    double price;
};

struct Node
{
    Order data;
    Node* next;
}

class User
{
    protected:
       string username;
       string password;

    public:
      User() { username = " "; password = " ";}
      virtual void login() = 0;
      virtual ~User() {};
};

class Customer : public User {
    private: 
       Node* head;
    public: 
       Customer() : head(NULL) {}

    void search(int id)
    {
        Node* current = head;
        while* (current != NULL) {
            if (current-> data.ID == id)
            {
            cout << "Order Found: " << current-> data.name << endl;
            return;
            }
            current = current -> next;
        }
        cout << "Order ID not found." << endl;      

    }
};

int main()
{
    int choice;
    Customer customerSel;

    while (true)
    {
        cout << "============================" << endl;
        cout << "     WELCOME TO KARABU      " << endl;
        cout << "============================" << endl;
        cout << "1. LOGIN    " << endl;
        cout << "2. REGISTER " << endl;
        cout << "3. ADMIN    " << endl;
        cout << "4. STAFF    " << endl;
        cout << "1. EXIT     " << endl;
        cout << "Please enter your selection: ";

        try{
           
            if( cin >> choice)
            {
                throw "Invalid selection! Please re-enter your selection:";
            }

            if(choice == 1)
            {
                customerSel.login();
            }
            else if(choice == 2)
            {
                customerSel.register();
            }
            else if(choice == 3)
            {
                cout << amdin.login();
            }
            else if(choice == 4)
            {
                cout << staff.login();
            }
            else if(choice == 5)
            {
                cout << "Exiting system. Thank you for using KARABU!" << endl;
            }
            else
            {
                cout << "Selectipn not available. Please re-enter your selection: " << endl;

            }
        }
        catch(const char* error)
        {
            cout << "Error: " << error << endl;
            cin.clear();
            cin.ignore(100, '\n');
        }
        cout << endl;
    }
    return 0;
}


