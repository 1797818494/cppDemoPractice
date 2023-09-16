#include "object_pool.h" 
#include "memory"
#include <cstdio>
using namespace ig;

struct initialer{
    void operator()(int* n) const {
        *n = 1;
    }
};
int main() {
    SharedObjectPool<int, 10, initialer>& pool = SharedObjectPool<int, 10, initialer>::Instance();
    std::shared_ptr<int> ptr = pool.Get();
    printf("%d", *ptr);
}