#include <iostream>
#include <stack>
#include <gtest/gtest.h>

/* zad 4
{9,22 || 6,19,21 || 14 || 10,17 || 3,5,60 || 30 || 29 || 1,8 || 7 || 6,15 || 12}
{6, 9, 19, 21, 22 || 10, 14, 17 || 3, 5, 30, 60 || 1, 8, 29 || 6, 7, 15 || 12}
{6, 9, 10, 14, 17, 19, 21, 22 || 1, 3, 5, 8, 29, 30, 60 || 6, 7, 12, 15}
{1, 3, 5, 6, 8, 9, 10, 14, 17, 19, 21, 22, 29, 30, 60 || 6, 7, 12, 15}
{1, 3, 5, 6, 6, 7, 8, 9, 10, 12, 14, 15, 17, 19, 21, 22, 29, 30, 60}
*/


struct lnode {
    int key;
    lnode* next;
    lnode(int key, lnode* next): key(key), next(next) {}
};

void reverse(lnode*& n) {
    lnode* p = nullptr;
    while (n != nullptr) {
        lnode* q = n;   // temporary save list (first element)
        n = n->next;    // move list to next element
        q->next = p;    // first element of list points to first element of reversed list
        p = q;          // 
    }
    n = p;
}

// 2 - insertion sort on linked list, edit only next pointers
void insertion_sort(lnode*& head) {
    lnode* current = head;          // copy of head
    lnode* rev_sorted = nullptr;    // reversed sorted list

    while (current != nullptr) {
        lnode** rev_ptr = &rev_sorted;      // ptr to ptr to 1. element of rev_sorted which let us insert new element without losing head of list
        while (*rev_ptr != nullptr && (*rev_ptr)->key > current->key)
            rev_ptr = &(*rev_ptr)->next;    // find place to insert

        lnode* next = current->next;        // save next element
        if (*rev_ptr == nullptr) {          // insert at the end, rev points poter to nullptr
            *rev_ptr = current;
            (*rev_ptr)->next = nullptr;
        } else {                            // insert before rev_ptr
            lnode* rev_next = (*rev_ptr);   // save current element from rev_ptr
            *rev_ptr = current;
            (*rev_ptr)->next = rev_next;    // point to current from rev_ptr
        }
        current = next;
    }

    reverse(rev_sorted);
    head = rev_sorted;
}


struct node {
    int key;
    node* left;
    node* right;
    int nL = 0; // number of nodes in left subtree
    node(int key): key(key), left(nullptr), right(nullptr) {}
};

// 6 ity with nL inside tree
node* ity(node* t, int i) {
    while (t != nullptr) {
        if (i == t->nL)     // if i is equal to number of nodes in left subtree
            return t;
        else if (i < t->nL) // if i is smaller than number of nodes in left subtree
            t = t->left;    // go to left subtree
        else {
            i -= t->nL + 1; // if i is bigger than number of nodes in left subtree
            t = t->right;   // go to right subtree
        }
    }
    return nullptr;
}

// 7 insert with nL
void insert(node*& t, int x) // wstawianie (nierekurencyjne)
{
    node** a = &t; // a to adres wskaźnika na korzeń

    while (*a != nullptr) {	 // dopóki nie znalazłeś nulla, w którym należy podczepić x
        if (x < (**a).key) { // jeśli x jest mniejszy od klucza w korzeniu
            (**a).nL++;
            a = &(**a).left; // przejdź do wskaźnika na lewe poddrzewa
        } else
            a = &(**a).right; // przejdź do wskaźnika na prawe poddrzewa
    }

    *a = new node(x); // znalazłeś właściwego nulla więc podczep tam x
}

