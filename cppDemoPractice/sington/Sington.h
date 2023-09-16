#include <iostream>
#include <boost/noncopyable.hpp>

template<typename T>
class Sington : boost::noncopyable {
    public:
    static T& instance() {
        pthread_once(&once_, &Sington::init);
        return *value_;
    }
    private:
        Sington();
        ~Sington();
        static void init() {
            printf("init\n");
            value_ = new T();
        }
    private:
        static T* value_;
        static pthread_once_t once_;

};

template<typename T>
T* Sington<T>::value_ = NULL;

template<typename T>
pthread_once_t Sington<T>::once_ = PTHREAD_ONCE_INIT;