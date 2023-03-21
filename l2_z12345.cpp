#include <iostream>
#include <stack>
#include <gtest/gtest.h>

struct node {
    int key;
    node* left;
    node* right;
    node* parent;
    node(int key, node* parent = nullptr): key(key), left(nullptr), right(nullptr), parent(parent) {}
};

void insert(node*& t, int x) {      // wstawianie (nierekurencyjne)
    node** a = &t;                  // a to adres wskaźnika na korzeń
    node* parent = nullptr;

    while (*a != nullptr)           // dopóki nie znalazłeś nulla, w którym należy podczepić x
        if (x < (**a).key) {        // jeśli x jest mniejszy od klucza w korzeniu
            parent = *a;
            a = &(**a).left;        // przejdź do wskaźnika na lewe poddrzewa
        } else {
            parent = *a;
            a = &(**a).right;       // przejdź do wskaźnika na prawe poddrzewa
        }

    *a = new node(x, parent);       // znalazłeś właściwego nulla więc podczep tam x
}

void remove(node*& t, int x) {                          // usuwanie elementu z drzewa (bez rekurencji)
    node** a = &t;                                      // a to adres wskaźnika na korzeń
    while (*a && (**a).key != x)                        // póki *a nie jest nullem, ale nie zawiera x
        a = x < (**a).key ? &(**a).left : &(**a).right; // przejdź do odpowiedniego poddrzewa

    if (*a) {                               // jeśli znaleziono x
        if ((**a).right && (**a).left)      // jeśli ma dwóch synów
        {
            node** b = &(**a).right;        // zaczynając od prawego syna
            while ((**b).left)              // idź w lewo póki można
                b = &(**b).left;            // docelowo b to minimum prawego poddrzewa
            (**a).key = (**b).key;          // nadpisz x w węźle a kluczem z węzła b
            a = b;                          // i przeznacz węzeł b do usunięcia
        }
        // tutaj *a ma już co najwyżej jednego syna
        node* son = (**a).right ? (**a).right : (**a).left;
        if (son != nullptr)
            son->parent = (**a).parent;
        delete* a; // usuwamy węzeł **t1
        *a = son;  // i zastępujemy synem
    }
}

// 2 recursive inorder_do functions
void inorder_do(node* t, void (*f)(node* t)) {
    if (t) {
        inorder_do(t->left, f);
        f(t);
        inorder_do(t->right, f);
    }
}

// 3 iterative inorder_do functions
void inorder_do_it(node* t, void (*f)(node* t)) {
    if (t == nullptr) 
        return;

    node* current = t;
    while (current->left)           // go to smallest node
        current = current->left;

    f(current); 

    node* next = current;
    while (next) {                  // loop until is sth to call f on
        if (current->right) {       // if right child exists
            next = current->right;  // move to right child 
            while (next->left)      // go to smallest node
                next = next->left;
        } else {                    // go up to parent 
            next = current->parent; // go to parent 
            while (next && next->right == current) {    // while current is right child of next 
                current = next;         // skip next and go to next's parent
                next = next->parent;    
            }
        }

        if (next != nullptr)
            f(next);                // if next is not nullptr
        current = next;
    }
}

// 4 BSTiter
class BSTiter {
private:
    node* current; 

public:
    BSTiter(node* t): current(t) {} 

    bool operator!=(BSTiter const& other) const {
        return current != other.current;
    }

    BSTiter& operator++() {
        if (current->right) {               
            current = current->right;       // go to the right subtree
            while (current->left)           // go to the smallest
                current = current->left;

        } else {                            // go up to the parent
            node* next = current->parent;   // go to the parent
            while (next && next->right == current) {    // while current is right child of next
                current = next;             // skip next and go to next's parent
                next = next->parent;
            }
            current = next;
        }
        return *this;
    }

    int operator*() const {
        return current->key;
    }
};

BSTiter begin(node* t) {
    node* current = t;
    while (current->left)           // go the smallest node
        current = current->left;

    return BSTiter(current);
}

BSTiter end(node* t) {
    return BSTiter(nullptr);
}

