#include <atomic>
#include <future>
#include <memory>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>
#include <unistd.h>

#include "concurrent_queue.h"
#include "function_wrapper.h"

class ThreadPool {
    public:
        ThreadPool() {
            std::size_t n = std::thread::hardware_concurrency();
            try {
                for (std::size_t i = 0; i < n; ++i) {
                    threads_.emplace_back(&ThreadPool::worker_thread, this);
                }
            } catch(...) {
                done_ = true;
                for(auto& x : threads_) {
                    if(x.joinable()) {
                        x.join();
                    }
                }
                throw;
            }
        }
        ~ThreadPool() {
            done_ = true;
            for(auto& x : threads_) {
                if(x.joinable()) {
                    x.join();
                }                                   
            }
        }

        template<typename F>
        std::future<std::invoke_result_t<F>> submit(F f) {
            std::packaged_task<std::invoke_result_t<F>()> task(std::move(f));
            std::future<std::invoke_result_t<F>> res(task.get_future());
            if(local_queue_) {
                local_queue_->push(std::move(task));
            } else {
                pool_queue_.push(std::move(task));
            } 
            return res;
        }
    private:
        void
         worker_thread() {
            local_queue_.reset(new std::queue<FunctionWrapper>);
            while(!done_) {
                FunctionWrapper task;
                if(local_queue_ && !local_queue_->empty()) {
                    task = std::move(local_queue_->front());
                    local_queue_->pop();
                    task();
                } else if(pool_queue_.try_pop(task)) {
                    task();
                } else {
                    std::this_thread::yield();
                }
            }
        }
    private:
        std::atomic<bool> done_ = false;
        ConcurrentQueue<FunctionWrapper> pool_queue_;
        // ODR
        // 不加inline的函数或者变量在整个程序里只允许有一个定义，而有inline的函数和变量在每一个使用它的翻译单元里都有一处定义。众所周知c++处理头文件的方式是把头文件内容插入到代码文件中#include的位置，而每一个代码文件都是一个翻译单元。
        // static 在线程中不用每次都初始化 local_queue_ threadlocal就是每一个线程中都是“static”且线程独立
        // 众所周知c++处理头文件的方式是把头文件内容插入到代码文件中#include的位置，
        // 而每一个代码文件都是一个翻译单元。所以从工程的角度上说加inline的作用很简单，
        // 就是允许你在头文件中定义函数和变量，而不需要把定义和声明拆开。这对于写header-only的库很有用。

        // C++17特性暂时省略
        inline static thread_local std::unique_ptr<std::queue<FunctionWrapper>> 
            local_queue_;
        std::vector<std::thread> threads_;
};

int main() {
    ThreadPool threadlocal;
    std::future<int> ft = threadlocal.submit([] {
        return 1;
    });
    printf("%d", ft.get());
    sleep(1);
    return 0;
}