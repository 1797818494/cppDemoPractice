#include <iostream>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <unistd.h>


class ThreadPool {
    public: 
        explicit ThreadPool(std::size_t n) {
            for(std::size_t i = 0; i < n; ++i) {
                std::thread{[this] {
                    std::unique_lock<std::mutex> l(m_);
                    while(true) {
                        if(!q_.empty()) {
                            auto task = std::move(q_.front());
                            q_.pop();
                            l.unlock();
                            task();
                            l.lock();
                        } else if(done_) {
                            break;
                        } else {
                            cv_.wait(l);
                        }
                    }
                }}.detach();
            }
            
        }
        ~ThreadPool() {
            {
            std::lock_guard<std::mutex> l(m_);
            done_ = true;
            }
            cv_.notify_all();
        }
        template<typename F>
        void submit(F&& f) {
            {
               std::lock_guard<std::mutex> l(m_);
               q_.emplace(std::forward<F>(f));
            }
            cv_.notify_one();
        }
    private:
        std::mutex m_;
        std::condition_variable cv_;
        bool done_ = false;
        std::queue<std::function<void()>> q_;
};

void func() {
    printf("hello world 1\n");
}

int main() {
    ThreadPool threadPool(2);
    threadPool.submit(func);

    threadPool.submit([] {
        printf("hello world 2\n");
    });
    sleep(1);
    return 0;

}