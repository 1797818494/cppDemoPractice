#include <iostream>
#include <list>
#include <memory>

class Colleague;

class Mediator {
 public:
  virtual void Notify(const std::shared_ptr<Colleague> &, std::string_view) = 0;
  virtual ~Mediator() = default;
};

class Colleague {
 public:
  explicit Colleague(const std::shared_ptr<Mediator> &p) : mediator_(p) {}
  virtual void Send(std::string_view) = 0;
  virtual void Receive(std::string_view) const = 0;
  virtual ~Colleague() = default;

 protected:
  std::weak_ptr<Mediator> mediator_;
};

class ColleagueA : public Colleague, public std::enable_shared_from_this<ColleagueA> {
 public:
  using Colleague::Colleague;

  void Send(std::string_view s) override { mediator_.lock()->Notify(shared_from_this(), s); }

  void Receive(std::string_view s) const override { std::cout << "A receive: " << s << '\n'; }
};

class ColleagueB : public Colleague, public std::enable_shared_from_this<ColleagueB> {
 public:
  using Colleague::Colleague;

  void Send(std::string_view s) override { mediator_.lock()->Notify(shared_from_this(), s); }

  void Receive(std::string_view s) const override { std::cout << "B receive: " << s << '\n'; }
};

class ConcreteMediator : public Mediator {
 public:
  void Append(const std::shared_ptr<Colleague> &c) { colleagues_.emplace_back(c); }

  void Notify(const std::shared_ptr<Colleague> &c, std::string_view s) override {
    for (auto &&x : colleagues_) {
      if (const auto p = x.lock()) {
        if (p != c) {
          p->Receive(s);  // 转发给其他成员
        }
      }
    }
  }

 private:
  std::list<std::weak_ptr<Colleague>> colleagues_;
};

int main() {
  auto mediator = std::make_shared<ConcreteMediator>();
  auto colleagueA = std::make_shared<ColleagueA>(mediator);
  auto colleagueB = std::make_shared<ColleagueB>(mediator);
  mediator->Append(colleagueA);
  mediator->Append(colleagueB);

  colleagueA->Send("hello");  // B receive: hello
  colleagueB->Send("world");  // A receive: world
}