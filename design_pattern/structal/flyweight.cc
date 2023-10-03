#include <iostream>
#include <memory>
#include <unordered_map>

class FlyWeight {
 public:
  virtual void Print() = 0;
  virtual ~FlyWeight() = default;
};

class ConcreteFlyweight : public FlyWeight {
 public:
  explicit ConcreteFlyweight(char key) : key_(key) {}
  void Print() override { std::cout << key_; }

 private:
  char key_;
};

class FlyWeightFactory {
 public:
  std::unique_ptr<FlyWeight> &FlyweightPtr(char c) {
    if (const auto it = m_.find(c); it != std::end(m_)) {
      std::cout << "existing key: ";
      return it->second;
    }
    std::unique_ptr<FlyWeight> p = std::make_unique<ConcreteFlyweight>(c);
    m_.emplace(c, std::move(p));
    return m_.at(c);
  }

 private:
  std::unordered_map<char, std::unique_ptr<FlyWeight>> m_;
};

int main() {
  FlyWeightFactory factory;
  factory.FlyweightPtr('a')->Print();  // a
  factory.FlyweightPtr('a')->Print();  // existing key: a
  factory.FlyweightPtr('b')->Print();  // b
  factory.FlyweightPtr('b')->Print();  // existing key: a
}