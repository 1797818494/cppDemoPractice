#include <iostream>
#include <memory>
class Context {
 public:
  void SetMessage(std::string_view s) { s_ = s; }
  std::string_view Message() const { return s_; }

 private:
  std::string s_;
};
class Expression {
 public:
  virtual bool Interpret(const Context &) = 0;
  virtual ~Expression() = default;
};

class TerminalExpression : public Expression {
 public:
  TerminalExpression(std::string_view s) { s_ = s; }
  bool Interpret(const Context &context) override { return context.Message().find(s_) == std::string_view::npos; }

 private:
  std::string s_;
};

class AndExpression : public Expression {
 public:
  AndExpression(std::shared_ptr<Expression> p, std::shared_ptr<Expression> q) : q_(std::move(q)), p_(std::move(p)) {}
  bool Interpret(const Context &context) override { return p_->Interpret(context) && q_->Interpret(context); }

 private:
  std::shared_ptr<Expression> p_;
  std::shared_ptr<Expression> q_;
};

int main() {
  std::shared_ptr<Expression> a = std::make_shared<TerminalExpression>("sb");
  std::shared_ptr<Expression> b = std::make_shared<TerminalExpression>("fk");
  std::shared_ptr<Expression> expression = std::make_shared<AndExpression>(a, b);

  Context context;
  context.SetMessage("hello world");

  if (expression->Interpret(context)) {
    std::cout << context.Message();
  } else {
    std::cout << "sensitive words";
  }
}