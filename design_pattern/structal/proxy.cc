#include <iostream>
#include <memory>

class Graphic {
 public:
  virtual void Display() = 0;
  virtual ~Graphic() = default;
};

class Image : public Graphic {
 public:
  void Display() override { std::cout << 1; }
};
class Proxy : public Graphic {
 public:
  void Display() override { ImageProxy().Display(); }

 protected:
  Image &ImageProxy() {
    if (!p_) {
      p_ = std::make_unique<Image>();
    }
    return *p_;
  }

 private:
  std::unique_ptr<Image> p_;
};

int main() {
  std::unique_ptr<Graphic> proxy = std::make_unique<Proxy>();
  proxy->Display();
}