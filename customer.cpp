#include <iostream>
#include <string>

using namespace std;

struct orderItems
{
    int id;
    string name;
    double price;
};

struct Node
{
    int orderID;
    string food;
    Node* next;
}

class User
{
    protected:
       string username;
    public:
      User() { username = "Guest";}
      virtual ~User() {}
      virtual void menu() = 0;
};