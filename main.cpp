#include <cstdio>

struct List {
    int x;
private:
    List* next;

    List(int x) {
        next = nullptr;
        this->x = x;
    }

public:
    static List* create(int x){
        return new List(x);
    }

    ~List() {
        delete next;
    }

    List* push(int x) {
        List* n = new List(x);
        n->next = this;
        return n;
    }
};

int main() {
    int n;
    scanf("%d", &n);
    List* list = List::create(n);
    printf("%d", list->x);
    scanf("%d", &n);
    list = list->push(n);
    delete list;

    return 0;
}
