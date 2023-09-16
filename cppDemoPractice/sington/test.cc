#include "Sington.h"
#include <thread>

void func() {
    int& t = Sington<int>::instance();
}
int main() {
    std::thread t1(func);
    std::thread t2(func);
    std::thread t3(func);
    t1.join();
    t2.join();
    t3.join();
}