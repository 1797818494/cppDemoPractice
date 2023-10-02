#include <iostream>
#include <list>
#include <memory>
class Element;
class Visitor {
 public:
  virtual void VisitA(const Element &) = 0;
  virtual void VisitB(const Element &) = 0;
  virtual ~Visitor() = default;
};
class ConcreteVisitor : public Visitor {
 public:
  void VisitA(const Element &) override { std::cout << "A"; }
  void VisitB(const Element &) override { std::cout << " B"; }
};

class Element {
 public:
  virtual void Accept(const std::shared_ptr<Visitor> &v) = 0;
  virtual ~Element() = default;
};

class ElementA : public Element {
 public:
  void Accept(const std::shared_ptr<Visitor> &v) override { v->VisitA(*this); }
};
class ElementB : public Element {
 public:
  void Accept(const std::shared_ptr<Visitor> &v) override { v->VisitB(*this); }
};

class ObjectStructure {
 public:
  void Append(const std::shared_ptr<Element> &element) { elements_.emplace_back(element); }

  void Remove(const std::shared_ptr<Element> &element) { elements_.remove(element); }

  void Accept(const std::shared_ptr<Visitor> &visitor) {
    for (auto &&x : elements_) {
      x->Accept(visitor);
    }
  }

 private:
  std::list<std::shared_ptr<Element>> elements_;
};

int main() {
  std::shared_ptr<Element> a = std::make_shared<ElementA>();
  std::shared_ptr<Element> b = std::make_shared<ElementB>();

  ObjectStructure o;
  o.Append(a);
  o.Append(b);
  std::shared_ptr<Visitor> visitor = std::make_shared<ConcreteVisitor>();
  o.Accept(visitor);
}