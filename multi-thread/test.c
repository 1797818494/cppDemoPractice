#include <stdlib.h>
#include <stdio.h>

static int func()
{
    static int i = 0;
    ++i;

    if (i == 2) {
        return 1;
    }
    return 0;
}

static int func3()
{
    return func();
}

static int func2()
{
    return func();
}

static int func1()
{
    return func();
}

int main()
{
    int ret = 0;

    ret += func1();
    ret += func2();
    ret += func3();

    return ret;
}
