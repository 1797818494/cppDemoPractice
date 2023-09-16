#include "vector.h"

int main()
{
    Vector<int> vec(2);
    vec.push_back(20);
    printf("cap is: %zu\n", vec.getCap());
    vec.push_back(40);
    vec.push_back(60);
    printf("cap is: %zu\n", vec.getCap());
    printf("len is %zu\n", vec.getLen());
    printf("value[2] %d\n", vec[2]);
    return 0;
}