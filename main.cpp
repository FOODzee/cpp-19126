#include <cstdio>

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

    void operator+= (int x) {
        this->append(x);
    }
};

void foo(List list) {
    list.push(6);
}

int main() {
    int n;
    scanf("%d", &n);

    List l;
    l.push(n);
    l.push(5);
    foo(l);

    l.append(42);
    l += 30;

    return 0;
}
