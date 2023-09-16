#include <memory>
#include <mutex>
#include <thread>
class obj {};
typedef std::shared_ptr<obj> objPtr;
class CopyOnWrite{
    public:
        void read() {
            objPtr tmp;
            {
                std::lock_guard<std::mutex> lock(mutex_);
                tmp = ptr_;
            }
            // read operate
        }
        void write() {
            bool flag = false;
            {
            std::lock_guard<std::mutex> lock(mutex_);
            if(!ptr_.unique()) {
                ptr_.reset(new obj(*ptr_));
                bool flag = true;
            }
            //write operate
            }
            if(flag) {
                printf("write on copy");
            }
        }

    private:
        objPtr ptr_ = std::make_shared<obj>();
        mutable std::mutex mutex_;
};
void func1(CopyOnWrite* obj) {
    obj->read();
}
void func2(CopyOnWrite* obj) {
    obj->write();
}

int main() {
    CopyOnWrite obj;
    std::thread t1(func1, &obj);
    std::thread t2(func2, &obj);
    t1.join();
    t2.join();
    return 0;
}