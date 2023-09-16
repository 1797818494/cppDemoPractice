#include <stddef.h>
#include <utility>
namespace my
{
    template <typename ValueT>
    class vector
    {
    public:
        using value_type = ValueT;
        using reference = ValueT &;
        using const_reference = const ValueT &;

        using iterator = ValueT *;
        using const_iterator = const ValueT *;

        using size_type = size_t;
        using difference_type = ::ptrdiff_t;

    private:
        ValueT *m_data;
        size_t m_size;
        size_t m_cap;

    public:
        constexpr vector() noexcept : m_data(nullptr), m_size(0), m_cap(0) {}
        ~vector()
        {
            for (size_t k = 0; k < this->m_size; k++)
            {
                this->m_data[k].~ValueT();
            }
            if (this->m_data)
            {
                ::operator delete(this->m_data);
            }
        }
        vector(const vector &rhs)
        {
            this->m_data = static_cast<ValueT>(::operator new(rhs.m_cap * sizeof(ValueT)));
            this->m_size = 0;
            this->m_cap = rhs.m_cap;
            try
            {
                for (size_t k = 0; k < rhs.m_size; k++)
                {
                    ::new (&this->m_data[k]) ValueT(rhs.m_data[k]);
                    this->m_size++;
                }
            }
            catch (...)
            {
                for (size_t k = 0; k < this->m_size; k++)
                {
                    this->m_data[k].~ValueT();
                }
                ::operator delete(this->m_data);
                throw;
            }
        }
        vector(vector &&rhs) noexcept
        {
            this->m_data = rhs.m_data;
            this->m_size = rhs.m_size;
            this->m_cap = rhs.m_cap;
            rhs.m_data = NULL;
            rhs.m_size = 0;
            rhs.m_cap = 0;
        }
        vector &
        operator=(const vector &rhs);
        vector &
        operator=(vector &&rhs) noexcept;

    public:
        // iterator
        iterator begin() noexcept
        {
            return this->m_data;
        }
        const_iterator begin() const noexcept
        {
            return this->m_data;
        }
        iterator
        end() noexcept
        {
            return this->m_data + this->m_size;
        }
        const_iterator
        end() const noexcept
        {
            return this->m_data + this->m_size;
        }
        value_type *
        data() noexcept
        {
            return this->m_data;
        }
        const value_type *
        data() const noexcept
        {
            return this->m_data;
        }
        size_type
        size() const noexcept
        {
            return this->m_size;
        }
        size_type
        cap() const noexcept
        {
            return this->m_cap;
        }
        bool
        empty() const noexcept
        {
            return this->m_size == 0;
        }
        void clear() const noexcept
        {
            for (size_t k = 0; k < this->m_size; ++k)
            {
                this->m_data[k].~ValueT();
            }
            this->m_size = 0;
        }
        void pop_back() noexcept
        {
            static_assert(!this->empty());
            size_t k = this->m_size - 1;
            this->m_data[k].~ValueT();
            this->m_size = k;
        }
        void push_back(const ValueT &value)
        {
            this->emplace_back(value);
        }
        void push_back(const ValueT &&value)
        {
            this->emplace_back(value);
        }
        template <typename... Args>
        reference emplace_back(Args &&...args)
        {
            if (this->m_size < this->m_cap)
            {
                size_t k = this->m_size;
                new (&this->m_data[k]) ValueT(::std::forward<Args>(args)...);
                this->m_size++;
                return this->m_data[k];
            }
            size_t new_size = this->m_size + 1;
            size_t new_cap = new_size | this->m_size / 2;
            ValueT *new_data = static_cast<ValueT *>(operator new(new_size * sizeof(ValueT)));
            size_t k = 0;
            try
            {
                for (k = 0; k < this->m_size; k++)
                {
                    new (&new_data[k]) ValueT(std::move(m_data[k]));
                }
                new (&new_data[k]) ValueT(std::forward<Args>(args)...);
            }
            catch (...)
            {
                for (size_t i = 0; i < k; i++)
                {
                    new_data[k].~ValueT();
                }
                ::operator delete(new_data);
                throw;
            }
            this->m_data = new_data;
            this->m_size = new_size;
            this->m_cap = new_cap;
            k = this->m_size - 1;
            return this->m_data[k];
        }
    };
}