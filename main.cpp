#include <cstdio>
#include <algorithm>
#include <iostream>

class Node {
    int x;
    Node* next;

    Node(int x) {
        next = nullptr;
        this->x = x;
    }

    friend class List;

public:
    static Node* create(int x){
        return new Node(x);
    }

    ~Node() {
        delete next;
    }

    friend std::ostream& operator<< (std::ostream& out, const Node& node) {
        out << node.x;
        return out;
    }
    friend std::ostream& operator<< (std::ostream& out, const List& list);
};

class List {
    Node* head;
    Node* tail;

public:
    List() {
        head = nullptr;
        tail = nullptr;
    }
    bool isEmpty() const { return head == nullptr; }

    List(const List& l) {
        if (l.isEmpty()) {
            head = tail = nullptr;
        } else {
            head = Node::create(l.head->x);
            Node* prev = head;
            Node* p = l.head->next;
            while (p != nullptr) {
                prev->next = Node::create(p->x);
                prev = prev->next;
                p = p->next;
            }
            tail = prev;
        }
    }

    ~List() {
        delete head;
        tail = nullptr;
    }

    List& operator= (List list) {
        swap(*this, list);
        return *this;
    }

    friend void swap(List& l1, List& l2) {
        using std::swap;
        swap(l1.head, l2.head);
        swap(l1.tail, l2.tail);
    }

    void push(int x) {
        Node* n = Node::create(x);
        if (isEmpty()) {
            head = tail = n;
        } else {
            n->next = head;
            head = n;
        }
    }

    void append(int x) {
        Node* n = Node::create(x);
        if (isEmpty()) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }

    void operator+= (int x);

    friend std::ostream& operator<< (std::ostream& out, const List& list) {
        if (list.isEmpty()) {
            out << "Empty";
        } else {
            Node* ptr = list.head;
            while (ptr != nullptr) {
                out << *ptr << "; ";
                ptr = ptr->next;
            }
        }
        return out;
    }

    friend std::istream& operator>> (std::istream& in, List& list) {
        int x;
        in >> x;
        list += x;
        // TODO: implement cycle?
    }
};

void List::operator+=(int x) {
    this->append(x);
}

void foo(List list) {
    list.push(6);
}

int main() {
    int n, k;
    std::cin >> n >> k;
    std::cout << n << k << std::endl;

    List l;
    List l2;
    l2.push(42);

    l.push(n);
    l.push(5);

    std::cout << l;

    l2 = l;

    return 0;
}
