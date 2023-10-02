#include <iostream>
#include <memory>
class Handle {
 public:
  virtual void Process(std::int32_t i) {
    if (p_) {
      p_->Process(i);
    } else {
      std::cout << i << "can't be process\n" << std::endl;
    }
  }
  void setNext(std::shared_ptr<Handle> p) { p_ = std::move(p); }
  virtual ~Handle() = default;

 private:
  std::shared_ptr<Handle> p_;
};

class A : public Handle {
 public:
  void Process(std::int32_t i) override {
    if (i < 10) {
      std::cout << i << " processd" << std::endl;
    } else {
      Handle::Process(i);
    }
  }
};

class B : public Handle {
 public:
  void Process(std::int32_t i) override {
    if (i < 100) {
      std::cout << i << " processd" << std::endl;
    } else {
      Handle::Process(i);
    }
  }
};

int main() {
  auto a = std::make_shared<A>();
  auto b = std::make_shared<B>();
  a->setNext(b);
  a->Process(9);
  a->Process(99);
  a->Process(999);
}