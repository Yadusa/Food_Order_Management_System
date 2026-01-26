#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <windows.h>
#include <stdexcept>
#include <cstring>
#include <limits>   

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

/* ===================== STRUCTS ===================== */
struct Food {
    int foodID;
    char name[30];
    float price;
};

struct ReportRecord {
    int reportID;
    char description[50];
};

/* ===================== LINKED LIST NODES ===================== */
struct FoodNode {
    Food data;
    FoodNode* next;
};

struct ReportNode {
    ReportRecord data;
    ReportNode* next;
};

/* ===================== GLOBAL HEADS ===================== */
FoodNode* foodHead = NULL;
ReportNode* reportHead = NULL;

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
            return;
        }

        id = "C001";
        customerMenu(*this);
    }
};

// ============================
// Admin Class
// ============================
class Admin : public User
{
public:
    Admin(string id = "", string username = "", string password = "")
        : User(id, username, password) {}

};

/* ===================== HELPER FUNCTIONS ===================== */
string toLowerCase(string s) {
    for (char &c : s) c = tolower(c);
    return s;
}

/* ===================== ADMIN FILE ===================== */
void initializeAdminFile() {
    ifstream in("admin.txt");
    if (!in) {
        ofstream out("admin.txt");
        out << "Admin 12345\n";   // default admin
        out.close();
        cout << "[System] Default admin file created.\n";
    }
    in.close();
}

/* ===================== LOGIN ===================== */
void adminLogin() {
    char username[20], password[20];
    bool found;

    while (true) {
        found = false;
        system("cls");
        cout << "\n===== ADMIN LOGIN =====\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (strcmp(username, "Admin") == 0 && strcmp(password, "12345") == 0) {
            cout << "Login Successful (Default Admin)!\n";
            cout << "Press Enter to continue...";
            return;
        }

        ifstream in("admin.txt");
        char u[20], p[20];
        while (in >> u >> p) {
            if (strcmp(username, u) == 0 && strcmp(password, p) == 0) {
                found = true;
                break;
            }
        }
        in.close();

        if (found) {
            cout << "Login Successful!\n";
            cout << "Press Enter to continue...";
            return;
        } else {
            cout << "\nWrong username or password. Try again.\n";
            cout << "Press Enter to continue...";
            cin.get();
        }
    }
}

/* ===================== FOOD FUNCTIONS ===================== */
void loadFoodFromFile() {
    ifstream in("menu.txt");
    if (!in) {
        cout << "menu.txt not found!\n";
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, nameStr, priceStr;
        Food f;

        getline(ss, idStr, ',');
        getline(ss, nameStr, ',');
        getline(ss, priceStr, ',');

        while (!nameStr.empty() && nameStr[0] == ' ') nameStr.erase(0,1);
        while (!priceStr.empty() && priceStr[0] == ' ') priceStr.erase(0,1);

        f.foodID = stoi(idStr);
        strcpy(f.name, nameStr.c_str());
        f.price = stof(priceStr);

        FoodNode* newNode = new FoodNode;
        newNode->data = f;
        newNode->next = foodHead;
        foodHead = newNode;
    }
    in.close();
}

void saveFoodToMenu() {
    ofstream out("menu.txt");
    FoodNode* temp = foodHead;
    while (temp) {
        out << temp->data.foodID << ","
            << temp->data.name << ","
            << temp->data.price << endl;
        temp = temp->next;
    }
    out.close();
}

/* ===== NORMAL DISPLAY (FOR ADD / EDIT / DELETE) ===== */
void displayFoodSimple() {
    FoodNode* temp = foodHead;
    if (!temp) {
        cout << "No food records found.\n";
        return;
    }

    cout << "\n=== FOOD LIST ===\n";
    while (temp) {
        cout << "ID: " << temp->data.foodID
             << " | Name: " << temp->data.name
             << " | Price: RM" << temp->data.price << endl;
        temp = temp->next;
    }
    cout << endl;
}

