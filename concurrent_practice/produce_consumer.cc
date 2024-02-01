#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::condition_variable cv_not_empty;
std::condition_variable cv_not_full;
std::mutex mutex;
std::queue<int> q;
int max_size = 5;
void producer(int num) {
  while (1) {
    std::unique_lock<std::mutex> lock(mutex);

    while (q.size() == max_size) {
      cv_not_full.wait(lock);
    }
    q.push(num);
    std::cout << std::this_thread::get_id() << " producer put " << num << std::endl;
    cv_not_empty.notify_one();
  }
}
void consumer() {
  while (1) {
    std::unique_lock<std::mutex> lock(mutex);
    while (q.empty()) {
      cv_not_empty.wait(lock);
    }
    int num = q.front();
    q.pop();
    std::cout << std::this_thread::get_id() << " consumer get " << num << std::endl;
    cv_not_full.notify_one();
  }
}
int main() {
  std::thread t1[5];
  for (int i = 0; i < 5; i++) {
    t1[i] = std::thread(producer, i);
  }
  std::thread t2(consumer);

  for (int i = 0; i < 5; i++) {
    t1[i].join();
  }
  t2.join();
}