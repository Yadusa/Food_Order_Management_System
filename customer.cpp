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
        cout << "               MENU                  \n";
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
// Admin Class (simple stub)
// ============================
class Admin : public User
{
public:
    Admin(string id = "", string username = "", string password = "")
        : User(id, username, password) {}

    void login() override
    {
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
    }
};

// ============================
// ORDER IMPLEMENTATION (Your part)
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

void viewOrderSummary(orderItem* head)
{
    system("cls");
    cout << "==================== ORDER SUMMARY ====================\n";

    if (head == NULL)
    {
        cout << "No items in your order yet.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

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
        cout << "Press Enter...";
        cin.get();
        return;
    }

    menuItem* m = findMenuById(mid);
    if (m == NULL)
    {
        cout << "Menu ID not found.\n";
        cout << "Press Enter...";
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
            cout << "Press Enter...";
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
        cout << "Press Enter...";
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
        cout << "Press Enter...";
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
            cout << "Press Enter...";
            cin.get();
            return;
        }
        cur = cur->next;
    }

    cout << "Order ID not found.\n";
    cout << "Press Enter...";
    cin.get();
}

void deleteOrderLine(orderItem*& head)
{
    if (head == NULL)
    {
        cout << "\nNo orders to delete.\n";
        cout << "Press Enter...";
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
        cout << "Press Enter...";
        cin.get();
        return;
    }

    orderItem* cur = head;
    while (cur->next != NULL && cur->next->orderId != oid)
        cur = cur->next;

    if (cur->next == NULL)
    {
        cout << "Order ID not found.\n";
        cout << "Press Enter...";
        cin.get();
        return;
    }

    orderItem* temp = cur->next;
    cur->next = temp->next;
    delete temp;

    cout << "Deleted successfully.\n";
    cout << "Press Enter...";
    cin.get();
}

void confirmOrderToFile(orderItem*& head)
{
    if (head == NULL)
    {
        cout << "\nNo orders to confirm.\n";
        cout << "Press Enter...";
        cin.get();
        return;
    }

    cout << "\nConfirm order and save to file? (Y/N): ";
    string ans;
    getline(cin, ans);

    if (ans.empty() || toupper(ans[0]) != 'Y')
    {
        cout << "Cancelled.\n";
        cout << "Press Enter...";
        cin.get();
        return;
    }

    ofstream out("orders.txt", ios::app);
    if (!out.is_open())
    {
        cout << "Failed to open orders.txt\n";
        cout << "Press Enter...";
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
    cout << "Press Enter...";
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
        cout << "1. Add Item\n";
        cout << "2. View Order Summary\n";
        cout << "3. Edit Quantity\n";
        cout << "4. Delete Item\n";
        cout << "5. Confirm Order (Save)\n";
        cout << "0. Back\n";
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
        cout << "|               Welcome, " << customer.getUsername() << "                               |\n";
        cout << "--------------------------------------------------------------\n";
        cout << "                         CUSTOMER MENU\n";
        cout << "--------------------------------------------------------------\n";
        cout << "1. View Menu\n";
        cout << "2. Order Now\n";
        cout << "3. View Order Summary\n";
        cout << "0. Logout\n";
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
        else if (choice == "3")
        {
            viewOrderSummary(orderHead);
        }

    } while (choice != "0");

    system("cls");
    cout << "\nLogging out customer: " << customer.getUsername() << "\n";
    cout << "Thank you for choosing KARABU!\n";
    cout << "Press Enter to continue...";
    cin.get();
}

// ============================
// Main
// ============================
int main()
{
    while (true)
    {
        system("cls");
        cout << "==================================================\n";
        cout << "     WELCOME TO KARABU FOOD ORDERING SYSTEM       \n";
        cout << "==================================================\n";
        cout << "1. CUSTOMER\n";
        cout << "2. ADMIN\n";
        cout << "3. EXIT\n";
        cout << "Please enter your selection: ";

        int choice;
        try
        {
            if (!(cin >> choice))
                throw runtime_error("Invalid selection! Please enter a number.");

            cin.ignore(); // clear newline

            if (choice == 1)
            {
                Customer customer;
                customer.login();
            }
            else if (choice == 2)
            {
                Admin admin;
                admin.login();
            }
            else if (choice == 3)
            {
                cout << "Exiting system. Thank you for using KARABU!\n";
                break;
            }
            else
            {
                cout << "Invalid selection. Press Enter...\n";
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
