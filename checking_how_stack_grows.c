#include <stdio.h>

void c() {
    int x;
    printf("c: %p\n", (void *)&x);
}

void b() {
    int x;
    printf("b: %p\n", (void *)&x);
    c();
}

void a() {
    int x;
    printf("a: %p\n", (void *)&x);
    b();
}

int main() {
    int x;
    printf("main: %p\n", (void *)&x);
    a();
}
