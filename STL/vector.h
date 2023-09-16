#include <iostream>

template <typename T>
class Vector
{
public:
    explicit Vector(size_t n)
    {
        cap = n;
        len = 0;
        arr = new T[n];
    }
    Vector(const Vector &v)
    {
        this->arr = new T[v.cap];
        this->cap = v.cap;
        this->len = v.len;
        std::copy(v.arr, v.arr + len, this->arr);
    }
    Vector &operator=(const Vector &v)
    {
        if (*this == v)
        {
            return *this
        }
        if (this->arr != NULL)
        {
            delete[] this->arr;
            this->cap = 0;
            this->len = 0;
        }
        this->arr = new T[v.cap];
        this->cap = v.cap;
        this->len = v.len;
        std::copy(v.arr, v.arr + len, this->arr);
        return *this;
    }
    void push_back(T val)
    {
        if (cap == len)
        {
            rebalance();
        }
        len++;
        *(T *)(arr + len - 1) = val;
    }
    void rebalance()
    {
        cap = cap * 2;
        T *arr_new = new T[cap];
        std::copy(arr, arr + len, arr_new);
        delete[] arr;
        arr = arr_new;
    }
    void pop_back()
    {
        len--;
    }
    T &operator[](int n)
    {
        return arr[n];
    }
    size_t getCap() const
    {
        return cap;
    }
    size_t getLen() const
    {
        return len;
    }
    ~Vector()
    {
        delete[] arr;
        arr = NULL;
    }

private:
    size_t cap;
    size_t len;
    T *arr;
};