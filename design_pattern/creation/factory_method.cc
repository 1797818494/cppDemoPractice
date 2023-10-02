#include <iostream>

class Sentence {
 public:
  virtual std::string Word1() = 0;
  virtual std::string Word2() = 0;
  void GetSentence() { printf("%s,%s!\n", Word1().c_str(), Word2().c_str()); }
  virtual ~Sentence() = default;
};

class SimpleSentence : public Sentence {
  std::string Word1() override { return "hello"; }
  std::string Word2() override { return "world"; }
};

int main() {
  SimpleSentence sentence;
  sentence.GetSentence();
  return 0;
}