/* ===== HELPER: COPY LIST FOR DISPLAY SORT ===== */
FoodNode* copyFoodList() {
    if (!foodHead) return NULL;

    FoodNode* newHead = NULL;
    FoodNode* tail = NULL;
    FoodNode* temp = foodHead;

    while (temp) {
        FoodNode* newNode = new FoodNode;
        newNode->data = temp->data;
        newNode->next = NULL;

        if (!newHead) {
            newHead = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        temp = temp->next;
    }
    return newHead;
}

/* ===== DISPLAY FOOD WITH SORT MENU (ONLY FOR OPTION 4) ===== */
void displayFood() {
    if (!foodHead) {
        cout << "No food records found.\n";
        return;
    }

    system("cls");

    cout << "\n=== DISPLAY FOOD / DRINK ===\n";
    cout << "1. Default sort\n";
    cout << "2. Sort by price\n";
    cout << "3. Sort by name\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;
    cin.ignore();

    FoodNode* temp = copyFoodList();
    FoodNode* i;
    FoodNode* j;

    if (choice == 2) { // Sort by price
        for (i = temp; i != NULL; i = i->next) {
            for (j = i->next; j != NULL; j = j->next) {
                if (i->data.price > j->data.price) {
                    Food tempData = i->data;
                    i->data = j->data;
                    j->data = tempData;
                }
            }
        }
    }
    else if (choice == 3) { // Sort by name
        for (i = temp; i != NULL; i = i->next) {
            for (j = i->next; j != NULL; j = j->next) {
                if (strcmp(i->data.name, j->data.name) > 0) {
                    Food tempData = i->data;
                    i->data = j->data;
                    j->data = tempData;
                }
            }
        }
    }

    cout << "\n=== FOOD LIST ===\n";
    FoodNode* printTemp = temp;
    while (printTemp) {
        cout << "ID: " << printTemp->data.foodID
             << " | Name: " << printTemp->data.name
             << " | Price: RM" << printTemp->data.price << endl;
        printTemp = printTemp->next;
    }
    cout << endl;

    while (temp) {
        FoodNode* del = temp;
        temp = temp->next;
        delete del;
    }

    cout << "Press Enter to continue...";
    cin.get();
}

/* ===== HELPER TO CHECK DUPLICATES ===== */
bool isFoodNameExist(const string& name) {
    FoodNode* temp = foodHead;
    string lowerName = toLowerCase(name);

    while (temp) {
        if (toLowerCase(temp->data.name) == lowerName) return true;
        temp = temp->next;
    }
    return false;
}

bool isFoodIDExist(int id) {
    FoodNode* temp = foodHead;
    while (temp) {
        if (temp->data.foodID == id) return true;
        temp = temp->next;
    }
    return false;
}

bool isAdminExist(const string& username) {
    ifstream in("admin.txt");
    char u[20], p[20];

    while (in >> u >> p) {
        if (toLowerCase(u) == toLowerCase(username)) {
            in.close();
            return true;
        }
    }
    in.close();
    return false;
}

/* ===== ADD FOOD ===== */
void addFood() {
    system("cls");
    displayFoodSimple();   // NORMAL DISPLAY

    FoodNode* newNode = new FoodNode;

    while (true) {
        cout << "Food ID (0 to go back): ";
        cin >> newNode->data.foodID;
        cin.ignore();
        if (newNode->data.foodID == 0) {
            delete newNode;
            return;
        }

        if (isFoodIDExist(newNode->data.foodID)) {
            cout << "Error: Food ID already exists! Please enter a different ID.\n";
            cout << "Press Enter...";
            cin.get();
        }

        cout << "Food Name (0 to go back): ";
        cin.getline(newNode->data.name, 30);
        if (strcmp(newNode->data.name, "0") == 0) {
            delete newNode;
            return;
        }

        if (isFoodNameExist(newNode->data.name)) {
            cout << "Error: Food Name already exists! Please enter a different name.\n";
            continue;
        }

        cout << "Price: RM (0 to go back): ";
        cin >> newNode->data.price;
        cin.ignore();
        if (newNode->data.price == 0) {
            delete newNode;
            return;
        }

        break;
    }

    newNode->next = NULL;

    if (foodHead == NULL) foodHead = newNode;
    else {
        FoodNode* temp = foodHead;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }

    saveFoodToMenu();
    cout << "Food Added Successfully!\n";
    cout << "Press Enter to continue...";
    cin.get();
}

/* ===== EDIT FOOD ===== */
void editFood() {
    system("cls");
    displayFoodSimple();   // NORMAL DISPLAY

    int id;
    cout << "Enter Food ID to edit (0 to go back): ";
    cin >> id;
    cin.ignore();
    if (id == 0) return;

    FoodNode* temp = foodHead;
    while (temp) {
        if (temp->data.foodID == id) {
            string newName;
            while (true) {
                cout << "New Name (0 to cancel): ";
                getline(cin, newName);
                if (newName == "0") return;
                if (isFoodNameExist(newName)) {
                    cout << "Error: Food Name already exists! Please enter a different name.\n";
                    continue;
                }
                strcpy(temp->data.name, newName.c_str());
                break;
            }

            cout << "New Price (0 to cancel): ";
            cin >> temp->data.price;
            cin.ignore();
            if (temp->data.price == 0) return;

            saveFoodToMenu();
            cout << "Food Updated Successfully!\n";
            cout << "Press Enter to continue...";
            return;
        }
        temp = temp->next;
    }
    cout << "Food Not Found.\n";
    cout << "Press Enter to continue...";
}

/* ===== DELETE FOOD ===== */
void deleteFood() {
    system("cls");
    displayFoodSimple();   // NORMAL DISPLAY

    int id;
    cout << "Enter Food ID to delete (0 to go back): ";
    cin >> id;
    if (id == 0) return;

    FoodNode *temp = foodHead, *prev = NULL;
    while (temp && temp->data.foodID != id) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        cout << "Food Not Found.\n";
        cout << "Press Enter to continue...";
        return;

    if (!prev) foodHead = temp->next;
    else prev->next = temp->next;

    delete temp;
    saveFoodToMenu();

    cout << "Food Deleted Successfully!\n";
    cout << "Press Enter to continue...";
    cin.get();

  }
}

