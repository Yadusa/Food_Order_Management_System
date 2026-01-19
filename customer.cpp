#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <windows.h>
#include <stdexcept>

using namespace std;

// ============================
// Linked List Structures
// ============================
struct menuItem
{
    int menuId;
    string menuName;
    float menuPrice;
    menuItem* next;
};

menuItem* itemHead = NULL;

struct orderItem
{
    int orderId;
    int menuId;
    string orderName;
    double orderPrice;
    int orderQuantity;
    double total;
    orderItem* next;
};

// Order list head (separate from menu head)
orderItem* orderHead = NULL;

// ============================
// Forward Declarations
// ============================
class Menu;
class Customer;
class Admin;

void customerMenu(Customer& customer);

// Order functions
void OrderMenu(Menu& menuItems, orderItem*& orderHead);
void addOrderItem(Menu& menuItems, orderItem*& orderHead);
void viewOrderSummary(orderItem* orderHead);
void editOrderQuantity(orderItem* orderHead);
void deleteOrderLine(orderItem*& orderHead);
void confirmOrderToFile(orderItem*& orderHead);
double calculateGrandTotal(orderItem* orderHead);
menuItem* findMenuById(int id);
int getNextOrderId(orderItem* head);

// ============================
// Base class
// ============================
class User
{
protected:
    string id, username, password;

public:
    User(string id = "", string username = "", string password = "")
        : id(id), username(username), password(password) {}

    string getUsername() const { return username; }
    string getId() const { return id; }

    virtual void login() = 0;
    virtual ~User() {}
};

// ============================
// Menu Class
// ============================
class Menu
{
public:
    Menu()
    {
        // clear old list if any
        itemHead = NULL;

        ifstream file("menu.txt");
        if (!file.is_open())
        {
            cout << "No menu.txt found!\n";
            return;
        }

        string line;
        while (getline(file, line))
        {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, name, priceStr;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, priceStr, ',');

            if (idStr.empty() || name.empty() || priceStr.empty()) continue;

            int id = stoi(idStr);
            float price = stof(priceStr);

            menuItem* newItem = new menuItem;
            newItem->menuId = id;
            newItem->menuName = name;
            newItem->menuPrice = price;
            newItem->next = NULL;

            if (itemHead == NULL)
            {
                itemHead = newItem;
            }
            else
            {
                menuItem* current = itemHead;
                while (current->next != NULL)
                    current = current->next;
                current->next = newItem;
            }
        }

        file.close();
    }

    void displayMenu()
    {
        cout << "=====================================\n";
        cout << "                MENU                  \n";
        cout << "=====================================\n";
        cout << left << setw(6) << "ID"
             << setw(22) << "Item Name"
             << right << setw(12) << "Price (RM)\n";
        cout << "-------------------------------------\n";

        menuItem* current = itemHead;
        while (current != NULL)
        {
            cout << left << setw(6) << current->menuId
                 << setw(22) << current->menuName
                 << right << setw(12) << fixed << setprecision(2) << current->menuPrice
                 << "\n";
            current = current->next;
        }
        cout << "-------------------------------------\n";
    }
};

// ============================
// Customer Class
// ============================
class Customer : public User
{
public:
    Customer(string id = "", string username = "", string password = "")
        : User(id, username, password) {}

    void login() override
    {
        system("cls");
        cout << "================ CUSTOMER LOGIN ================\n";
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        // Simple demo login (you can connect to file later)
        if (username.empty() || password.empty())
        {
            cout << "Login failed. Press Enter...\n";
            cin.get();
            return;
        }

        id = "C001";
        customerMenu(*this);
    }
};

// ============================
// Admin Class (DISABLED)
// ============================
class Admin : public User
{
public:
    Admin(string id = "", string username = "", string password = "")
        : User(id, username, password) {}

