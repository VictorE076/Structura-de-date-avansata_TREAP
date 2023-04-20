// Economu Victor, grupa 134

// Treaps_Data_Structure_Proiect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <ctime>
using namespace std;
//ifstream f("abce.in.txt");
//ofstream g("abce.out.txt");
ifstream f("abce.in");
ofstream g("abce.out");

const unsigned short limit = 65535;
class Node
{
public:
    int value;
    unsigned short priority;
    Node* left;
    Node* right;

public:
    Node() = default;
    Node(const int value)
    {
        this->value = value;
        this->priority = rand() % limit;
        left = NULL;
        right = NULL;
    }
};
class Treap
{
private:
    unsigned size;
    Node* begin_;

public:
    Treap()
    {
        srand(time(0));
        size = 0;
        this->begin_ = NULL;
    }
    ~Treap()
    {
        delete_all_nodes(this->begin_);
    }
    //////
    unsigned get_size()
    {
        return this->size;
    }
    Node* get_first()
    {
        return this->begin_;
    }
private:
    void delete_all_nodes(Node* init)
    {
        if (init == NULL)
            return;
        delete_all_nodes(init->left);
        delete_all_nodes(init->right);
        Node* p = init;
        p = NULL;
        delete p;
    }
    void In_Order(Node* init)
    {
        if (init == NULL)
            return;
        In_Order(init->left);
        cout << init->value << " ( " << init->priority << " )\n";
        In_Order(init->right);
    }
    bool Max_Heap_Property(Node* init)
    {
        if (init == NULL)
            return true;
        if ((init->left != NULL && init->priority < init->left->priority) || (init->right != NULL && init->priority < init->right->priority))
            return false;
        bool lft = Max_Heap_Property(init->left);
        bool rgt = Max_Heap_Property(init->right);
        return (lft && rgt);
    }
    Node* Find(const int val)
    {
        Node* temp = this->begin_;
        while (temp != NULL)
        {
            if (temp->value == val)
                return temp;
            if (val < temp->value)
                temp = temp->left;
            else
                temp = temp->right;
        }
        return temp;
    }   
    Node* Left_Rotation(Node* current)
    {
        Node* right_child = current->right;
        Node* right_left_child = current->right->left;

        right_child->left = current;
        current->right = right_left_child;
        return right_child;
    }
    Node* Right_Rotation(Node* current)
    {
        Node* left_child = current->left;
        Node* left_right_child = current->left->right;

        left_child->right = current;
        current->left = left_right_child;
        return left_child;
    }
    Node* Insert_Value(Node* current, const int val)
    {
        if (current == NULL)
            return new Node(val);

        if (val < current->value)
        {
            current->left = Insert_Value(current->left, val);
            if (current->priority < current->left->priority)
                current = Right_Rotation(current);
            return current;
        }
        else
        {
            current->right = Insert_Value(current->right, val);
            if (current->priority < current->right->priority)
                current = Left_Rotation(current);
            return current;
        }
    }
    Node* Delete_Value(Node* current, const int val)
    {
        if (current == NULL)
            return current;

        if (val < current->value)
        {
            current->left = Delete_Value(current->left, val);
        }
        else if (current->value < val)
        {
            current->right = Delete_Value(current->right, val);
        }
        else if (current->left == NULL || current->right == NULL)
        {
            Node* p = current;
            if (current->left == NULL)
            { 
                current = current->right;  
                delete p;
            }
            else if (current->right == NULL)
            {
                current = current->left;  
                delete p;
            }
        }
        else
        {
            if (current->right->priority <= current->left->priority)
            {
                current = Right_Rotation(current);
                current->right = Delete_Value(current->right, val);
            }
            else
            {
                current = Left_Rotation(current);
                current->left = Delete_Value(current->left, val);
            }
        }
        return current;
    }
    Node* Closest_Below(Node* current, const int val)
    {
        if (current == NULL)
            return current;
        if (current->value == val)
            return current;
        Node* pt;
        if (val < current->value)
            pt = Closest_Below(current->left, val);
        else
            pt = Closest_Below(current->right, val);
        if (pt == NULL && current->value < val)
        {
            pt = current;
            return pt;
        }
        return pt;
    }
    Node* Closest_Above(Node* current, const int val)
    {
        if (current == NULL)
            return current;
        if (current->value == val)
            return current;
        Node* pt;
        if (val < current->value)
            pt = Closest_Above(current->left, val);
        else
            pt = Closest_Above(current->right, val);
        if (pt == NULL && current->value > val)
        {
            pt = current;
            return pt;
        } 
        return pt;
    }
    void sorted_sequence(Node* current, const int x, const int y)
    {
        if (current == NULL)
            return;
        if (current->left != NULL && x < current->value)
            sorted_sequence(current->left, x, y);

        if (x <= current->value && current->value <= y)
        {
            g << current->value << " ";
        }
            
        if (current->right != NULL && y > current->value)
            sorted_sequence(current->right, x, y);
    }

public:
    void Print_In_Order()
    {
        cout << "Afisare TREAP (value | priority):\n\n";
        In_Order(this->begin_);
        cout << "\n\n";
    }
    void Is_Max_Heap()
    {
        cout << "Max Heap: ";
        if (Max_Heap_Property(this->begin_))
            cout << "Da!\n";
        else
            cout << "Nu!\n";
    }

    ////// 
    bool Value_Found(const int val)
    {
        Node* p = Find(val);
        if (p == NULL)
            return false;
        return true;
    }
    void Insert(const int val)
    {
        this->begin_ = Insert_Value(this->begin_, val);
        this->size++;
    } 
    void Delete(const int val)
    {
        Node* p = Find(val);
        if (p == NULL)
            return;
        this->begin_ = Delete_Value(this->begin_, val);
        this->size--;
    }
    int get_Closest_Below_or_Above(const int val, const unsigned short choice)
    {
        Node* p = NULL;
        if (choice == 4)
            p = Closest_Below(this->begin_, val);
        else if(choice == 5)
            p = Closest_Above(this->begin_, val);
        if (p != NULL)
            return p->value;

        return 2147483647;
    }
    void Print_Sorted_Sequence(const int x, const int y)
    {
        sorted_sequence(this->begin_, x, y);
        g << "\n";
    }
    //////
};
int main()
{
    Treap t;
    int Q;
    f >> Q;
    for (int i = 0; i < Q; i++)
    {
        int cerinta, x;
        f >> cerinta >> x;

        switch (cerinta)
        {
        case 1:
            t.Insert(x);
            break;
        case 2:
            t.Delete(x);
            break;
        case 3:
            g << t.Value_Found(x) << "\n";
            break;
        case 4:
            g << t.get_Closest_Below_or_Above(x, 4) << "\n";
            break;
        case 5:
            g << t.get_Closest_Below_or_Above(x, 5) << "\n";
            break;
        case 6:
            int y;
            f >> y;
            t.Print_Sorted_Sequence(x, y);
            break;
        default:
            break;
        }
    }
    f.close();
    ///
    /*
    t.Print_In_Order();
    cout << "Size: " << t.get_size() << "\n";
    cout << "First Node: " << t.get_first()->value << " | " << t.get_first()->priority << "\n\n";
    t.Is_Max_Heap();
    */
    ///
    g.close();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
