#include <iostream>
//derived
//https://zhuanlan.zhihu.com/p/142175297
template<typename T>
class Functor;
template<typename Ret, typename... Arg>
class Functor<Ret(Arg...)> {
    public:
        struct CallBase {
            virtual Ret operator()(Arg&&... arg) = 0;
            virtual struct CallBase* copy() const = 0;
            virtual ~CallBase() {} 
        };
        struct CallBase* base;
        template<typename F>
        struct CallBaseDrived : public CallBase {
            F f;
            CallBaseDrived(F fucntor) : f(fucntor) {}
            virtual Ret operator()(Arg&&... arg) override {
                return f(std::forward<Arg>(arg)...);
            }
            virtual struct CallBase* copy() const override {
                return new CallBaseDrived<F>(f);
            }
        };
    public:
        template<typename F>
        Functor(F f) : base(new CallBaseDrived<F>(f)) {}
        ~Functor() {
            std::cout << "delete" << std::endl;
            delete base;
        }
        Ret operator()(Arg&&... arg) {
            return (*base)(std::forward<Arg>(arg)...);
        }
        Functor(const Functor& f) {
            base = f.base->copy();
        }
        Functor& operator=(const Functor& f) {
            std::cout << "assign construct" << std::endl;
            if(this == &f) {
                return *this;
            }
            delete base;
            base = f.base->copy();
            return *this;
        }

};

int getOne(int a, int b) {
    return a + b;
}

struct getTwo {
    getTwo() {}
    int operator()(int a) {
        return 2;
    }
};

int main() {
    Functor<int(int, int)> f1(getOne);
    Functor<int(int)> f2(getTwo{});

    std::cout<<f1(1, 3)<<""<<f2(2)<<std::endl;   
}