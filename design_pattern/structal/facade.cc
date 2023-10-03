#include <iostream>
class Scanner {
 public:
  void Scan() { std::cout << "generate token\n"; }
};

class Parser {
 public:
  void Parse() { std::cout << "build syntax tree\n"; }
};

class CodeGenerator {
 public:
  void GenerateCode() { std::cout << "generate machine code\n"; }
};

class Complier {
 public:
  void run() {
    Scanner s;
    s.Scan();
    Parser p;
    p.Parse();
    CodeGenerator c;
    c.GenerateCode();
  }
};

int main() {
  Complier complier;
  complier.run();
}