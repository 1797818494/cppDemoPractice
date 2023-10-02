#include <functional>
#include <iostream>
#include <list>
#include <memory>

class Receiver {
 public:
  void Print() { std::cout << 1; }
};

class Command {
 public:
  virtual void Execute() = 0;
  virtual ~Command() = default;
};

template <typename T>
class CommandA : public Command {
 public:
  using Action = std::function<void(T &)>;
  CommandA(const std::shared_ptr<T> &p, Action a) : receiver_(p), action(std::move(a)) {}
  void Execute() override {
    std::cout << "A";
    if (const auto p = receiver_.lock()) {
      action(*p);
    }
  }

 private:
  std::weak_ptr<T> receiver_;
  Action action;
};
template <typename T>
class CommandB : public Command {
 public:
  using Action = std::function<void(T &)>;
  CommandB(const std::shared_ptr<T> &p, Action a) : receiver_(p), action(std::move(a)) {}
  void Execute() override {
    std::cout << "B";
    if (const auto p = receiver_.lock()) {
      action(*p);
    }
  }

 private:
  std::weak_ptr<T> receiver_;
  Action action;
};
class Invoker : public Command {
 public:
  void Append(const std::shared_ptr<Command> &c) { commands.emplace_back(c); }
  void Remove(const std::shared_ptr<Command> &c) {
    commands.remove_if([&](std::weak_ptr<Command> &x) { return x.lock() == c; });
  }
  void Execute() override {
    for (auto &&x : commands) {
      if (const auto p = x.lock()) {
        p->Execute();
      }
    }
  }

 private:
  std::list<std::weak_ptr<Command>> commands;
};

int main() {
  auto receiver = std::make_shared<Receiver>();
  const std::shared_ptr<Command> commandA = std::make_shared<CommandA<Receiver>>(receiver, &Receiver::Print);
  const std::shared_ptr<Command> commandB = std::make_shared<CommandB<Receiver>>(receiver, &Receiver::Print);

  Invoker invoker;
  invoker.Append(commandA);
  invoker.Append(commandB);
  invoker.Execute();  // A1B1
}