/* ===== SEARCH FOOD ===== */
void searchFood(){
    system("cls");

    string key;
    cout << "Enter Food Name to search (0 to go back): ";
    getline(cin, key); // no cin.ignore before this

    if (key == "0") return;

    key = toLowerCase(key);

    FoodNode* temp = foodHead;
    bool found = false;
    while (temp) {
        if (toLowerCase(temp->data.name) == key) {
            cout << "\nFood Found:\n";
            cout << "ID: " << temp->data.foodID << endl;
            cout << "Name: " << temp->data.name << endl;
            cout << "Price: RM" << temp->data.price << endl;
            found = true;
            break;
        }
        temp = temp->next;
    }

    if (!found) cout << "Food Not Found.\n";
    cout << "Press Enter to continue...";
}

/* ===== SEARCH MENU ===== */
void adminSearchMenu() {
    system("cls");

    if (!foodHead) {
        cout << "No food records available.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    cout << "--- SEARCH MENU ---\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Name\n";
    cout << "0. Back\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // FULL CLEAR
    if (choice == 0) return;

    FoodNode* temp = foodHead;

    if (choice == 1) {
        int id;
        cout << "Enter Food ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // FULL CLEAR

        while (temp) {
            if (temp->data.foodID == id) {
                cout << "\nFood Found:\n";
                cout << "ID: " << temp->data.foodID << endl;
                cout << "Name: " << temp->data.name << endl;
                cout << "Price: RM" << temp->data.price << endl;
                cout << "Press Enter...";
                cin.get();
                return;
            }
            temp = temp->next;
        }
        cout << "Food Not Found.\n";
        cout << "Press Enter to continue...";
        cin.get();
    }
    else if (choice == 2) {
        string key;
        cout << "Enter Food Name: ";
        getline(cin, key); // now works on first try
        key = toLowerCase(key);

        bool found = false;
        while (temp) {
            if (toLowerCase(temp->data.name) == key) {
                cout << "\nFood Found:\n";
                cout << "ID: " << temp->data.foodID << endl;
                cout << "Name: " << temp->data.name << endl;
                cout << "Price: RM" << temp->data.price << endl;
                found = true;
                break;
            }
            temp = temp->next;
        }
        if (!found) cout << "Food Not Found.\n";
        cout << "Press Enter to continue...";
        cin.get();
    }
}

/* ===================== ADMIN MANAGEMENT ===================== */
void addNewAdmin() {
    system("cls");

    char newUser[20], newPass[20];

    while (true) {
        cout << "New Admin Username (0 to go back): ";
        cin >> newUser;
        if (strcmp(newUser, "0") == 0) return;

        if (isAdminExist(newUser)) {
            cout << "Error: Username already exists!\n";
            continue;
        }

        cout << "New Admin Password (0 to go back): ";
        cin >> newPass;
        if (strcmp(newPass, "0") == 0) return;

        break;
    }

    ofstream out("admin.txt", ios::app);
    out << newUser << " " << newPass << "\n";
    out.close();
    cout << "New Admin Added!\n";
    cout << "Press Enter to continue...";;
    cin.get();
}

void displayAdmin() {
    ifstream in("admin.txt");
    char u[20], p[20];
    cout << "\n=== ADMIN LIST ===\n";
    while (in >> u >> p) cout << "Username: " << u << " | Password: " << p << endl;
    in.close();
    cout << endl;
}

void searchAdmin() {
    system("cls");

    string key;
    cout << "Enter Admin Username to Search (0 to go back): ";
    cin >> key;
    if (key == "0") return;
    key = toLowerCase(key);

    ifstream in("admin.txt");
    char u[20], p[20];
    bool found = false;

    while (in >> u >> p) {
        if (toLowerCase(u) == key) {
            cout << "Admin Found: " << u << " | Password: " << p << endl;
            found = true;
            break;
        }
    }
    in.close();

    if (!found) cout << "Admin Not Found.\n";
    cout << "Press Enter to continue...";
    cin.get();
}

void deleteAdmin() {
    system("cls");
    displayAdmin();

    char delUser[20];
    cout << "Enter Admin Username to Delete (0 to go back): ";
    cin >> delUser;
    if (strcmp(delUser, "0") == 0) return;

    if (strcmp(delUser, "Admin") == 0) {
        cout << "Cannot delete default Admin!\n";
        cout << "Press Enter to continue...";
        return;
    }

    ifstream in("admin.txt");
    ofstream out("temp.txt");
    char u[20], p[20];
    bool found = false;

    while (in >> u >> p) {
        if (strcmp(u, delUser) == 0) { found = true; continue; }
        out << u << " " << p << "\n";
    }
    in.close(); 
    out.close();

    remove("admin.txt");
    rename("temp.txt", "admin.txt");

    if (found) cout << "Admin Deleted!\n";
    else cout << "Admin Not Found.\n";

    cout << "Press Enter to continue...";
    cin.get();
}

/* ===================== MENUS ===================== */
void foodMenu() {
    int choice;
    do {
        system("cls");
        cout << "\n--- FOOD & DRINK MENU ---\n";
        cout << "1. Add Food/Drink\n";
        cout << "2. Edit Food/Drink\n";
        cout << "3. Delete Food/Drink\n";
        cout << "4. Display Food/Drink\n";
        cout << "5. Search Food/Drink\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // FULL CLEAR

        switch(choice) {
            case 1: 
                addFood(); 
                break;
            case 2: 
                editFood(); 
                break;
            case 3: 
                deleteFood(); 
                break;
            case 4: 
                displayFood(); 
                break;  // Sorting menu display
            case 5: 
                searchFood(); 
                break;
        }
    } while(choice != 0);
}

void manageAdminMenu() {
    char masterPass[20];
    system("cls");
    cout << "\nEnter Master Password to access Manage Admin (0 to go back): ";
    cin >> masterPass;
    if (strcmp(masterPass, "0") == 0) return;
    if (strcmp(masterPass, "Admin123") != 0) {
        cout << "Access Denied!\n"; 
        cout << "Press Enter to continue...";
        return;
    }

    int choice;
    do {
        system("cls");
        cout << "\n--- MANAGE ADMIN MENU ---\n";
        cout << "1. Add New Admin\n";
        cout << "2. Delete Admin\n";
        cout << "3. Display All Admin\n";
        cout << "4. Search Admin\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";
        cin >> choice;

        switch(choice) {
            case 1: 
                addNewAdmin(); 
                break;
            case 2: 
                deleteAdmin(); 
                break;
            case 3: 
                displayAdmin(); 
                break;
            case 4: 
                searchAdmin(); 
                break;
        }
    } while(choice != 0);
}

/* ===== VIEW RECEIPT BY ORDER ID ===== */
void viewReceiptByOrder()
{
    system("cls");
    ifstream in("orders.txt");
    if (!in)
    {
        cout << "No orders.txt found!\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    string line;
    string orderIDs = "";
    bool first = true;

    while (getline(in, line))
    {
        if (line.empty()) continue;
        stringstream ss(line);
        string oid;
        getline(ss, oid, ',');
        if (orderIDs.find(oid) == string::npos)
        {
            if (!first) orderIDs += ", ";
            orderIDs += oid;
            first = false;
        }
    }

    if (orderIDs.empty())
    {
        cout << "No receipts found.\n";
        in.close();
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    cout << "Available Receipts (Order IDs):\n";
    cout << orderIDs << "\n\n";
    cout << "Enter Order ID to view (0 to back): ";
    string selectedID;
    cin >> selectedID;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (selectedID == "0") { in.close(); return; }

    in.clear();
    in.seekg(0);
    system("cls");
    cout << "==================== RECEIPT FOR ORDER ID: " << selectedID << " ====================\n";
    cout << left << setw(8) << "OID" << setw(8) << "MID" << setw(22) << "Item"
         << right << setw(6) << "Qty" << setw(12) << "Price" << setw(12) << "Total" << "\n";
    cout << "---------------------------------------------------------------\n";

    bool found = false;
    double grandTotal = 0.0;

    while (getline(in, line))
    {
        if (line.empty()) continue;
        stringstream ss(line);
        string oid, mid, name, qty, price, total;

        getline(ss, oid, ',');
        getline(ss, mid, ',');
        getline(ss, name, ',');
        getline(ss, qty, ',');
        getline(ss, price, ',');
        getline(ss, total, ',');

        if (oid == selectedID)
        {
            try {
                if(total.empty()) continue; 
                double val = stod(total);
                found = true;
                grandTotal += val;

                cout << left << setw(8) << oid << setw(8) << mid << setw(22) << name
                     << right << setw(6) << qty << setw(12) << price << setw(12) << total << "\n";
            } catch (...) { continue; } // Skips lines that cause stod errors
        }
    }

    if (!found) cout << "\nReceipt not found or data corrupted for Order ID: " << selectedID << "\n";
    else {
        cout << "---------------------------------------------------------------\n";
        cout << "Grand Total: RM " << fixed << setprecision(2) << grandTotal << "\n";
    }
    cout << "===============================================================\n";
    in.close();
    cout << "Press Enter to continue...";
    cin.get();
}

void viewSummaryReport() {
    ifstream inFile("orders.txt");
    if (!inFile) {
        cout << "Error: No order history (orders.txt) found to generate report.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    double grandTotal = 0;
    int totalItems = 0;
    int transactionCount = 0;

    ofstream outFile("summaryreport.txt");
    
    outFile << "====================================================\n";
    outFile << "                   SALES SUMMARY REPORT             \n";
    outFile << "====================================================\n";
    outFile << left << setw(15) << "Order ID" << setw(20) << "Item Name" << "Total (RM)\n";
    outFile << "----------------------------------------------------\n";

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string oID, mID, name, qty, price, total;

        // Parsing CSV format: orderID,menuID,name,qty,price,total
        // Note: Your file format in confirmedOrderToFile is: ID, MID, Name, Qty, Price, Total
        if (!getline(ss, oID, ',')) continue;
        if (!getline(ss, mID, ',')) continue;
        if (!getline(ss, name, ',')) continue;
        if (!getline(ss, qty, ',')) continue;
        if (!getline(ss, price, ',')) continue;
        if (!getline(ss, total, ',')) continue;

        try {
            // Check if strings are not empty before converting
            if (total.empty() || qty.empty()) continue;

            double lineTotal = stod(total);
            int lineQty = stoi(qty);

            outFile << left << setw(15) << oID 
                    << setw(20) << name 
                    << fixed << setprecision(2) << lineTotal << "\n";

            grandTotal += lineTotal;
            totalItems += lineQty;
            transactionCount++;
        } catch (...) {
            // Skips lines that have non-numeric data in price/qty fields
            continue; 
        }
    }

    outFile << "----------------------------------------------------\n";
    outFile << "Total Transactions: " << transactionCount << "\n";
    outFile << "Total Units Sold:   " << totalItems << "\n";
    outFile << "GRAND TOTAL REVENUE: RM " << fixed << setprecision(2) << grandTotal << "\n";
    outFile << "====================================================\n";
    outFile << "Report Generated successfully.\n";

    inFile.close();
    outFile.close();

    cout << "The Summory Report has been updated successfully!";
    cout << "Press Enter to continue...";
    cin.get();
}
void adminFoodMenu() {
    loadFoodFromFile();
    int choice;
    do {
        system("cls");
        cout << "\n===== ADMIN MENU =====\n";
        cout << "1. Food & Drink\n";
        cout << "2. Search Menu\n";
        cout << "3. View Receipts\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: 
                foodMenu(); 
                break;
            case 2: 
                adminSearchMenu(); 
                break;
            case 3: 
                viewReceiptByOrder(); 
                break;
        }
    } while(choice != 0);
}

void adminOption()
{
	int choice;
	do 
	{
		system("cls");
		cout << "\n===== ADMIN DASHBOARD =====\n";
		cout << "1.Manage Food Menu\n";
        cout << "2. Manage Admin\n";
        cout << "3.Generate Summary Report\n";
        cout << "0. Logout\n";
        cout << "\nChoice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1: 
                adminFoodMenu(); 
                break;
            case 2: 
                manageAdminMenu(); 
                break;
            case 3: 
                viewSummaryReport(); 
                break;
            case 0: 
                cout << "Logging out...\n"; 
                break;
            default:
            	cout << "Invalid choice!\n";           
        }
    } while(choice != 0);
	
		
}



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
    cout << "============================== ORDER SUMMARY =================================\n";
    cout << left << setw(10) << "OrderID"
         << setw(10) << "MID"
         << setw(22) << "Item"
         << right << setw(6) << "Qty"
         << setw(12) << "Price"
         << setw(12) << "Total" << "\n";
    cout << "------------------------------------------------------------------------------\n";

    orderItem* cur = head;
    while (cur != NULL)
    {
        cout << left << setw(10) << cur->orderId
             << setw(10) << cur->menuId
             << setw(22) << cur->orderName
             << right << setw(6) << cur->orderQuantity
             << setw(12) << fixed << setprecision(2) << cur->orderPrice
             << setw(12) << fixed << setprecision(2) << cur->total
             << "\n";
        cur = cur->next;
    }

    cout << "------------------------------------------------------------------------------\n";
    cout << "Grand Total: RM " << fixed << setprecision(2) << calculateGrandTotal(head) << "\n";
    cout << "===============================================================================\n";
    cout << "Press Enter to continue...";
    cin.get();
}