    void login() override
    {
        /* ADMIN PORTAL DISABLED
        system("cls");
        cout << "================= ADMIN LOGIN =================\n";
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        if (username == "admin" && password == "admin")
        {
            cout << "Admin login success!\n";
        }
        else
        {
            cout << "Admin login failed!\n";
        }

        cout << "Press Enter to continue...";
        cin.get();
        */
        cout << "\nAdmin login is currently disabled.\n";
        cout << "Press Enter to return...";
        cin.get();
    }
};

// ============================
// ORDER IMPLEMENTATION
// ============================
menuItem* findMenuById(int id)
{
    menuItem* current = itemHead;
    while (current != NULL)
    {
        if (current->menuId == id) return current;
        current = current->next;
    }
    return NULL;
}

double calculateGrandTotal(orderItem* head)
{
    double sum = 0.0;
    while (head != NULL)
    {
        sum += head->total;
        head = head->next;
    }
    return sum;
}

int getNextOrderId(orderItem* head)
{
    int mx = 0;
    while (head != NULL)
    {
        if (head->orderId > mx) mx = head->orderId;
        head = head->next;
    }
    return mx + 1;
}

// ============================
// SORTING IMPLEMENTATION (Bubble Sort)
// ============================

void swapNodeData(orderItem* a, orderItem* b) {
    // Swaps all data fields between two nodes
    swap(a->orderId, b->orderId);
    swap(a->menuId, b->menuId);
    swap(a->orderName, b->orderName);
    swap(a->orderPrice, b->orderPrice);
    swap(a->orderQuantity, b->orderQuantity);
    swap(a->total, b->total);
}

