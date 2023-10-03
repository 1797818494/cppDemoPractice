#include <iostream>
#include <memory>

class Shape {
 public:
  virtual void Display() = 0;
  virtual ~Shape() = default;
};

template <typename... T>
class Line : public Shape, public T... {
 public:
  void Display() override {
    std::apply([](auto &&...x) { (x.Display(), ...); }, std::forward_as_tuple(T()...));
  }
};

class ColorDecorator {
 public:
  void Display() { std::cout << "red"; }
};
class WeightDecorator {
 public:
  void Display() { std::cout << "bold "; }
};

int main() {
  Line line1;
  Line<WeightDecorator> line3;
  Line<ColorDecorator> line2;
  Line<ColorDecorator, WeightDecorator> line4;
  line1.Display();  // line
  line2.Display();  // red line
  line3.Display();  // bold line
  line4.Display();  // red bold line
}