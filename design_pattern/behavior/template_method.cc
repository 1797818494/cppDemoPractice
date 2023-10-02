#include <iostream>
#include <memory>

class Abstract {
 public:
  void TemplateMethod() {
    FuncA();
    FuncB();
    FuncC();
  }
  virtual ~Abstract() = default;

 private:
  virtual void FuncA() = 0;
  virtual void FuncB() = 0;
  virtual void FuncC() = 0;
};

class A : public Abstract {
  void FuncA() override { std::cout << "1"; }
  void FuncB() override { std::cout << "2"; }
  void FuncC() override { std::cout << "3"; }
};

int main() {
  std::unique_ptr<Abstract> p = std::make_unique<A>();
  p->TemplateMethod();
}