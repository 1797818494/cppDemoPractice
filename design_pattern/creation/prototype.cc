#include <assert.h>
#include <iostream>
#include <memory>
class Prototype {
 public:
  virtual std::unique_ptr<Prototype> Clone() = 0;
  virtual ~Prototype() = default;
};

class A : public Prototype {
  std::unique_ptr<Prototype> Clone() override { return std::make_unique<A>(*this); }
};

int main() {
  std::unique_ptr<Prototype> p = std::make_unique<A>();
  std::unique_ptr<Prototype> q = p->Clone();
  assert(p != q);
}