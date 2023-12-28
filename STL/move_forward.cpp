#include <iostream>

//move
//https://andycong.top/2020/07/03/%E6%B5%85%E8%B0%88std-move%E5%92%8Cstd-forward%E5%8E%9F%E7%90%86/
template<typename T>
class remove_reference {
    typedef T type;
};

template<typename T>
class remove_reference<T&> {
    typedef T type;
};
template<typename T>
class remove_reference<T&&> {
    typedef T type;
};

template<typename T>
typename remove_reference<T>::type move(T t) {
    return static_cast<typename remove_reference<T>::type&&>(t);
}
// forward
// https://blog.csdn.net/albertsh/article/details/118886009
// 右值引用是左值
// 所以我们要在T&&时把这个属性（&&还是&）截住
template<typename T>
T&& forward(typename remove_reference<T>::type& arg) {
    return static_cast<T&&>(arg);
}
template<typename T>
T&& forward(typename remove_reference<T>::type&& arg) {
    return static_cast<T&&>(arg);
}





