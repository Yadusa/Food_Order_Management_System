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
            newItem -> menuId =id;
            newItem -> menuName = name;
            newItem -> menuPrice = price;
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

      string tolower(string str)
      {
        for(int i = 0; i < str.length(); i++)
        {
            if(str[i] >= 'A' && str[i] <= 'Z')
            {
                str[i] = str[i] + 32;
            }
        }
        return str;
      }
      
      int getMeuArray(menuItem* arr[], int maxSize)
      {
        int count = 0;
        menuItem* current = itemHead;
        while(current != NULL && count < maxSize )
        {
            arr[count++] = current;
            current = current -> next;
        }
        return count;
      }

      void inlineSortMenuById(menuItem* arr[], int n)
      {
        for(int i = 0; i < n - 1; i++)
        {
            for(int j = 0; j < n - i; j++)
            {
                if (arr[j] -> menuId > arr[j + 1] -> menuId)
                {
                    menuItem* temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
      }

      menuItem* binarySearchById(menuItem* arr[], int left, int left, int right, int targetId)
      {
        if(right >= left)
        {
            int mid = left + (right - left) / 2;
            if(arr[mid] -> menuId == targetId)
            {
                return arr[mid];
            }
            if(arr[mid] -> menuId > targetId)
            {
                return binarySearchById(arr, left, mid - 1, targetId);
            }
            return binarySearchById(arr, mid + 1, right, targetId);
        }
        return NULL;
      }

      menuItem* searchById(int id)
      {
        counst int MAX = 100;
        menuItem* arr[MAX];
        int count = getMenuArr(arr, MAX);
        inlineSortMenuById(arr, 0, count - 1, id);
        return binarySearchById(arr, 0, count - 1, id);
      }

      void searchByName(const string& searchName)
      {
        string lowerSearchName = toLower(searchName);
        menuItem* current = itemHead;
        bool found = false;

        cout << "=====================================" << endl;
        cout << "\n            Search Results for     " << searchName << " : " << endl;
        cout << "=====================================" << endl;
        cout << " ID | Item Name       | Price(RM)    " << endl;
        cout << "-------------------------------------" << endl;

        while(current != NULL)
        {
            string lowerMenuName = toLower(current -> name);

            if(lowerMenuName.find(lowerSeachName) != string::npos)
            {
                cout << "|" << setw(2) << current -> menuId << "|"
                     << setw(18) << left << current -> menuName << "|"
                     <<setw(12) << right << fixed << setprecision(2) << current -> menuPrice << "|" << endl;
                
               found = true;
            }

            current = current -> next;
        }

        if(!found)
        {
            cout << "No matching items found! " << searchName << "" << endl;
        }
        cout << "-------------------------------------" << endl;
      }

      void displaySeachMenu()
      {
        cout << "\n Search Menu by: \n";
        cout << "1.Menu ID\n";
        cout << "2. Menu Name \n";
        cout << "3. Back \n";
        cout << "Enter your choice: ";
        string searchChoice;
        getline(cin, searchChoice);
        
        if(searchChoice == "3")
        {
            return;
        }
        if(searchChoice == "1")
        {
            cout << "Enter the Menu ID to seach: ";
            int searchId;
            cin >> searchId;
            cin.ignore();


            menuItem* ound = searchById(searchId);
            cout << "\n=======================================" << endl;
            cout << "             Search Results              " << endl;
            cout << "=========================================" << endl;
            cout << " ID | Item Name       | Price(RM)        " << endl;
            cout << "-----------------------------------------" << endl;
        if(found)
        {
            cout << "|" << setw(2) << current -> menuId << "|"
                     << setw(18) << left << current -> menuName << "|"
                     <<setw(12) << right << fixed << setprecision(2) << current -> menuPrice << "|" << endl;
        }
        else 
        {
            cout << "---------------------------------" << endl;
        }
        else if(searchChoice == "2")
        {
            cout << "Enter item name to search: "
            string searchName;
            getline(cin, searchName);
            searchByName(searchName);
        }
        else
        {
            cout << "Incorrect choice. \n";
        }

        cout << "Press enter to continue.... ";
        cin.get();
      }

      void displayMenu()
      {
        cout << "=====================================" << endl;
        cout << "               MENU                  " << endl;
        cout << "=====================================" << endl;
        cout << "-------------------------------------" << endl;
        cout << " ID | Item Name      | Price(RM)     " << endl;
        cout << "--------------------------------- ---" << endl;

        menuItem* co=urrrent = itemHead;

        while(curret !NULL)
        {
            cout << "|" << setw(2) << current -> menuId << "|"
                     << setw(18) << left << current -> menuName << "|"
                     <<setw(12) << right << fixed << setprecision(2) << current -> menuPrice << "|" << endl;
            current = current -> next; 
        }

        cout << "------------------------------------------------" << endl;
      }

      void addMenuItem(int id, const string& name, floar price)
      {
        menuItem* newnode = new menuItem;
        newNode -> menuId = id;
        newNode -> menuName = name;
        newNode -> menuPrice = price;
        newNode -> next = NULL;

        if (itemHead == NULL)
        {
            itemHead = newNode;
        }
        else
        {
            menuItem* current = itemHead;
            while(current -> next !NULL)
            {
                current = current -> next;
            }
            current -> next = newNode;
        }
      }

      void editMenuItem(int id, string newName, float newPrice)
      {
        menuItem* current = itemHead;

        while(current !NULL)
        {
            if(curent -> menuId == id)
            {
                current -> menuName = name;
                current -> menuPrice = price;
                cout << "Menu updated successfully!" << endl;
                return;
            }
            current = current -> next;
        }
        cout << "Menu Item not found!" << endl;
      }

      void deleteMenuItem(int id)
      {
        if(itemHead == NULL)
        {
            cout << "The menu is empty!" << endl;
            return;
        }

        if(itemHead -> menuId = id)
        {
            menuItem* tenp = itemHead;
            itemHead = itemHead -> next;
            delete temp;
            cout << "Menu item has been deleted successfully!" << endl;
            return;
        }

        menuItem* current = itemHead;
        while(current -> next != NULL && current -> next -> menuId != id)
        {
            current = current -> next;
        }

        if(current -> next != NULL)
        {
            menuItem* temp = current -> next;
            current -> next = temp -> next;
            delete temp;
            cout << "Menu item has been deleted successfully!" << endl;
        }
        else 
        {
            cout << "Menu item not found!" << endl;
        }
      }

      bool ExistId(int id)
      {
        menuItem* current = itemHead;
        while(current !=NULL)
        {
            if(current -> menuId == id)
            {
                return true;
            }
            current = current -> next;
        }
        return false;
      }

      void getNewMenuItem()
      {
        int id;
        string name;
        float price;

        cout << "Enter Menu Id: ";
        cin >> id;

        if(ExistId(id))
        {
            cout << "Menu Id already exist!" << endl;
            return;
        }

        cout << "Enter Menu Name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Enter Menu Price: ";
        cin >> price;

        addMenu(id, title, price);
        cout << "Menu item added successfully! " << endl;
      }

      void getEditMenuItem()
      {
        int id;
        string newName;
        float newPrice;

        cout << "Enter Menu ID to edit: ";
        cin >> id;

        if(!ExistId(id))
        {
            cout << "Menu Item not foun! " << endl;
            return;
        }

        cout << "Enter new Name: ";
        cin.ignore();
        getline(cin, newName);

        cout << "Enter new Price: ";
        cin >> newPrice;

        editMenu(id, newName, newPrice);
      }

      void getdelMenuItem()
      {
        int id;
        cout << "Enter Menu Item to delete: ";
        cin >> id; 
        deleteMenu(id);
      }
};

class Customer: public User
{
    public: 
       Customer(string id= " ", string username = " ", string password = " ")
       : User(id, username, password)
       {
        if (!id.empty())
        {
            cout << "\n Logging out customer: " << getUsername() << endl;
            cout << "Thank you for choosing KARABU!" << endl;
            cout << "Press enter to continue...";
            cin.get();
        }
       }

        friend void bubbleSortByPrice(menuItem*&);
        friend voif bubbleSortByName(menuItem*&);
};


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


