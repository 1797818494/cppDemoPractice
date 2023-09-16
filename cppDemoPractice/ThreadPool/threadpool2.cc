#include <atomic>
#include <future>
#include <thread>
#include <type_traits>
#include <vector>

#include "concurrent_queue.h"
#include "function_wrapper.h"

class ThreadPool{
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
                // 必须对thread进行join或者detach，不然会在~thread时调用terminate终止程序
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
            q_.push(std::move(task));
            return res;
        }
    private:
        void worker_thread() {
            while(!done_) {
                FunctionWrapper task;
                if(q_.try_pop(task)) {
                    task();
                } else {
                    std::this_thread::yield();
                }
            }
        }
    private:
        std::atomic<bool> done_ = false;
        ConcurrentQueue<FunctionWrapper> q_;
        std::vector<std::thread> threads_; //  要在 done_ 和 q_ 之后声明
};

int main() {
    ThreadPool threadPool;
    std::future<int> ft = threadPool.submit([]() {
        return 1;
    });
    printf("ans: {%d}", ft.get());
}