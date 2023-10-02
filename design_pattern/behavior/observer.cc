#include <iostream>
#include <list>
#include <memory>
class Subject;
class Observer {
 public:
  explicit Observer(const std::shared_ptr<Subject> &p) : subjects_(p) {}
  virtual void Update() = 0;
  virtual ~Observer() = default;

 protected:
  std::weak_ptr<Subject> subjects_;
};

class Subject {
 public:
  void Append(const std::shared_ptr<Observer> &o) { observers_.emplace_back(o); }
  void Remove(const std::shared_ptr<Observer> &o) { observers_.remove(o); }
  void Notify() {
    for (auto &&x : observers_) {
      x->Update();
    }
  }
  void SetValue(std::int32_t n) { value_ = n; }
  std::int32_t Value() const { return value_; }

 private:
  std::list<std::shared_ptr<Observer>> observers_;
  std::int32_t value_ = 0;
};

class ObserverA : public Observer {
 public:
  using Observer::Observer;

  void Update() override {
    if (const auto p = subjects_.lock()) {
      std::cout << "Subject value in A: " << p->Value() << '\n';
    }
  }
};

class ObserverB : public Observer {
 public:
  using Observer::Observer;

  void Update() override {
    if (const auto p = subjects_.lock()) {
      std::cout << "Subject value in B: " << p->Value() << '\n';
    }
  }
};
int main() {
  auto subject = std::make_shared<Subject>();
  auto a = std::make_shared<ObserverA>(subject);
  auto b = std::make_shared<ObserverB>(subject);
  subject->Append(a);
  subject->Append(b);
  subject->SetValue(3);
  subject->Notify();
}
