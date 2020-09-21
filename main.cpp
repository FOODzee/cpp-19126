#include <cstdio>
#include <cstdlib>

struct List {
    int x;
    List* next;
};

List* createList(int x) {
    List* n = (List *) malloc(sizeof(List));
    n->next = nullptr;
    n->x = x;
    return n;
}

void destroyList(List* list) {
    if (list -> next != nullptr) {
        destroyList(list->next);
    }
    free(list);
}

int main() {
    int n;
    scanf("%d", &n);
    List* list = createList(n);
    printf("%d", list->x);
    destroyList(list);
    return 0;
}