// 8 remove with nL
void remove(node*& t, int x) // usuwanie elementu z drzewa (bez rekurencji)
{
    node** a = &t;										// a to adres wskaźnika na korzeń
    while (*a && (**a).key != x) {						// póki *a nie jest nullem, ale nie zawiera x
        bool left = x < (**a).key;   // czy x jest mniejsze od klucza w węźle **a
        if (left)                   // jeśli x jest mniejsze od klucza w węźle **a
            (**a).nL--;             // zmniejsz liczbę węzłów w lewym poddrzewie
        a = left ? &(**a).left : &(**a).right; // przejdź do odpowiedniego poddrzewa
    }

    if (*a) // jeśli znaleziono x
    {
        if ((**a).right && (**a).left) // jeśli ma dwóch synów
        {
            node** b = &(**a).right; // zaczynając od prawego syna
            while ((**b).left) {              // idź w lewo póki można
                (**b).nL--;                 // zmniejsz liczbę węzłów w lewym poddrzewie
                b = &(**b).left;            // docelowo b to minimum prawego poddrzewa
            }
            (**a).key = (**b).key;	 // nadpisz x w węźle a kluczem z węzła b
            a = b;					 // i przeznacz węzeł b do usunięcia
        }
        // tutaj *a ma już co najwyżej jednego syna
        node* son = (**a).right ? (**a).right : (**a).left;

        delete* a; // usuwamy węzeł **t1
        *a = son;  // i zastępujemy synem
    }
}

// 8 remove ity
void remove_ity(node*& tree, int i) {
    if (i < 0) return;

    node** to_remove = &tree;
    while (*to_remove && (i != (**to_remove).nL)) {
        if (i < (**to_remove).nL) { // if i is smaller than number of nodes in left subtree
            (**to_remove).nL--;
            to_remove = &(**to_remove).left;    // go to left subtree
        } else {
            i -= (**to_remove).nL + 1; // if i is bigger than number of nodes in left subtree
            to_remove = &(**to_remove).right;   // go to right subtree
        }
    }

    if (*to_remove == nullptr) return;   // if i > n

    // if to_remove has two children
    if ((**to_remove).left && (**to_remove).right) {
        node** replace = &(**to_remove).right;
        while ((**replace).left) {
            (**replace).nL--;
            replace = &(**replace).left;
        }
        (**to_remove).key = (**replace).key;
        to_remove = replace;
    }
    // if to_remove has one child
    node* child = (**to_remove).left ? (**to_remove).left : (**to_remove).right;

    // remove to_remove and replace it with child
    delete* to_remove;
    *to_remove = child;
}

///////////////////////// TESTS /////////////////////////

TEST(l3t2, insertion_sort) {
    lnode* unsorted = new lnode(3, new lnode(2, new lnode(1, nullptr)));

    insertion_sort(unsorted);
    EXPECT_EQ(unsorted->key, 1);
    EXPECT_EQ(unsorted->next->key, 2);
    EXPECT_EQ(unsorted->next->next->key, 3);

    lnode* unsorted2 = new lnode(3, new lnode(3, new lnode(4, new lnode(1, nullptr))));

    insertion_sort(unsorted2);
    EXPECT_EQ(unsorted2->key, 1);
    EXPECT_EQ(unsorted2->next->key, 3);
    EXPECT_EQ(unsorted2->next->next->key, 3);
    EXPECT_EQ(unsorted2->next->next->next->key, 4);
}

TEST(l3t6, ityBST) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);
    insert(t, 8);

    EXPECT_EQ(ity(t, -1), nullptr);
    EXPECT_EQ(ity(t, 15), nullptr);
    EXPECT_EQ(ity(t, 2)->key, 4);
    EXPECT_EQ(ity(t, 0)->key, 2);
    EXPECT_EQ(ity(t, 5)->key, 7);
    EXPECT_EQ(ity(t, 4)->key, 6);
    EXPECT_EQ(ity(t, 6)->key, 8);
}

