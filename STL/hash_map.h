#include <iostream>
template<typename K, typename V>
class HashNode {
public:
    K k_;
    V v_;
    HashNode* next;
    HashNode(K k, V v) : k_(k), v_(v), next(NULL){}
};

template <class Key, class Value>
class HashMap
{
public:
    HashMap(int size);
    ~HashMap();
    bool insert(const Key& key, const Value& value);
    bool del(const Key& key);
    bool find(const Key& key, Value& value);
    Value& operator [](const Key& key);

private:
    HashNode<Key, Value> **table;
    unsigned int _size;
    Value ValueNULL;
};

template <class Key, class Value>
HashMap<Key, Value>::HashMap(int size) : _size(size)
{
    table = new HashNode<Key, Value>*[size];
    for(int i = 0; i < size; i++) {
        table[i] = NULL;
    }
}
template <class Key, class Value>
HashMap<Key, Value>::~HashMap() {
    for(int i = 0; i < _size; i++) {
        while(table[i] != NULL) {
            auto next = table[i]->next;
            delete table[i];
            table[i] = next;
        }
    }
    delete[] table;
}

template <class Key, class Value>
bool HashMap<Key, Value>::insert(const Key& key, const Value& value) {
    int index = key % _size; // 支持整数    
    for(auto node = table[index]; node != nullptr; node = node->next) {
        if(node->k_ == key) {
            return false;
        }
    }
    auto node = new HashNode(key, value);
    node->next = table[index];
    table[index] = node;
    return true;
}
template <class Key, class Value>
bool HashMap<Key, Value>::del(const Key& key) {
    int index = key % _size;
    HashNode<Key, Value>* pre_node = NULL;
    for(auto node = table[index]; node != nullptr; node = node->next) {
        if(node->k_ == key) {
            auto next = node->next;
            delete node;
            if(pre_node == NULL) {
                table[index] = next;
            } else {
                pre_node->next = next;
            }   
            return true;
        }
        pre_node = node;
    }
    return false;    

}
template<class Key, class Value>
bool HashMap<Key, Value>::find(const Key& key, Value& value) {
    int index = key % _size;
    for(auto node = table[index]; node != nullptr; node = node->next) {
        if(node->k_ == key) {
            value = node->v_;
            return true;
        }
    }
    return false;
}
template<class Key, class Value>
Value& HashMap<Key, Value>::operator [](const Key& key) {
    int index = key % _size;
    for(auto node = table[index]; node != nullptr; node = node->next) {
        if(node->k_ == key) {
            return node->v_;
           
        }
    }  
    return ValueNULL; 
}

