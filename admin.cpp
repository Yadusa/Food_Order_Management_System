#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cctype>
#include <limits>
using namespace std;

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

/* ===================== HELPER FUNCTIONS ===================== */
string toLowerCase(string s) {
    for (char &c : s) c = tolower(c);
    return s;
}

void pressEnterToContinue() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
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
            pressEnterToContinue();
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
            pressEnterToContinue();
            return;
        } else {
            cout << "\nWrong username or password. Try again.\n";
            pressEnterToContinue();
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

        if (!getline(ss, idStr, ',')) continue;
        if (!getline(ss, nameStr, ',')) continue;
        if (!getline(ss, priceStr, ',')) continue;

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
        out << temp->data.foodID << ", " 
            << temp->data.name << ", " 
            << temp->data.price << endl;
        temp = temp->next;
    }
    out.close();
}

void displayFood() {
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

/* ===== ADD FOOD WITH DUPLICATE CHECK ===== */
void addFood() {
    system("cls");
    displayFood();

    FoodNode* newNode = new FoodNode;

    while (true) {
        cout << "Food ID (0 to go back): ";
        cin >> newNode->data.foodID;
        cin.ignore();
        if (newNode->data.foodID == 0) {
            delete newNode;
            return;
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
    pressEnterToContinue();
}

/* ===== EDIT FOOD ===== */
void editFood() {
    system("cls");
    displayFood();

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
            pressEnterToContinue();
            return;
        }
        temp = temp->next;
    }
    cout << "Food Not Found.\n";
    pressEnterToContinue();
}

void deleteFood() {
    system("cls");
    displayFood();

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
        pressEnterToContinue();
        return;
    }

    if (!prev) foodHead = temp->next;
    else prev->next = temp->next;

    delete temp;

    saveFoodToMenu();
    cout << "Food Deleted Successfully!\n";
    pressEnterToContinue();
}

/* ===== CASE-INSENSITIVE FOOD SEARCH ===== */
void searchFood() {
    system("cls");

    string key;
    cout << "Enter Food Name to search (0 to go back): ";
    cin.ignore();
    getline(cin, key);
    if (key == "0") return;

    key = toLowerCase(key);

    FoodNode* temp = foodHead;
    while (temp) {
        if (toLowerCase(temp->data.name) == key) {
            cout << "\nFood Found:\n";
            cout << "ID: " << temp->data.foodID << endl;
            cout << "Name: " << temp->data.name << endl;
            cout << "Price: RM" << temp->data.price << endl;
            pressEnterToContinue();
            return;
        }
        temp = temp->next;
    }
    cout << "Food Not Found.\n";
    pressEnterToContinue();
}

/* ===== SEARCH MENU BY ID OR NAME (MAIN MENU) ===== */
void searchMenu() {
    system("cls");

    if (!foodHead) {
        cout << "No food records available.\n";
        pressEnterToContinue();
        return;
    }

    cout << "--- SEARCH MENU ---\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Name\n";
    cout << "0. Back\n";
    cout << "Choice: ";
    int choice;
    cin >> choice;
    cin.ignore();
    if (choice == 0) return;

    FoodNode* temp = foodHead;

    if (choice == 1) {
        int id;
        cout << "Enter Food ID: ";
        cin >> id;
        cin.ignore();

        while (temp) {
            if (temp->data.foodID == id) {
                cout << "\nFood Found:\n";
                cout << "ID: " << temp->data.foodID << endl;
                cout << "Name: " << temp->data.name << endl;
                cout << "Price: RM" << temp->data.price << endl;
                pressEnterToContinue();
                return;
            }
            temp = temp->next;
        }
        cout << "Food Not Found.\n";
        pressEnterToContinue();
    }
    else if (choice == 2) {
        string key;
        cout << "Enter Food Name: ";
        getline(cin, key);
        key = toLowerCase(key);

        while (temp) {
            if (toLowerCase(temp->data.name) == key) {
                cout << "\nFood Found:\n";
                cout << "ID: " << temp->data.foodID << endl;
                cout << "Name: " << temp->data.name << endl;
                cout << "Price: RM" << temp->data.price << endl;
                pressEnterToContinue();
                return;
            }
            temp = temp->next;
        }
        cout << "Food Not Found.\n";
        pressEnterToContinue();
    }
    else {
        cout << "Invalid Choice!\n";
        pressEnterToContinue();
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
            cout << "Error: Username already exists! Please enter a different username.\n";
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
    pressEnterToContinue();
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
    pressEnterToContinue();
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
        pressEnterToContinue();
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

    pressEnterToContinue();
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
        cout << "5. Search Food/Drink (by Name Only)\n";
        cout << "0. Back\n";
        cout << "\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: addFood(); break;
            case 2: editFood(); break;
            case 3: deleteFood(); break;
            case 4: displayFood(); pressEnterToContinue(); break;
            case 5: searchFood(); break;
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
        pressEnterToContinue();
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
            case 1: addNewAdmin(); break;
            case 2: deleteAdmin(); break;
            case 3: displayAdmin(); pressEnterToContinue(); break;
            case 4: searchAdmin(); break;
        }
    } while(choice != 0);
}

void adminMenu() {
    loadFoodFromFile();
    int choice;
    do {
        system("cls");
        cout << "\n===== ADMIN MENU =====\n";
        cout << "1. Food & Drink\n";
        cout << "2. Search Menu (by ID or Name)\n";
        cout << "3. Manage Admin\n";
        cout << "0. Logout\n";
        cout << "\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: foodMenu(); break;
            case 2: searchMenu(); break;
            case 3: manageAdminMenu(); break;
        }
    } while(choice != 0);
}

/* ===================== MAIN ===================== */
int main() {
    initializeAdminFile();
    adminLogin();
    adminMenu();
    return 0;
}
