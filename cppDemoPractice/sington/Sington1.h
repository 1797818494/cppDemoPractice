#pragma once
#include <stdexcept>
/*
    饿汉: 自己抢先创建，这样就没有多线程问题

    饱汉: 就是等到用户调用才创建

    多线程: C++11后利用static创建线程安全
*/
template <typename T>
class Singleton {
    public:
        template<typename... Args>
        static T* Instance(Args&&... args) {
            if(m_pInstance == nullptr) {
                m_pInstance = new T(std::forward<Args>(args)...);
            }
            return m_pInstance;
        }
        static T* GetInstance() {
            if(m_pInstance == nullptr) {
                throw std::logic_error("the instance is not init");
            }
            return m_pInstance;
        }
        static void DestroyInstance() {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    
    private:
        static T* m_pInstance;
};

template<typename T>
T* Singleton<T>::m_pInstance = nullptr;