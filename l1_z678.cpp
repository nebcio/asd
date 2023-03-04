#include <iostream>

struct lnode {
    int key;
    lnode *next;
    lnode(int k, lnode *n = nullptr) : key(k), next(n) {}
};

// 6a - print elements of a linked list with space between them
void print(lnode *n) {
    while (n != nullptr) {
        std::cout << n->key << " ";
        n = n->next;
    }
    std::cout << std::endl;
}

// 6b - sum elemnts of a linked list
int sum(lnode *n) {
    int sum = 0;
    while (n != nullptr) {
        sum += n->key;
        n = n->next;
    }
    return sum;
}

// 6c - nth element of a linked list or 0 if n is out of range /////////////////////
int nth(lnode *n, int i) {
    while (n != nullptr && i > 0) {
        n = n->next;
        i--;
    }
    return n == nullptr ? 0 : n->key;
}

// 6d - insert element at the beginning of a linked list
void insert(lnode *&n, int x) {
    n = new lnode(x, n);
}

// 6e - insert after smaller elements
void insert_after_smaller(lnode *&n, int x) {
    // if list is empty
    if (n == nullptr) {
        n = new lnode(x);
        return;
    }

    // if x is smaller than first element
    if (n->key > x) {
        n = new lnode(x, n);
        return;
    }

    // if x is bigger than first element
    lnode *p = n;
    while (p->next != nullptr && p->next->key < x) {
        p = p->next;
    }
    p->next = new lnode(x, p->next);    // p->next == nullptr or p->next->key >= x
}

// 6f - remove elements with key x
void remove(lnode *&n, int x) {
    // if list is empty
    if (n == nullptr) return;

    // if first element is x
    if (n->key == x) {
        lnode *p = n;
        n = n->next;
        delete p;
        return;
    }

    // if first element is not x
    lnode *p = n;
    while (p->next != nullptr) {
        if (p->next->key == x) {    // if next element is x
            lnode *q = p->next;     // temporary pointer to next element
            p->next = p->next->next;// overwrite next element
            delete q;               // delete temporary pointer
        } else {
            p = p->next;
        }
    }
}

// 6g - remove elements unsatisfying a condition
void filter(lnode *&n, bool (*cond)(int)) {
    // if list is empty
    if (n == nullptr) return;

    // go through list and remove elements unsatisfying a condition
    lnode *p = n;
    while (p->next != nullptr) {
        if (!cond(p->next->key)) {  // if next element unsatisfies a condition
            lnode *q = p->next;     // temporary pointer to next element
            p->next = p->next->next;// overwrite next element
            delete q;               // delete temporary pointer
        } else {
            p = p->next;
        }
    }
}

// 6h - destroy a linked list
void destroy(lnode *&n) {
    while (n != nullptr) {
        lnode *p = n;
        n = n->next;
        delete p;
    }
}

/////////////////////////
// 7 - reverse without new and delete
void reverse(lnode *&n) {
    lnode *p = nullptr;
    while (n != nullptr) {
        lnode *q = n;   // temporary save list (first element)
        n = n->next;    // move list to next element
        q->next = p;    // first element of list points to first element of reversed list
        p = q;          // 
    }
    n = p;
}

/////////////////////////
// 8 - merge sorted lists, amount of comparisons <= length (a+b)
lnode *merge(lnode *a, lnode *b) {
    lnode *p = nullptr; 
    lnode *q = nullptr; 

    if (a == nullptr) {
        return b;
    } else if (b == nullptr) {
        return a;
    }

    if (a->key < b->key) {
        p = a;
        q = p;
        a = a->next;
    } else {
        p = b;
        q = p;
        b = b->next;
    }
    // go through both lists and compare elements until both lists are empty
    while (a != nullptr && b != nullptr) {
        if (a->key < b->key) {
            q->next = a;
            q = q->next;
            a = a->next;
        } else {
            q->next = b;
            q = q->next;
            b = b->next;
        }
    }

    return p;
}

int main() {
    // lnode *list = new lnode(1, new lnode(2, new lnode(3, new lnode(4, new lnode(5)))));
    // print(list);
    // std::cout << "6b sum\t" << sum(list) << std::endl;
    // std::cout << "6c nth\t" << nth(list, 3) << std::endl;
    // std::cout << "6d insert 0\t";
    // insert(list, 0);
    // print(list);
    // std::cout << "6e ins after smaller 3\t";
    // insert_after_smaller(list, 3);
    // print(list);
    // std::cout << "6f remove 3\t";
    // remove(list, 3);
    // print(list);
    // std::cout << "6g filter even\t";
    // filter(list, [](int x) { return x % 2 == 0; });
    // print(list);
    // std::cout << "6h destroy\t";
    // destroy(list);
    // print(list);
    // std::cout << "7 reverse\t";
    // list = new lnode(1, new lnode(2, new lnode(3, new lnode(4, new lnode(5)))));
    // reverse(list);
    // print(list);
    std::cout << "8 merge\t";
    lnode *list1 = new lnode(1, new lnode(1, new lnode(2, new lnode(7, new lnode(9)))));
    lnode *list2 = new lnode(2, new lnode(4, new lnode(6, new lnode(8, new lnode(10)))));
    lnode *list3 = merge(list1, list2);
    print(list3);
}