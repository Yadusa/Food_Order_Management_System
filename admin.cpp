#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
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
        cout << "\n===== ADMIN LOGIN =====\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (strcmp(username, "Admin") == 0 && strcmp(password, "12345") == 0) {
            cout << "Login Successful (Default Admin)!\n";
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
            return;
        } else {
            cout << "\n❌ Wrong username or password. Try again.\n";
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
        out << temp->data.foodID << ", " << temp->data.name << ", " << temp->data.price << endl;
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
}

void addFood() {
    FoodNode* newNode = new FoodNode;

    cout << "Food ID: ";
    cin >> newNode->data.foodID;
    cin.ignore();
    cout << "Food Name: ";
    cin.getline(newNode->data.name, 30);
    cout << "Price: RM ";
    cin >> newNode->data.price;
    cin.ignore();

    newNode->next = foodHead;
    foodHead = newNode;

    saveFoodToMenu();
    cout << "Food Added Successfully!\n";
}

void editFood() {
    int id;
    cout << "Enter Food ID to edit: ";
    cin >> id;
    cin.ignore();

    FoodNode* temp = foodHead;
    while (temp) {
        if (temp->data.foodID == id) {
            cout << "New Name: ";
            cin.getline(temp->data.name, 30);
            cout << "New Price: RM ";
            cin >> temp->data.price;
            cin.ignore();

            saveFoodToMenu();
            cout << "Food Updated Successfully!\n";
            return;
        }
        temp = temp->next;
    }
    cout << "Food Not Found.\n";
}

void deleteFood() {
    int id;
    cout << "Enter Food ID to delete: ";
    cin >> id;

    FoodNode *temp = foodHead, *prev = NULL;
    while (temp && temp->data.foodID != id) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        cout << "Food Not Found.\n";
        return;
    }

    if (!prev) foodHead = temp->next;
    else prev->next = temp->next;

    delete temp;

    saveFoodToMenu();
    cout << "Food Deleted Successfully!\n";
}