void sortOrders(orderItem* head, int criteria) {
    if (head == NULL || head->next == NULL) return;

    bool swapped;
    orderItem* ptr1;
    orderItem* lptr = NULL; // last pointer

    do {
        swapped = false;
        ptr1 = head;

        while (ptr1->next != lptr) {
            bool shouldSwap = false;
            
            if (criteria == 1) { // Sort by Name (A-Z)
                if (ptr1->orderName > ptr1->next->orderName) shouldSwap = true;
            } else { // Sort by Order ID (Lowest to Highest)
                if (ptr1->orderId > ptr1->next->orderId) shouldSwap = true;
            }

            if (shouldSwap) {
                swapNodeData(ptr1, ptr1->next);
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void viewOrderSummary(orderItem* head)
{
    system("cls");
    if (head == NULL)
    {
        cout << "==================== ORDER SUMMARY ====================\n";
        cout << "No items in your order yet.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    // Ask user for sorting preference
    cout << "Order found! Would you like to sort the view?\n";
    cout << "1. Sort by Name (A-Z)\n";
    cout << "2. Sort by Order ID\n";
    cout << "0. No Sort / Keep Current\n";
    cout << "Enter choice: ";
    string sChoice;
    getline(cin, sChoice);

    if (sChoice == "1") sortOrders(head, 1);
    else if (sChoice == "2") sortOrders(head, 2);

    system("cls");
    cout << "==================== ORDER SUMMARY ====================\n";
    cout << left << setw(8) << "OID"
         << setw(8) << "MID"
         << setw(22) << "Item"
         << right << setw(6) << "Qty"
         << setw(12) << "Price"
         << setw(12) << "Total" << "\n";
    cout << "-------------------------------------------------------\n";

    orderItem* cur = head;
    while (cur != NULL)
    {
        cout << left << setw(8) << cur->orderId
             << setw(8) << cur->menuId
             << setw(22) << cur->orderName
             << right << setw(6) << cur->orderQuantity
             << setw(12) << fixed << setprecision(2) << cur->orderPrice
             << setw(12) << fixed << setprecision(2) << cur->total
             << "\n";
        cur = cur->next;
    }

    cout << "-------------------------------------------------------\n";
    cout << "Grand Total: RM " << fixed << setprecision(2) << calculateGrandTotal(head) << "\n";
    cout << "=======================================================\n";
    cout << "Press Enter to continue...";
    cin.get();
}

void addOrderItem(Menu& menuItems, orderItem*& head)
{
    system("cls");
    menuItems.displayMenu();

    cout << "\nEnter Menu ID: ";
    int mid;
    cin >> mid;

    cout << "Enter Quantity: ";
    int qty;
    cin >> qty;
    cin.ignore();

    if (qty <= 0)
    {
        cout << "Quantity must be more than 0.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    menuItem* m = findMenuById(mid);
    if (m == NULL)
    {
        cout << "Menu ID not found.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    // Merge if same item already exists
    orderItem* cur = head;
    while (cur != NULL)
    {
        if (cur->menuId == mid)
        {
            cur->orderQuantity += qty;
            cur->total = cur->orderQuantity * cur->orderPrice;
            cout << "Updated quantity for " << cur->orderName
                 << " (Qty now: " << cur->orderQuantity << ")\n";
            cout << "Press Enter to continue...";
            cin.get();
            return;
        }
        cur = cur->next;
    }

    orderItem* node = new orderItem;
    node->orderId = getNextOrderId(head);
    node->menuId = mid;
    node->orderName = m->menuName;
    node->orderPrice = m->menuPrice;
    node->orderQuantity = qty;
    node->total = node->orderPrice * qty;
    node->next = NULL;

    if (head == NULL) head = node;
    else
    {
        orderItem* t = head;
        while (t->next != NULL) t = t->next;
        t->next = node;
    }

    cout << "Added: " << node->orderName << " x" << qty << "\n";
    cout << "Press Enter...";
    cin.get();
}

void editOrderQuantity(orderItem* head)
{
    if (head == NULL)
    {
        cout << "\nNo orders to edit.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    cout << "\nEnter Order ID to edit: ";
    int oid;
    cin >> oid;

    cout << "Enter new quantity: ";
    int newQty;
    cin >> newQty;
    cin.ignore();

    if (newQty <= 0)
    {
        cout << "Quantity must be more than 0.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    orderItem* cur = head;
    while (cur != NULL)
    {
        if (cur->orderId == oid)
        {
            cur->orderQuantity = newQty;
            cur->total = cur->orderPrice * newQty;
            cout << "Order updated successfully.\n";
            cout << "Press Enter to continue...";
            cin.get();
            return;
        }
        cur = cur->next;
    }

    cout << "Order ID not found.\n";
    cout << "Press Enter to continue...";
    cin.get();
}

void deleteOrderLine(orderItem*& head)
{
    if (head == NULL)
    {
        cout << "\nNo orders to delete.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    cout << "\nEnter Order ID to delete: ";
    int oid;
    cin >> oid;
    cin.ignore();

    if (head->orderId == oid)
    {
        orderItem* temp = head;
        head = head->next;
        delete temp;
        cout << "Deleted successfully.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    orderItem* cur = head;
    while (cur->next != NULL && cur->next->orderId != oid)
        cur = cur->next;

    if (cur->next == NULL)
    {
        cout << "Order ID not found.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    orderItem* temp = cur->next;
    cur->next = temp->next;
    delete temp;

    cout << "Deleted successfully.\n";
    cout << "Press Enter to continue...";
    cin.get();
}

void confirmOrderToFile(orderItem*& head)
{
    if (head == NULL)
    {
        cout << "\nNo orders to confirm.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    cout << "\nConfirm order and save to file? (Y/N): ";
    string ans;
    getline(cin, ans);

    if (ans.empty() || toupper(ans[0]) != 'Y')
    {
        cout << "Cancelled.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    ofstream out("orders.txt", ios::app);
    if (!out.is_open())
    {
        cout << "Failed to open orders.txt\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    // Format: orderId,menuId,name,qty,price,total
    orderItem* cur = head;
    while (cur != NULL)
    {
        out << cur->orderId << ","
            << cur->menuId << ","
            << cur->orderName << ","
            << cur->orderQuantity << ","
            << fixed << setprecision(2) << cur->orderPrice << ","
            << fixed << setprecision(2) << cur->total
            << "\n";
        cur = cur->next;
    }
    out.close();

    // clear session after confirm
    while (head != NULL)
    {
        orderItem* temp = head;
        head = head->next;
        delete temp;
    }

    cout << "Order confirmed and saved!\n";
    cout << "Press Enter to continue...";
    cin.get();
}

void OrderMenu(Menu& menuItems, orderItem*& head)
{
    string choice;

    do
    {
        system("cls");
        cout << "====================================================\n";
        cout << "                    ORDER NOW                       \n";
        cout << "====================================================\n";
        cout << "                   1. Add Item                       \n";
        cout << "                   2. View Order Summary             \n";
        cout << "                   3. Edit Quantity                  \n";
        cout << "                   4. Delete Item                    \n";
        cout << "                   5. Confirm Order (Save)           \n";
        cout << "                   0. Back                           \n";
        cout << "====================================================\n";
        cout << "Enter choice: ";
        getline(cin, choice);

        if (choice == "1")
        {
            addOrderItem(menuItems, head);
        }
        else if (choice == "2")
        {
            viewOrderSummary(head);
        }
        else if (choice == "3")
        {
            viewOrderSummary(head);
            editOrderQuantity(head);
        }
        else if (choice == "4")
        {
            viewOrderSummary(head);
            deleteOrderLine(head);
        }
        else if (choice == "5")
        {
            viewOrderSummary(head);
            confirmOrderToFile(head);
        }

    } while (choice != "0");
}

// ============================
// Customer Menu
// ============================
void customerMenu(Customer& customer)
{
    string choice;
    Menu menuItems; // loads menu.txt

    do
    {
        system("cls");
        cout << "--------------------------------------------------------------\n";
        cout << "|                Welcome, " << customer.getUsername() << "                                |\n";
        cout << "--------------------------------------------------------------\n";
        cout << "                        CUSTOMER MENU\n";
        cout << "--------------------------------------------------------------\n";
        cout << "                       1. View Menu                            \n";
        cout << "                       2. Order Now                            \n";
        cout << "                       0. Logout                              \n";
        cout << "--------------------------------------------------------------\n";
        cout << "Enter your choice: ";
        getline(cin, choice);

        if (choice == "1")
        {
            system("cls");
            menuItems.displayMenu();
            cout << "Press Enter to continue...";
            cin.get();
        }
        else if (choice == "2")
        {
            OrderMenu(menuItems, orderHead);
        }

    } while (choice != "0");

    system("cls");
    cout << "\nLogging out customer: " << customer.getUsername() << "\n";
    cout << "Thank you for choosing KARABU!\n";
    cout << "Press Enter to continue...";
    cin.get();
}

class RegisterOrLogin
{
    string username, password;
    int userType;
    
    public: 
        RegisterOrLogin(int userType)
        {
            this -> userType = userType;
        }

        void RegisterOrLoginMenu()
        {
            int choice;
            do
            {
                system("cls");
                cout << "--------------------------------------------" << endl;
                cout << "                SELECT YOUR CHOICE            " << endl;
                cout << "--------------------------------------------" << endl;
                cout << "                   1. LOGIN                  " << endl;
                if(userType == 1)
                   cout << "                   2. REGISTER               " << endl;
                cout << "                   3. BACK                   " << endl;
                cout << "                   0. EXIT                   " << endl;
                cout << "--------------------------------------------" << endl;
                cout << "\n Please enter your choice in Interger(1,2,3...): ";

                if(!(cin >> choice))
                {
                    cout << "Invalid number.";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Press Enter to continue...";
                    cin.get();
                    continue;
                }

                if(userType == 2 && choice == 2)
                {
                    cout << "Invalid choice. Please try again!" << endl;
                    cout << "Press Enter to continue...";
                    cin.ignore(10000, '\n');
                    cin.get();
                    continue;
                }

                if(choice != 1 && choice != 2 && choice != 3 && choice != 0)
                {
                    cout << "Invalid choice. Please try again!" << endl;
                    cout << "Press Enter to continue...";
                    cin.ignore(10000, '\n');
                    cin.get();
                }
                else
                {
                    if(choice == 0)
                    {
                        exit(1);
                    }
                    else if(choice == 1)
                    {
                        login();
                    }
                    else if(choice == 2)
                    {
                        registerUser();
                    }
                }
            } while(choice != 3);
            return;
        }

        void login()
        {
            while(true)
            {
                system("cls");
                cout << "--------------------------------------------------" << endl;
                cout << "                         LOGIN                      " << endl;
                cout << "--------------------------------------------------" << endl;
                cout << " Enter username (or type 'exit' to cancel login): ";
                cin >> username;
                  
                for (char& c: username)
                {
                    c = tolower(c);
                }
                if(username == "exit")
                {
                    cout << "Exiting Login... "  << endl;
                    cout << "Press Enter to continue...";
                    cin.ignore(10000, '\n');
                    cin.get();
                    return;
                } 
                    
                cout << "Enter your password: ";
                cin >> password;
                
                // ADMIN FILENAME LOGIC COMMENTED OUT FOR SECURITY
                string filename = (userType == 1) ? "customers.txt" : "admin.txt";
                
                if (userType == 2) {
                   cout << "\nAdmin direct login through this module is disabled.\n";
                   cout << "Press Enter to continue...";
                   cin.ignore(10000, '\n');
                   cin.get();
                   return;
                }

                ifstream file(filename.c_str());
                if(!file)
                {
                    cout << "Error! No file found" << endl;
                    return;
                }

                string line;
                bool found = false;
                string fileUsername, filePassword, id;

                while(getline(file, line))
                {
                    if(userType == 1)
                    {
                        size_t p1 = line.find(',');
                        size_t p2 = line.find(',', p1 + 1);
                           
                        if(p1 != string::npos && p2 != string::npos)
                        {
                            id = line.substr(0,p1);
                            fileUsername = line.substr(p1+ 1, p2 - p1 - 1);
                            filePassword = line.substr(p2 + 1);
                               
                            if(username == fileUsername && password == filePassword)
                            {
                                found = true;
                                Customer customer(id, fileUsername, filePassword);
                                file.close();

                                cout << "Press Enter to continue...";
                                cin.ignore();
                                cin.get();
                                customerMenu(customer);
                                return;
                            }
                        }
                    }
                    /* ADMIN LOGIC COMMENTED OUT
                    else
                    {
                        string level, status;
                        size_t p1 = line.find(',');
                        size_t p2 = line.find(',', p1 + 1);
                        size_t p3 = line.find(',', p2 + 1);
                        size_t p4 = line.find(',', p3 + 1);

                        if(p1 != string::npos && p2 != string::npos && p3 != string::npos && p4 != string::npos)
                        {
                            id = line.substr(0, p1);
                            fileUsername = line.substr(p1 + 1, p2 - p1 - 1);
                            filePassword = line.substr(p2 + 1, p3 - p2 - 1);
                            level = line.substr(p3 + 1, p4 - p3 - 1);
                            status = line.substr(p4 + 1);

                            string lowerFileUsername = fileUsername;
                            for(char& c : lowerFileUsername)
                            {
                                c = tolower(c);
                            }
                            if(username == lowerFileUsername && password == filePassword)
                            {
                                found = true;
                                if(status == "active")
                                {
                                    // Staff admin(id, fileUsername, filePassword, stoi(level), status);
                                    // file.close();
                                    // cout << "Login successful!" << endl;
                                    // adminMenu(admin);
                                    // return;
                                }
                            }
                        }
                    }
                    */
                }
                file.close();
                if(!found)
                {
                    cout << "Invalid username or password." << endl;
                    cout << "Press Enter to try again...";
                    cin.ignore(10000, '\n');
                    cin.get();
                }
            }
        }

        void registerUser()
        {
            while(true)
            {
                system("cls");
                cout << "--------------------------------------------------" << endl;
                cout << "                        Register                    " << endl;
                cout << "--------------------------------------------------" << endl;
                cout << " Enter new username (key in 'exit' to exit Register): ";
                cin >> username; 

                if(username == "exit") return;

                for (char& c : username)
                {
                    c = tolower(c);
                }

                cout << "Enter new password: ";
                cin >> password;

                ifstream readFile("customers.txt");
                string line, existingUsername;
                bool exists = false;
                int maxId = 0;

                while(getline(readFile, line))
                {
                    size_t p1 = line.find(',');
                    size_t p2 = line.find(',', p1 + 1);

                    if(p1 != string::npos && p2 != string::npos)
                    {
                        string idStr = line.substr(0, p1);
                        existingUsername = line.substr(p1 + 1, p2 - p1 - 1);

                        if(username == existingUsername)
                        {
                            exists = true;
                            break;
                        }

                        string idNumStr = idStr.substr(1);
                        int idNum = stoi(idNumStr);
                        if(idNum > maxId) maxId = idNum;
                    }
                }
                readFile.close();

                if(exists)
                {
                    cout << "Username already exists. Try again." << endl;
                    cout << "Press Enter to continue...";
                    cin.ignore(10000, '\n');
                    cin.get();
                }
                else
                {
                    stringstream ss;
                    ss << "C" << setw(3) << setfill('0') << (maxId + 1);
                    string newId = ss.str();

                    ofstream writeFile("customers.txt", ios::app);
                    if(!writeFile)
                    {
                        cout << "Error. No file found for writing." << endl;
                        return;
                    }
                    writeFile << newId << "," << username << "," << password << endl;
                    writeFile.close();
                   
                    cout << "Registration successful! Your ID is: " << newId << endl;
                    cout << "You can now log in." << endl;
                    cout << "\nPress Enter to continue...";
                    cin.ignore(10000, '\n');
                    cin.get();
                    break;              
                }
            }
        }
};


// ============================
// Main
// ============================
int main()
{
    while (true)
    {
        system("cls");
        cout << "==================================================\n";
        cout << "      WELCOME TO KARABU FOOD ORDERING SYSTEM       \n";
        cout << "==================================================\n";
        cout << "                 1. CUSTOMER                      \n";
        cout << "                 2. ADMIN (DISABLED)              \n";
        cout << "                 0. EXIT                          \n";
        cout << "==================================================\n";
        cout << "Please enter your selection: ";

        int choice;
        try
        {
            if (!(cin >> choice))
                throw runtime_error("Invalid selection! Please enter a number.");

            cin.ignore(); // clear newline

            if (choice == 1)
            {
                // CHANGE: Instead of Customer.login(), 
                // we call the RegisterOrLogin class menu
                RegisterOrLogin customerPortal(1); // 1 indicates Customer type
                customerPortal.RegisterOrLoginMenu();
            }
            else if (choice == 2)
            {
                // Admin is disabled as per your request
                cout << "Admin portal is currently disabled.\n";
                cout << "Press Enter to continue...";
                cin.get();
            }
            else if (choice == 0)
            {
                cout << "Exiting system. Thank you for using KARABU!\n";
                break;
            }
            else
            {
                cout << "Invalid selection. Press Enter to continue...\n";
                cin.get();
            }
        }
        catch (const exception& e)
        {
            cout << "Error: " << e.what() << "\n";
            cout << "Press Enter...\n";
            cin.clear();
            cin.ignore(10000, '\n');
            cin.get();
        }
    }

    return 0;
}