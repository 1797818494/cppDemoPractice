#include <iostream>
#include <memory>
class Target {
 public:
  virtual void Print() = 0;
  virtual ~Target() = default;
};

class Adaptee {
 public:
  virtual void PrintImpl() { std::cout << 1; }
  virtual ~Adaptee() = default;
};

class Adapter : public Target {
 public:
  Adapter() : p_(std::make_unique<Adaptee>()) {}
  void Print() override { p_->PrintImpl(); }

 private:
  std::unique_ptr<Adaptee> p_;
};

int main() {
  std::unique_ptr<Target> target = std::make_unique<Adapter>();
  target->Print();  // 1
}