// 5 BSTiter2 without parent with stack
class BSTiter2 {
private:
    node* current;
    std::stack<node*> stack; // stack of nodes that should be visited after the current node

public:
    BSTiter2(node* t): current(t) {
        while (current->left) { // go to the smallest node and push all nodes on the path to the stack
            stack.push(current);
            current = current->left;
        }
    }

    bool operator!=(BSTiter2 const& other) const {
        return current != other.current;
    }

    BSTiter2& operator++() {
        if (current->right) {
            current = current->right;
            while (current->left) {
                stack.push(current);        // push all nodes on the path to the stack
                current = current->left;
            }
        } else {
            if (stack.empty())
                current = nullptr;          // if the stack is empty, we are at the end
            else {
                current = stack.top();      // pop the next node from the stack
                stack.pop();
            }
        }
        return *this; 
    }

    // Return the key of the current node
    int operator*() const {
        return current->key;
    }
};

BSTiter2 begin2(node* t) {
    return BSTiter2(t);
}

BSTiter2 end2(node* t) {
    return BSTiter2(nullptr);
}

///////////////////////// TESTS /////////////////////////

TEST(l2_1, insert) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);

    EXPECT_EQ(t->key, 5);
    EXPECT_EQ(t->left->key, 3);
    EXPECT_EQ(t->right->key, 7);
    EXPECT_EQ(t->left->left->key, 2);
    EXPECT_EQ(t->left->right->key, 4);
    EXPECT_EQ(t->right->left->key, 6);

    EXPECT_EQ(t->parent, nullptr);
    EXPECT_EQ(t->left->parent->key, 5);
    EXPECT_EQ(t->right->parent->key, 5);
    EXPECT_EQ(t->left->left->parent->key, 3);
}

TEST(l2_1, remove) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);

    remove(t, 3);
    EXPECT_EQ(t->left->key, 4);
    EXPECT_EQ(t->left->left->key, 2);
    EXPECT_EQ(t->left->parent->key, 5);
    EXPECT_EQ(t->left->left->parent->key, 4);

    remove(t, 5);
    EXPECT_EQ(t->key, 6);
    EXPECT_EQ(t->parent, nullptr);
    EXPECT_EQ(t->left->key, 4);

    remove(t, 2);
    EXPECT_EQ(t->left->left, nullptr);
    EXPECT_EQ(t->left->parent->key, 6);
}

TEST(l2_2, inorder_do) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);

    inorder_do(t, [](node* t) { std::cout << t->key << " "; });
    std::cout << "\n";

    inorder_do(t, [](node* t) { t->key++; });
    EXPECT_EQ(t->key, 6);
    EXPECT_EQ(t->left->key, 4);
    EXPECT_EQ(t->right->key, 8);
    EXPECT_EQ(t->left->left->key, 3);
    EXPECT_EQ(t->left->right->key, 5);
    EXPECT_EQ(t->right->left->key, 7);
}

TEST(l2_3, inorder_do_it) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);

    inorder_do_it(t, [](node* t) { std::cout << t->key << " "; });
    std::cout << "\n";

    inorder_do_it(t, [](node* t) { t->key++; });
    EXPECT_EQ(t->key, 6);
    EXPECT_EQ(t->left->key, 4);
    EXPECT_EQ(t->right->key, 8);
    EXPECT_EQ(t->left->left->key, 3);
    EXPECT_EQ(t->left->right->key, 5);
    EXPECT_EQ(t->right->left->key, 7);

}

TEST(l2_4, BSTiter) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);

    int count = 0;
    for (BSTiter it = begin(t); it != end(t); ++it) {
        std::cout << *it << " ";
        count++;
    }
    std::cout << "\n";
    EXPECT_EQ(count, 6);

    count = 0;
    for (auto x : t) {
        std::cout << x << " ";
        count++;
    }
    std::cout << "\n";
    EXPECT_EQ(count, 6);
}

TEST(l2_5, name) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);

    int count = 0;
    for (BSTiter it = begin(t); it != end(t); ++it) {
        std::cout << *it << " ";
        count++;
    }
    std::cout << "\n";
    EXPECT_EQ(count, 6);

    count = 0;
    for (auto x : t) {
        std::cout << x << " ";
        count++;
    }
    std::cout << "\n";
    EXPECT_EQ(count, 6);
}
