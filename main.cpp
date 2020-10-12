#include <cstdio>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

class Collection {
public:
    virtual bool isEmpty() const = 0;

    virtual void print(std::ostream& out) const = 0;

    virtual ~Collection() = 0;

    friend std::ostream& operator<< (std::ostream& out, const Collection& stack) {
        stack.print(out);
        return out;
    }
};

class Stack : public Collection {
public:
    virtual int pop() = 0;
    virtual void push(int x) = 0;
};

class Queue {
public:
    virtual int dequeue() = 0;
    virtual void enqueue(int x) = 0;
};

class ArrayList : public Stack {
    int size;
    int end;
    int* arr;

    void ensureCapacity() {
        if (end == size) {
            int newSize = size * 2;
            int* newArr = new int[newSize];
            delete[] arr;
            arr = newArr;
            size = newSize;
        }
    }
public:
    ArrayList() : ArrayList(16) {}

    ArrayList(int size): end(0), size(size), arr(new int[size]) {}

    ArrayList(const ArrayList& a) : ArrayList(a.size) {
        for (int i = 0; i < size; i++) {
            arr[i] = a.arr[i];
        }
    }

    ~ArrayList() override {
        delete[] arr;
    }

    int pop() {
        if (isEmpty()) {
            cout << "Stack underflow";
            exit(-1);
        }
        return arr[--end];
    }

    void push(int x) {
        ensureCapacity();
        arr[end++] = x;
    }

    bool isEmpty() const {
        return end == 0;
    }
    
    void print(std::ostream& out) const {
        if (isEmpty()) {
            out << " Empty ArrayList";
        } else {
            for (int i = 0; i < end; i++) {
                out << arr[i] << " ";
            }
        }
    }
};

class LinkedList : public Stack, public Queue {
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
    LinkedList() {
        head = nullptr;
        tail = nullptr;
    }
    bool isEmpty() const { return head == nullptr; }

    LinkedList(const LinkedList& l) {
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

    ~LinkedList() {
        delete head;
        tail = nullptr;
    }

    LinkedList& operator= (LinkedList list) {
        swap(*this, list);
        return *this;
    }

    friend void swap(LinkedList& l1, LinkedList& l2) {
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

    friend std::ostream& operator<< (std::ostream& out, const LinkedList& list) {
        list.print(out);
        return out;
    }

    friend std::istream& operator>> (std::istream& in, LinkedList& list) {
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

    int dequeue() override {
        if (isEmpty()) {
            std::cout << "dequeue from empty";
            exit(1);
        }
        Node* n = head;
        head = head->next;
        int res = n->x;
        delete n;
        return res;
    }

    void enqueue(int x) override {
        Node* n = Node::create(x);
        if (isEmpty()) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
};

void foo(Stack& l) {
    l.push(420);
}

void bar(Queue& q) {
    q.enqueue(123);
}

int main() {
    LinkedList list;
    list.push(42);
    std::cout << list << std::endl;

    bar(list);
    foo(list);

    ArrayList arrL;
    arrL.push(14);

    foo(arrL);

    Stack& stack = list;
    stack.push(12);
    std::cout << stack;

    return 0;
}
