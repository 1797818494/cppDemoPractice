#include "vector.hpp"
#include <string>
#include <iostream>
int main()
{
    my::vector<::std::string> vec;
    vec.push_back("hello");
    vec.emplace_back();
    vec.emplace_back("hello", 4);
    for (const auto &v : vec)
    {
        std::cout << v << std::endl;
    }
    return 0;
}