// ========================================================
// SEARCHING IMPLEMENTATION (Binary Search from File)
// ========================================================

void searchMenu() { 
    struct TempItem {
        int id;
        string name;
        float price;
    };
    
    TempItem arr[100]; 
    int count = 0;

    ifstream file("menu.txt");
    if (!file.is_open()) {
        cout << "\n[ERROR]: Could not open menu.txt.\n";
        cout << "Press Enter to return...";
        cin.get();
        return;
    }

    string line;
    while (getline(file, line) && count < 100) {
        stringstream ss(line);
        string idStr, name, priceStr;
        
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, priceStr, ',');

        if (!idStr.empty() && !name.empty()) {
            // FIX: Remove leading/trailing spaces from the name
            size_t first = name.find_first_not_of(' ');
            if (string::npos != first) {
                size_t last = name.find_last_not_of(' ');
                name = name.substr(first, (last - first + 1));
            }

            arr[count].id = stoi(idStr);
            arr[count].name = name;
            arr[count].price = stof(priceStr);
            count++;
        }
    }
    file.close();

    if (count == 0) {
        cout << "\n[ERROR]: The menu is empty.\n";
        cin.get();
        return;
    }

    // Sort array by name for Binary Search
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (arr[j].name > arr[j+1].name) swap(arr[j], arr[j+1]);
        }
    }

    system("cls");
    cout << "================== SEARCH MENU ==================\n";
    cout << "Enter Food Name: ";
    string target;
    getline(cin, target);

    // Convert target to lowercase
    string lowTarget = target;
    for(int i=0; i < lowTarget.length(); i++) {
        lowTarget[i] = tolower(lowTarget[i]); 
    }

    int low = 0, high = count - 1;
    bool found = false;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        string lowCurrent = arr[mid].name;
        for(int i=0; i < lowCurrent.length(); i++) {
            lowCurrent[i] = tolower(lowCurrent[i]);
        }

        if (lowCurrent == lowTarget) {
            cout << "\n>>> ITEM AVAILABLE! <<<\n";
            cout << "--------------------------------------\n";
            cout << "Item Name : " << arr[mid].name << "\n";
            cout << "Menu ID   : " << arr[mid].id << "\n";
            cout << "Price     : RM " << fixed << setprecision(2) << arr[mid].price << "\n";
            cout << "--------------------------------------\n";
            found = true;
            break;
        }
        
        if (lowCurrent < lowTarget) low = mid + 1;
        else high = mid - 1;
    }

    if (!found) {
        cout << "\n[SEARCH ERROR]: '" << target << "' is not available.\n";
    }

    cout << "\nPress Enter to continue...";
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
    cout << "Press Enter to continue...";
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
        cout << "                   2. Search Food                    \n";
        cout << "                   3. View Order Summary             \n";
        cout << "                   4. Edit Quantity                  \n";
        cout << "                   5. Delete Item                    \n";
        cout << "                   6. Confirm Order (Save)           \n";
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
            searchMenu();
        }
        else if (choice == "3")
        {
            viewOrderSummary(head);
        }
        else if (choice == "4")
        {
            editOrderQuantity(head);
        }
        else if (choice == "5")
        {
            deleteOrderLine(head);
        }
        else if (choice == "6")
        {
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
        cout << "                 2. ADMIN                         \n";
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
                initializeAdminFile();
                adminLogin();
                adminOption();
            }
            else if (choice == 0)
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