#include <iostream>
#include <vector>
using namespace std;
template<template<typename, typename> class OutContainer = vector,
typename F, class R>
auto fmap(F&& f, R&& inputs) {
    typedef decay_t<decltype(f(*inputs.begin()))> result_type;
    OutContainer<result_type, allocator<result_type>> result;
    for(auto&& item : inputs) {
        result.push_back(f(item));
    }
    return result;
}
int add_1(int x) {
    return x + 1;
}
int main() {
    vector<int> v{1, 2, 3, 4, 5};
    auto result = fmap(add_1, v);
    for(auto&& item : result) {
        cout<<item<<" ";
    }
}