#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main()
{
    void *t3 = malloc(500);
    void *t1 = malloc(4096);
    void *t2 = malloc(4097);
    t3 = malloc(4098);
    t3 = malloc(4098);
    t3 = malloc(4098);

    t3 = malloc(10000000);
    getchar();
}