#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
template <typename T>
class ConcurrentQueue {
 public:
  ConcurrentQueue(int cap) : capicity_(cap) {}
  ~ConcurrentQueue() = default;
  ConcurrentQueue(const ConcurrentQueue &queue) = delete;
  ConcurrentQueue &operator=(const ConcurrentQueue &queue) = delete;

 public:
  void push(T val) {
    std::unique_lock<std::mutex> lock(lock_);
    cv1_.wait(lock, [this] { return q_.size() < capicity_; });
    q_.push(std::move(val));
    cv2_.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(lock_);
    cv2_.wait(lock, [this] { return q_.size() > 0; });
    auto val = q_.front();
    q_.pop();
    cv1_.notify_one();
    return val;
  }

 private:
  std::queue<T> q_;
  std::condition_variable cv1_;
  std::condition_variable cv2_;
  std::mutex lock_;
  int capicity_;
};
ConcurrentQueue<int> q(2);

void producer(int time) {
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(time));
    std::cout << "push success! thread id:" << std::this_thread::get_id() << std::endl;
    q.push(1);
  }
}
void consumer(int time) {
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(time));
    int val = q.pop();
    std::cout << "pop " << val << " success! thread id:" << std::this_thread::get_id() << std::endl;
  }
}

int main() {
  std::thread t1(producer, 3);
  std::thread t2(producer, 5);
  std::thread t3(consumer, 1);
  t1.join();
  t2.join();
  t3.join();
}