void sortFoodByPrice() {
    if (!foodHead) return;
    for (FoodNode* i = foodHead; i->next; i = i->next) {
        for (FoodNode* j = i->next; j; j = j->next) {
            if (i->data.price > j->data.price) {
                Food temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
    saveFoodToMenu();
    cout << "Food Sorted by Price!\n";
}

void searchFood() {
    int id;
    cout << "Enter Food ID to search: ";
    cin >> id;

    FoodNode* temp = foodHead;
    while (temp) {
        if (temp->data.foodID == id) {
            cout << "Food Found: " << temp->data.name
                 << " | Price: RM" << temp->data.price << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "Food Not Found.\n";
}

/* ===================== REPORT FUNCTIONS ===================== */
void addReportRecord() {
    ReportNode* newNode = new ReportNode;
    cout << "Enter Report ID: ";
    cin >> newNode->data.reportID;
    cin.ignore();
    cout << "Enter Description: ";
    cin.getline(newNode->data.description, 50);

    newNode->next = reportHead;
    reportHead = newNode;
    cout << "Report Record Added Successfully!\n";
}

void editReportRecord() {
    int id;
    cout << "Enter Report ID to edit: ";
    cin >> id;
    cin.ignore();

    ReportNode* temp = reportHead;
    while (temp) {
        if (temp->data.reportID == id) {
            cout << "New Description: ";
            cin.getline(temp->data.description, 50);
            cout << "Report Updated!\n";
            return;
        }
        temp = temp->next;
    }
    cout << "Record Not Found.\n";
}

void deleteReportRecord() {
    int id;
    cout << "Enter Report ID to delete: ";
    cin >> id;

    ReportNode *temp = reportHead, *prev = NULL;
    while (temp && temp->data.reportID != id) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        cout << "Record Not Found.\n";
        return;
    }

    if (!prev) reportHead = temp->next;
    else prev->next = temp->next;

    delete temp;
    cout << "Record Deleted!\n";
}

void displayReportRecord() {
    if (!reportHead) {
        cout << "No report records found.\n";
        return;
    }

    cout << "\n=== REPORT RECORDS ===\n";
    for (ReportNode* temp = reportHead; temp; temp = temp->next) {
        cout << "ID: " << temp->data.reportID
             << " | Description: " << temp->data.description << endl;
    }
}

void searchReportRecord() {
    int id;
    cout << "Enter Report ID to search: ";
    cin >> id;

    for (ReportNode* temp = reportHead; temp; temp = temp->next) {
        if (temp->data.reportID == id) {
            cout << "Found: " << temp->data.description << endl;
            return;
        }
    }
    cout << "Record Not Found.\n";
}

void summaryReport() {
    int count = 0;
    for (ReportNode* temp = reportHead; temp; temp = temp->next) count++;
    cout << "\n=== SUMMARY REPORT ===\n";
    cout << "Total Records: " << count << endl;
}

/* ===================== ADMIN MANAGEMENT ===================== */
void addNewAdmin() {
    char newUser[20], newPass[20];
    cout << "New Admin Username: ";
    cin >> newUser;
    cout << "New Admin Password: ";
    cin >> newPass;

    ofstream out("admin.txt", ios::app);
    out << newUser << " " << newPass << "\n";
    out.close();
    cout << "New Admin Added!\n";
}

void displayAdmin() {
    ifstream in("admin.txt");
    char u[20], p[20];
    cout << "\n=== ADMIN LIST ===\n";
    while (in >> u >> p) {
        cout << "Username: " << u << " | Password: " << p << endl;
    }
    in.close();
}

void deleteAdmin() {
    char delUser[20];
    cout << "Enter Admin Username to Delete: ";
    cin >> delUser;

    if (strcmp(delUser, "Admin") == 0) {
        cout << "Cannot delete default Admin!\n";
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
    in.close(); out.close();

    remove("admin.txt");
    rename("temp.txt", "admin.txt");

    if (found) cout << "Admin Deleted!\n";
    else cout << "Admin Not Found.\n";
}

/* ===================== MENUS ===================== */
void foodMenu() {
    int choice;
    do {
        cout << "\n--- FOOD & DRINK MENU ---\n";
        cout << "1. Add Food/Drink\n";
        cout << "2. Edit Food/Drink\n";
        cout << "3. Delete Food/Drink\n";
        cout << "4. Display Food/Drink\n";
        cout << "5. Search Food/Drink\n";
        cout << "6. Sort by Price\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addFood(); break;
            case 2: editFood(); break;
            case 3: deleteFood(); break;
            case 4: displayFood(); break;
            case 5: searchFood(); break;
            case 6: sortFoodByPrice(); break;
        }
    } while(choice != 0);
}

void reportMenu() {
    int choice;
    do {
        cout << "\n--- REPORT MENU ---\n";
        cout << "1. Add Record\n";
        cout << "2. Edit Record\n";
        cout << "3. Delete Record\n";
        cout << "4. Summary Report\n";
        cout << "5. Display Record\n";
        cout << "6. Search Record\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addReportRecord(); break;
            case 2: editReportRecord(); break;
            case 3: deleteReportRecord(); break;
            case 4: summaryReport(); break;
            case 5: displayReportRecord(); break;
            case 6: searchReportRecord(); break;
        }
    } while(choice != 0);
}

void manageAdminMenu() {
    char masterPass[20];
    cout << "\nEnter Master Password to access Manage Admin: ";
    cin >> masterPass;
    if (strcmp(masterPass, "Admin123") != 0) {
        cout << "Access Denied!\n"; return;
    }

    int choice;
    do {
        cout << "\n--- MANAGE ADMIN MENU ---\n";
        cout << "1. Add New Admin\n";
        cout << "2. Delete Admin\n";
        cout << "3. Display All Admin\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addNewAdmin(); break;
            case 2: deleteAdmin(); break;
            case 3: displayAdmin(); break;
        }
    } while(choice != 0);
}

void adminMenu() {
    loadFoodFromFile();
    int choice;
    do {
        cout << "\n===== ADMIN MENU =====\n";
        cout << "1. Food & Drink\n";
        cout << "2. Report\n";
        cout << "3. Manage Admin\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch(choice) {
            case 1: foodMenu(); break;
            case 2: reportMenu(); break;
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
