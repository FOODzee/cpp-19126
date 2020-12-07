#include <algorithm>
#include <iostream>
#include <sstream>
#include <functional>
#include <exception>

//// multi-threading

#include <thread>
#include <chrono>
#include <mutex>


class tsPrinter {
    std::mutex printMTX;
    std::ostream& out;
public:
    explicit tsPrinter(std::ostream& out) : out(out) {}

    template<typename T>
    void println(const T& val) {
        std::lock_guard<std::mutex> lock(printMTX); // RAII
        out << val << std::endl;
    }
};
tsPrinter tsp(std::cout);

void myThreadBody(int iterations) {
    for (int i = 0; i < iterations; i++) {
        tsp.println("Hello world from new thread");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main3() {
    std::thread newThread(myThreadBody, 100);
    for (int i = 0; i < 100; i++) {
        tsp.println("Hello world from main thread");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    newThread.join();
    return 0;
}

///////////////

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

    virtual void print(std::ostream& out) {
        SimpleIterator<T>* iter = iterator();
        while (iter->hasNext()) {
            out << iter->next() << "; ";
        }
        delete iter;
    }

    virtual SimpleIterator<T>* iterator() const {
        std::cout << "unimplemented iterator()";
        exit(-1);
    }

    virtual ~Collection() {};

    friend std::ostream& operator<< (std::ostream& out, const Collection& stack) {
        stack.print(out);
        return out;
    }
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
    class CppIter {
        int i;
        DummyMap<K, V, N>* map;

    public:
        CppIter(DummyMap<K, V, N>* map, int i = 0) : map(map), i(i) {}

        bool operator!=(const CppIter& that) const {
            return this->map != that.map || this->i != that.i;
        }
        CppIter& operator++() {
            i++;
            return *this;
        }
        Pair<K, V>* operator->() { return &map->storage[i]; }
        Pair<K, V> operator*() { return map->storage[i]; }
    };

    CppIter begin() {
        return CppIter(this);
    }
    CppIter end() {
        return CppIter(this, N);
    }

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
/*
void showcaseIterators() {
    int a1, a2;
    cin >> a1 >> a2;
    string s1, s2;
    cin >> s1 >> s2;

    DummyMap<int, string, 2> i2s;
    i2s.add(a1, s1);
    i2s.add(a2, s2);

    for (auto i = i2s.begin(); i != i2s.end(); ++i) {
        cout << i->key << " -> " << i->value << endl;
    }
    cout << endl;



    for (auto i : i2s) {
        cout << i.key << " -> " << i.value << endl;
    }
    cout << endl;

    auto iter1 = i2s.iterator();
    while (iter1->hasNext()) {
        const auto& p = iter1->next();
        cout << p.key << " -> " << p.value << endl;
    }
    cout << endl;
    delete iter1;

    i2s.get(124);
    //..
}
*/
struct RAII_pointer {
    int* p;
    RAII_pointer() : p(new int) {}
    ~RAII_pointer() { delete p; }
};
/*
int main2() {
    try {
        RAII_pointer rp;
        *rp.p = 42;
        showcaseIterators();

        cout << "after showcase";
    } catch (std::exception& e) {
        cout << e.what();
    }
    cout << "after try";
    return 0;
}
*/
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

    ArrayList(ArrayList&& a) {
        size = std::move(a.size);
        end  = std::move(a.size);
        arr  = std::move(a.arr);
        a.arr = nullptr;
        a.size = a.end = 0;
    }

    ~ArrayList() override {
        delete[] arr;
    }

    ArrayList& operator=(ArrayList that) {
        swap(*this, that);
        return *this;
    }

    friend void swap(ArrayList& a, ArrayList& b) {
        using std::swap; // allow ADL (Argument-Dependent-Lookup)
        swap(a.end, b.end);
        swap(a.size, b.size);
        swap(a.arr, b.arr);
    }

    int pop() override {
        if (isEmpty()) {
            std::cout << "Stack underflow";
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
    
    void print(std::ostream& out) override {
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

    std::mutex mtx;
    std::condition_variable elementAdded;

public:
    LinkedList() noexcept {
        head = nullptr;
        tail = nullptr;
    }

    /** Not thread-safe! Use under lock only! */
    bool isEmpty() const override {
        return head == nullptr;
    }

    LinkedList(LinkedList& l) {
        std::lock_guard lock(l.mtx);
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
        elementAdded.notify_all();
        return *this;
    }

    friend void swap(LinkedList& l1, LinkedList& l2) {
        using std::swap;
        std::unique_lock lock1(l1.mtx, std::defer_lock);
        std::unique_lock lock2(l2.mtx, std::defer_lock);
        std::lock(lock1, lock2);
        swap(l1.head, l2.head);
        swap(l1.tail, l2.tail);
    }

    void print(std::ostream& out) override {
        std::lock_guard lock(mtx);
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

    friend std::ostream& operator<< (std::ostream& out, LinkedList& list) {
        list.print(out);
        return out;
    }

    friend std::istream& operator>> (std::istream& in, LinkedList& list) {
        int x;
        //std::lock_guard lock(list.mtx); // this requires recursive mutex, but conditional_variable works only for regular ones
        while (in >> x) {
            list.enqueue(x);
        }
        return in;
    }

    operator bool() const {
        return isEmpty();
    }

    int pop() override {
        std::lock_guard lock(mtx);
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
        std::lock_guard lock(mtx);
        Node* n = Node::create(x);
        if (isEmpty()) {
            head = tail = n;
        } else {
            n->next = head;
            head = n;
        }
        elementAdded.notify_all();
    }

    int dequeue() override {
        std::lock_guard lock(mtx);
        if (isEmpty()) {
            throw std::exception("Empty list!");
        }
        Node* n = head;
        n->next = nullptr;
        head = head->next;
        int res = n->x;
        delete n;
        return res;
    }

private:
    bool noMoreElements = false;
public:
    struct StopPolling : std::exception {
        StopPolling() : std::exception("list stopped polling") {}
    };

    int poll() {
        std::unique_lock lock(mtx);
        while (isEmpty()) {
            if (noMoreElements) {
                throw StopPolling();
            }
            elementAdded.wait(lock);
        }
        Node* n = head;
        n->next = nullptr;
        head = head->next;
        int res = n->x;
        delete n;
        return res;
    }

    void setNoMoreElements() {
        noMoreElements = true;
        elementAdded.notify_all();
    }

    void enqueue(int x) override {
        std::lock_guard lock(mtx);
        Node* n = Node::create(x);
        if (isEmpty()) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
        elementAdded.notify_all();
    }
};

void foo(Stack& l) {
    l.push(420);
}

void bar(Queue& q) {
    q.enqueue(123);
}

LinkedList list;
tsPrinter printer(std::cout);

void pollExample(const std::string& threadName, int iters) {
    try {
        for (int n = 0; n < iters; n++) {
            int i = list.poll();
            std::stringstream ss;
            ss << threadName << ": " << i;
            printer.println(ss.str());
        }
    } catch (LinkedList::StopPolling& stopped) {
        printer.println(stopped.what());
    }
}
int main() {
    std::thread worker1(pollExample, "w1", 200);
    std::thread worker2(pollExample, "w2", 300);

    std::cin >> list;
    std::cout << list;

    list.setNoMoreElements();

    worker1.join();
    worker2.join();

    return 0;
}
