#include <iostream>

template<typename T>
class SharedPtr{
    public:
    SharedPtr() : ptr_(NULL), cnt_(new size_t(0)){
    }
    explicit SharedPtr(T* ptr) : ptr_(ptr), cnt_(new size_t(1)) {
    }
    SharedPtr(SharedPtr& rhs) : ptr_(rhs.ptr_), cnt_(rhs.cnt_) {
        (*this->cnt_)++;
    }
    SharedPtr(SharedPtr&& rhs) : ptr_(rhs.ptr_), cnt_(rhs.cnt_) {
        rhs.ptr_ = NULL;
        rhs.cnt_ = NULL;
    }
    ~SharedPtr() {
        if(cnt_ == NULL) {
            return;
        }
        (*cnt_)--;
        if(cnt_ == 0) {
           delete ptr_;
           delete cnt_;
           // dangling pointer，指向已经销毁的对象或已经回收的地址。以下情况可能出现空悬指针
           // 没有初始化的指针就是野指针，一般情况下，编译器会进行警告。
           ptr_ = NULL;
           cnt_ = NULL;
        }
    }
    SharedPtr& operator=(const SharedPtr& rhs) {
        //特判
        if(this->ptr_ == rhs.ptr_) {
            return *this;
        }
        if(cnt_) {
        (*this->cnt_)--;
        if(*(this->cnt_) == 0) {
            delete this->ptr_;
            delete this->cnt_;
        } 
        this->ptr_ = rhs.ptr_;
        this->cnt_ = rhs.cnt_;
        (*this->cnt_)++;
        }
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& rhs) {
        if(this->ptr_ == rhs.ptr_) {
            return *this;
        }
        this->ptr_ = rhs.ptr_;
        this->cnt_ = rhs.cnt_;
        rhs.ptr_ = NULL;
        rhs.cnt_ = NULL;
        return* this;
    }
    T* get() const {
        return this->ptr_;
    }
    T& operator*() const {
        return  *(this->ptr_);
    }
    T* operator->() {
        return this->ptr_;
    }
    operator bool() const {
        //隐式转换
        return ptr_ != nullptr;
    }
    size_t use_count() const {
        return *cnt_;
    }
    bool unique() const {
        return *cnt_ == 1;
    }
    void swap(SharedPtr& rhs) {
        std::swap(*this, rhs);
    }
    private:
        T* ptr_;
        size_t* cnt_;
};
struct test{
    void testfunc() {
        printf("2");
    }
};
int main() {
    SharedPtr<int> ptr(new int(1));
    printf("%d", *ptr);
    SharedPtr<test> ptr1(new test());
    SharedPtr<test> tmp(new test());
    ptr1->testfunc();
    if(ptr) {
        printf("3");
    }
    printf("%ld", ptr1.use_count());
    SharedPtr<test> ptr2 = ptr1;
    printf("%ld", ptr1.use_count());
    ptr2 = std::move(ptr2);
    printf("%ld", ptr1.use_count());
    ptr2 = tmp;
    printf("%ld", ptr1.use_count());
    //1231221
    
}