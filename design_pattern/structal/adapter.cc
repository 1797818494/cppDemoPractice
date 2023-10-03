#include <iostream>
#include <memory>
class Target {
 public:
  virtual void Print() = 0;
  virtual ~Target() = default;
};

class Adaptee {
 public:
  virtual void PrintImpl() { std::cout << "1"; }
  virtual ~Adaptee() = default;
};

class Adapter : public Target, public Adaptee {
 public:
  void Print() override { Adaptee::PrintImpl(); }
};

int main() {
  std::unique_ptr<Target> target = std::make_unique<Adapter>();
  target->Print();  // 1
}