#include<iostream>
#include<array>
#include "assert.h"
template<typename T>
inline constexpr size_t memory_chunk_size = 64;

template<typename T>
class memory_chunk {
public:
union node {
    T data;
    node* next;
    node() {}
    ~node() {}
    node(const node&) = delete;
    node& operator=(const node&)  = delete;
};
memory_chunk(memory_chunk* next_chunk);
node* get_free_nodes() {
    return storage_.data();
}
memory_chunck* get_next() const {
    return next_chunk_;
}

private:
    memory_chunk* next_chunk_{};
    std::array<node, memory_chunk_size<T>> storage_;
};

template<typename T>
memory_chunk<T>::memory_chunk(memory_chunk* next_chunk) 
: next_chunk_(next_chunk) {
    for(size_t i = 0; i < storage_.size(); ++i) {
        storage_[i].next = &storage_[i + 1];
    }
    storage_[storage_.size() - 1].next = nullptr;
}

template<typename T>
class memory_pool{
public:
    using node = typename memory_chunk<T>::node;
    memory_pool() = default;
    memory_pool(const memory_pool&) = delete;
    memory_pool& operator=(const memory_pool&) = delete;
    ~memory_pool();
    T* allocate();
    void deallocate(T* ptr);
private:
    node* free_list_{};
    memory_chunk<T>* chunk_list_{};
};

template<typename T>
memory_pool<T>::~memory_pool() {
    while(chunk_list_) {
        memory_chunk<T>* chunk = chunk_list_;
        chunk_list_ = chunk_list_->get_next();
        delete chunk;
    }
}


template<typename T>
T* memory_pool<T>::allocate() {
    if(free_list_ == nullptr) {
        chunk_list_ = new memory_chunk<T>(chunk_list_);
        free_list_ = chunk_list_->get_free_nodes();
    }
    T* result = &free_list_->data;
    free_list_ = free_list_->next;
    return result;
}


template <typename T>
void memory_pool<T>::deallocate(T* ptr)
{
 auto free_item =
 reinterpret_cast<node*>(ptr);
 free_item->next = free_list_;
 free_list_ = free_item;
}


class Obj {
public:
 int a;
 void* operator new(size_t size);
 void operator delete(
 void* ptr) noexcept;
};
memory_pool<Obj> obj_pool;
void* Obj::operator new(size_t size) {
    assert(size == sizeof(Obj));
    return obj_pool.allocate();
}
void Obj::operator delete(void* ptr) noexcept {
    obj_pool.deallocate(static_cast<Obj*>(ptr));
}
// allocator
template<typename T>
memory_pool<T>& get_memory_pool() {
    thread_local memory_pool<T> pool;
    return pool;
}

template<typename T, typename Base = std::allocator<T>>
struct pooled_allocator : private Base{
    T* allocate(size_t n) {
        if(n == 1) {
            return get_memory_pool<T>().alloacte();
        } else {
            return Base::allocate(n);
        }
    }
    void deallocate(T* ptr, size_t n) {
        if(n == 1) {
            return get_memory_pool<T>().deallocate(p);
        }
    } else {
        return Base::deallocate(ptr, n);
    }
};





int main() {
    
}