TEST(l3t7, insertBTS) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);
    insert(t, 8);

    EXPECT_EQ(t->nL, 3);
    EXPECT_EQ(t->left->nL, 1);
    EXPECT_EQ(t->left->left->nL, 0);
    EXPECT_EQ(t->left->right->nL, 0);
    EXPECT_EQ(t->right->nL, 1);
    EXPECT_EQ(t->right->left->nL, 0);
    EXPECT_EQ(t->right->right->nL, 0);

    EXPECT_EQ(t->key, 5);
    EXPECT_EQ(t->left->key, 3);
    EXPECT_EQ(t->left->left->key, 2);
    EXPECT_EQ(t->left->right->key, 4);
    EXPECT_EQ(t->right->key, 7);
    EXPECT_EQ(t->right->left->key, 6);
    EXPECT_EQ(t->right->right->key, 8);
}

TEST(l3t8, removeBTS) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);
    insert(t, 8);

    remove(t, 5);
    EXPECT_EQ(t->nL, 3);
    EXPECT_EQ(t->left->nL, 1);
    EXPECT_EQ(t->left->left->nL, 0);
    EXPECT_EQ(t->left->right->nL, 0);
    EXPECT_EQ(t->right->nL, 0);
    EXPECT_EQ(t->right->right->nL, 0);
}

TEST(l3t8, removeBTSity) {
    node* t = nullptr;
    insert(t, 5);
    insert(t, 3);
    insert(t, 7);
    insert(t, 2);
    insert(t, 4);
    insert(t, 6);
    insert(t, 8);

    remove_ity(t, 10);
    remove_ity(t, -1);

    remove_ity(t, 5);
    EXPECT_EQ(t->nL, 3);
    EXPECT_EQ(t->left->nL, 1);
    EXPECT_EQ(t->right->nL, 1);
    EXPECT_EQ(t->left->left->nL, 0);
    EXPECT_EQ(t->left->right->nL, 0);
    EXPECT_EQ(t->right->left->nL, 0);

    remove_ity(t, 3);
    EXPECT_EQ(t->nL, 3);
    EXPECT_EQ(t->left->nL, 1);
    EXPECT_EQ(t->right->nL, 0);


    node* root = nullptr;
    insert(root, 3);
    insert(root, 1);
    insert(root, 0);
    insert(root, 2);
    insert(root, 4);
    insert(root, 6);
    insert(root, 5);

    remove_ity(root, 3);
    EXPECT_EQ(root->key, 4);
    EXPECT_EQ(root->nL, 3);
    EXPECT_EQ(root->left->key, 1);
    EXPECT_EQ(root->left->nL, 1);
    EXPECT_EQ(root->right->key, 6);
    EXPECT_EQ(root->right->nL, 1);

    remove_ity(root, 1);
    EXPECT_EQ(root->key, 4);
    EXPECT_EQ(root->nL, 2);
    EXPECT_EQ(root->left->key, 2);
    EXPECT_EQ(root->left->nL, 1);
    EXPECT_EQ(root->left->left->key, 0);
    EXPECT_EQ(root->left->left->nL, 0);

    remove_ity(root, 4);
    EXPECT_EQ(root->key, 4);
    EXPECT_EQ(root->nL, 2);
    EXPECT_EQ(root->left->key, 2);
    EXPECT_EQ(root->left->nL, 1);
    EXPECT_EQ(root->right->key, 5);
    EXPECT_EQ(root->right->nL, 0);

    remove_ity(root, 7);
    EXPECT_EQ(root->key, 4);
    EXPECT_EQ(root->nL, 2);
    EXPECT_EQ(root->left->key, 2);
    EXPECT_EQ(root->left->nL, 1);
    EXPECT_EQ(root->right->key, 5);
    EXPECT_EQ(root->right->nL, 0);

    remove_ity(root, -1);
    EXPECT_EQ(root->key, 4);
    EXPECT_EQ(root->nL, 2);
    EXPECT_EQ(root->left->key, 2);
    EXPECT_EQ(root->left->nL, 1);
    EXPECT_EQ(root->right->key, 5);
    EXPECT_EQ(root->right->nL, 0);

}
