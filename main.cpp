#include <algorithm>
#include <iostream>
#include <functional>
#include <exception>

template<typename T>
void showcaseStdHash(T v) {
    std::cout << std::hash<T>{}(v) << std::endl;
}

struct Int {
    int i = 0;
};

namespace std {
    template<>
    struct hash<Int> {
        int operator() (Int i) {
            return i.i;
        }
    };
}
/*
int main() {
    showcaseStdHash<std::string>("abc");
    showcaseStdHash<int>(42);

    Int i;
    i.i = 42;
    showcaseStdHash<Int>(i);
}*/

using namespace std;

template<typename T>
struct SimpleIterator {
    virtual bool hasNext() = 0;
    virtual const T& next() = 0;
    virtual ~SimpleIterator() {};
};

template<typename T>
class Collection {
public:
    virtual bool isEmpty() const = 0;

    virtual void print(std::ostream& out) const {
        SimpleIterator<T>* iter = iterator();
        while (iter->hasNext()) {
            out << iter->next() << "; ";
        }
        delete iter;
    }

    virtual SimpleIterator<T>* iterator() const {
        cout << "unimplemented iterator()";
        exit(-1);
    }

    virtual ~Collection() {};

    friend std::ostream& operator<< (std::ostream& out, const Collection& stack) {
        stack.print(out);
        return out;
    }
};

template<typename T>
class CppIterator {

};

template<typename K, typename V>
struct Pair {
    K key;
    V value;

public:
    Pair() = default;
    Pair(K key, V value) {
        this->key = key;
        this->value = value;
    }

    friend std::ostream& operator<< (std::ostream& out, const Pair<K, V>& p) {
        out << "key: " << p.key << "; value: " << p.value << endl;
        return out;
    }
};

template<typename K, typename V>
class Map : public Collection<Pair<K, V>> {
    virtual V get(K key) = 0;
    virtual bool add(K key, V value) = 0;
};

template<typename K, typename V, int N>
class DummyMap : public Map<K, V> {

    Pair<K, V> storage[N];
    int size = 0;

    int findPlace(K key) {
        for (int i = 0; i < size; i++) {
            if (storage[i].key == key) {
                return i;
            }
        }
        for (Pair<K, V>* p = storage; p != storage + N; p++) {
            if (p->key == key) {
                return p-storage;
            }
        }
        return -1;
    }
public:
    //class CppIter {
    //    int i;
    //    const DummyMap<K, V, N>& map;
//
    //public:
    //    CppIter(const DummyMap<K, V, N>& map, int i = 0) : map(map), i(i) {}
//
    //    bool operator!=(const CppIter& that) const { /*...*/ }
    //    CppIter& operator++() {/*...*/}
    //    Pair<K, V>* operator->() {/*...*/}
    //};
//
    //CppIter begin() {
    //    return CppIter(*this);
    //}
    //CppIter end() {
    //    return CppIter(*this, N);
    //}

    struct KeyNotFound : std::exception {
        K key;
        explicit KeyNotFound(const K& key) : key(key) {}

        const char* what() const override {
            return "No key found!";
        }
    };

    bool isEmpty() const override {
        return size == 0;
    }
    V get(K key) override {
        int i = findPlace(key);
        if (i != -1) {
            return storage[i].value;
        }
        throw KeyNotFound(key);
    }
    bool add(K key, V value) override {
        int i = findPlace(key);
        if (i != -1) {
            storage[i].value = value;
            return true;
        } else {
            int newIndex = size++;
            if (newIndex >= N) {
                cout << "storage overflow";
                exit(-1);
            }
            storage[newIndex].key = key;
            storage[newIndex].value = value;
            return false;
        }
    }

    class Iter : public SimpleIterator<Pair<K, V>> {
        int i = 0;
        const DummyMap<K, V, N>& map;
    public:
        Iter(const DummyMap<K, V, N>& map) : map(map) {}
        bool hasNext() override {
            return i < map.size;
        }
        const Pair<K, V>& next() override {
            return map.storage[i++];
        }
    };

    SimpleIterator<Pair<K, V>>* iterator() const override {
        return new Iter(*this);
    }
};

void showcaseIterators() {
    int a1, a2;
    cin >> a1 >> a2;
    string s1, s2;
    cin >> s1 >> s2;

    DummyMap<int, string, 2> i2s;
    i2s.add(a1, s1);
    i2s.add(a2, s2);

    //for (auto i = i2s.begin(); i != i2s.end(); ++i) {
    //    cout << i->key << " -> " << i->value << endl;
    //}

    auto iter1 = i2s.iterator();
    while (iter1->hasNext()) {
        const auto& p = iter1->next();
        cout << p.key << " -> " << p.value << endl;
    }
    delete iter1;

    i2s.get(124);
    //..
}

struct RAII_pointer {
    int* p;
    RAII_pointer() : p(new int) {}
    ~RAII_pointer() { delete p; }
};

int main() {
    try {
        RAII_pointer rp;
        *rp.p = 42;
        showcaseIterators();

        cout << "after showcase";
    } catch (std::exception& e) {
        cout << e.what();
    }
    cout << "after try";
}

class Stack : public Collection<int> {
public:
    virtual int pop() = 0;
    virtual void push(int x) = 0;
};

class Queue : public Collection<int> {
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

    int pop() override {
        if (isEmpty()) {
            cout << "Stack underflow";
            exit(-1);
        }
        return arr[--end];
    }

    void push(int x) override {
        ensureCapacity();
        arr[end++] = x;
    }

    bool isEmpty() const override {
        return end == 0;
    }
    
    void print(std::ostream& out) const override {
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

    ~LinkedList() override {
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

/*int main() {
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
}*/
