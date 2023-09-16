#include <memory>
#include <utility>

class FunctionWrapper {
    public:
        FunctionWrapper() = default;
        FunctionWrapper(const FunctionWrapper&) = delete;
        FunctionWrapper& operator=(const FunctionWrapper&) = delete;
        FunctionWrapper(FunctionWrapper&& rhs) noexcept
        : impl_(std::move(rhs.impl_)) {
        }
        FunctionWrapper& operator=(FunctionWrapper&& rhs) noexcept {
            impl_ = std::move(rhs.impl_);
            return *this;
        }
        template<typename F>
        FunctionWrapper(F&& f) : impl_(new ImplType<F>(std::move(f))) {}
        void operator()() const {
            impl_->call();
        }
        
    private:
        struct ImplBase {
            virtual void call() = 0;
            virtual ~ImplBase() = default;
        };
        template<typename F>
        struct ImplType : ImplBase {
            ImplType(F&& f) noexcept : f_(std::move(f)) {}
            void call() override { f_(); }
            F f_;
        };
    private:
        std::unique_ptr<ImplBase> impl_;
};