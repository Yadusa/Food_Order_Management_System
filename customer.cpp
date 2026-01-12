#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <sstream>
#include <iomainip>
#include <cctype> // for toupper()
#include <windows.h>
#include <stdexcept> // for try catch()
#include <map>


using namespace std;

struct menuItem
{
    int menuId;
    string menuName;
    float menuPrice;
    menuItem* next;

}

menuItem* itemHead = NULL;

struct orderItems
{
    int orderId;
    string ordeName;
    double orderPrice;
    int orderQuantity;
    float total;
    OrderItem* next;
};

orderItem* itemHead = NULL;

//declaration of sorting functions
void bubbleSortByPrice(menuItem*& head);
void bubbleSortByName(menuItem*& head);
void displaySalesReport();
void OrderMenu();


//Base class
class User
{
    protected:
        string  id,username, password;
    public: 
       user(string id = "", string username = "", string password = "")
       : id(id), username(username), password(password) {}

       string getUsername() const{
        return username;
       }

       string getId() const {
        return id;
       }
    
    virtual voi login() = 0; 
    virtual ~User() {}
};

class Menu{

    public: 
      Menu()
      {
        itemHead = NULL;
        ifstream file(menu.txt);

        if(!file.is.open()){
            cout << "No menu.txt found"c<< endl;
            return;
        }
        
        // converting to string 
        string line;
        while(getline(file, line)) {
            stringstream ss(line); // slipt line commas
            string idStr,name, priceStr;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, price, ',');

            // convert back to int and float
            int id = stoi(idStr);
            float price = stof(priceStr);

            menuItem* newItem = new menuItem;
            newItem -> id =id;
            newItem -> name = name;
            newItem -> price = price;
            newItem -> next = NULL;

            if (itemHead == NULL)
            {
                itemHead = newItem;
            }
            else 
            {
                menuItem* current = itemHead;
                while(current -> next != NULL)
                {
                    current = current -> next;
                }
                current -> next = newItem;
            }
        }
        file.close();
      }
      
}

int main()
{
    int choice;

    while (true)
    {
        cout << "==================================================" << endl;
        cout << "     WELCOME TO KARABU FOOD ORDERING SYSTEM       " << endl;
        cout << "==================================================" << endl;
        cout << "                    1. CUSTOMER                   " << endl;
        cout << "                    2. ADMIN                      " << endl;
        cout << "                    3. EXIT                       " << endl;
        cout << "Please enter your selection: ";

        try{

            if(!(cin >> choice) )
            {
                throw "Invalid selection! Please re-enter your selection:";
            }

            if(choice == 1)
            {
                customer.login();
            }
            else if(choice == 2)
            {
                admin.login();
            }
            else if(choice == 3)
            {
                cout << "Exiting system. Thank you for using KARABU!" << endl;
            }
            else
            {
                cout << "Invalid selection. Please re-enter your selection: " << endl;

            }
        }
        catch(const char* error)
        {
            cout << "Error: " << error << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cout << "Exiting program..." << endl;
    }
    cout << "Exiting program..." << endl;
}


