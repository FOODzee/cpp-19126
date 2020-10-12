#include <cstdio>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;


class Stack {
public:
    virtual int pop() = 0;
    virtual void push(int x) = 0;

    virtual bool isEmpty() const = 0;

    virtual void print(std::ostream& out) const = 0;

    friend std::ostream& operator<< (std::ostream& out, const Stack& stack) {
        stack.print(out);
        return out;
    }
};

class List : public Stack {
protected:
    class Node {
    public:
        int x;
        Node* next;

        Node(int x) {
            next = nullptr;
            this->x = x;
        }

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
    };

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

    void append(int x) {
        Node* n = Node::create(x);
        if (isEmpty()) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }

    void operator+=(int x) {
        this->append(x);
    }

    virtual void print(std::ostream& out) const {
        if (this->isEmpty()) {
            out << "Empty";
        } else {
            Node* ptr = head;
            while (ptr != nullptr) {
                out << *ptr << "; ";
                ptr = ptr->next;
            }
        }
    }

    friend std::ostream& operator<< (std::ostream& out, const List& list) {
        list.print(out);
        return out;
    }

    friend std::istream& operator>> (std::istream& in, List& list) {
        int x;
        while (in >> x) {
            list += x;
        }
    }

    operator bool() const {
        return isEmpty();
    }

    int pop() override {
        if (isEmpty()) {
            std::cout << "pop from empty";
            exit(1);
        }
        Node* n = head;
        head = head->next;
        int x = n->x;
        delete n;
        return x;
    }

    void push(int x) override {
        Node* n = Node::create(x);
        if (isEmpty()) {
            head = tail = n;
        } else {
            n->next = head;
            head = n;
        }
    }
};

class A {
protected:
    const int size;
    int* arr;
public:
    A(int size): size(size), arr(new int[size]) {}

    A(const A& a) : A(a.size) {
        for (int i = 0; i < size; i++) {
            arr[i] = a.arr[i];
        }
    }

    virtual ~A() {
        delete[] arr;
    }
};

class B : public A {
    int* arr2;
public:
    B(int size): A(size), arr2(new int[size]) {}

    B(const B& b) : B(b.size) {
        for (int i = 0; i < size; i++) {
            arr[i] = b.arr[i];
            arr2[i] = b.arr2[i];
        }
    }

    ~B() override {
        delete[] arr2;
    }
};

void foo(Stack& l) {
    l.push(420);
}

void foo(A* a) {
    // actions
    delete a;
}

int main() {
    B* b = new B(2);
    foo(b);

    List list;
    list.push(42);
    std::cout << list << std::endl;

    foo(list);

    Stack& stack = list;
    stack.push(12);
    std::cout << stack;

    return 0;
}
