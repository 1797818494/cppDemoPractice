#include <iostream>
#include <memory>
class Shape {
 public:
  virtual void Display() = 0;
  virtual ~Shape() = default;
};

// 定义实现的 ConcreteComponent
class Line : public Shape {
 public:
  void Display() override { std::cout << "line\n"; }
};

// 持有一个 Component 指针的 Decorator，它提供与 Component 一致的接口
class Decorator : public Shape {
 public:
  Decorator(std::unique_ptr<Shape> p) : shape_(std::move(p)) {}

  void Display() override { shape_->Display(); }

 private:
  std::unique_ptr<Shape> shape_;
};
class ColoerDecorator : public Decorator {
 public:
  using Decorator::Decorator;
  void Display() override {
    std::cout << "red";
    Decorator::Display();
  }
};
int main() {
  std::unique_ptr<Shape> colorDecorator = std::make_unique<ColoerDecorator>(std::make_unique<Line>());
  colorDecorator->Display();  // red line
}
