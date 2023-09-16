#include <iostream>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <future>
#include <thread>
#include <utility>
#include <memory>
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
        template<class F, class... Args>
        auto submit(F&& f, Args&&... args) {
            using RT = std::invoke_result_t<F, Args...>;
            // packaged_task only can move, so need shared_ptr包装
            // cpp11 lamdba不支持移动捕获, 支持值捕获和引用
            // function<void>  If your lambda function captures non-copyable objects, such as references
            // std::packaged_task 不允许拷贝构造，不能直接传入 lambda，
            // 因此要借助 std::shared_ptr
            auto task = std::make_shared<std::packaged_task<RT()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
            // 但 std::bind 会按值拷贝实参，因此这个实现不允许任务的实参是 move-only 类型
            {
             std::lock_guard<std::mutex> l(m_);
             q_.emplace([task]() { (*task)(); });  // 捕获指针以传入 std::packaged_task
            }
            cv_.notify_one();
            return task->get_future();
        }
    private:
        std::mutex m_;
        std::condition_variable cv_;
        bool done_ = false;
        std::queue<std::function<void()>> q_;
};

void func(int arg) {
    printf("hello world {%d}\n", arg);
}

int main() {
    ThreadPool threadPool(2);
    auto ft = threadPool.submit(func, 1);
    ft.get();
    return 0;

}