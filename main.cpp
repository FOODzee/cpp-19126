#include <cstdio>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

class List {
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
};

class Stack : public List {
    int depth;
public:
    Stack() : depth(0) {}

    int pop() {
        if (isEmpty()) {
            std::cout << "pop from empty";
            exit(1);
        }
        depth--;
        Node* n = head;
        head = head->next;
        int x = n->x;
        delete n;
        return x;
    }

    void push(int x) {
        Node* n = Node::create(x);
        if (isEmpty()) {
            head = tail = n;
        } else {
            n->next = head;
            head = n;
        }
        depth++;
    }

    bool isEmpty() const {
        return List::isEmpty();
    }

    virtual void print(std::ostream& out) const override {
        List::print(out);
        out << "; depth = " << depth;
    }

    friend std::ostream& operator<< (std::ostream& out, const Stack& stack) {
        stack.print(out);
        return out;
    }
};

int main() {
    Stack st;
    st.push(42);
    std::cout << st << std::endl;

    List& l = st;
    std::cout << l;

    return 0;
}
