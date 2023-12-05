#include <stdexcept>            // std::runtime_error
#include <string>     
#include <assert.h>
#include <istream>
class istream_line_reader {
public:
    class iterator {
        public:
            typedef ptrdiff_t difference_type;
            typedef std::string value_type;
            typedef const value_type* pointer;
            typedef const value_type& reference;
            typedef std::input_iterator_tag iterator_category;
            iterator() = default;
            explicit iterator(std::istream& is) : _M_stream(&is) 
            {
                ++*this;
            }
            reference operator*() const noexcept {
                assert(_M_stream != nullptr);
                return _M_line;
            }
            pointer operator->() const noexcept {
                assert(_M_stream != nullptr);
                return &_M_line;
            }
            iterator& operator++() {
                assert(_M_stream != nullptr);
                getline(*_M_stream, _M_line);
                if(!*_M_stream) {
                    _M_stream = nullptr;
                }
                return *this;
            }
            iterator operator++(int) {
                iterator temp(*this);
                ++*this;
                return temp;
            }
            bool operator==(const iterator& rhs) const noexcept {
                return _M_stream == rhs._M_stream;
            }
            bool operator!=(const iterator& rhs) const noexcept {
                return !operator==(rhs);
            }
        private:
            std::istream* _M_stream{};
            std::string _M_line;
            
    };
    explicit istream_line_reader(std::istream& is) noexcept : _M_stream(&is) {

    }
    istream_line_reader(istream_line_reader&&) = default;
    istream_line_reader& operator=(istream_line_reader&&) = default;
    iterator begin() {
        if(_M_stream->fail()) {
            throw std::runtime_error("input stream error");
        }
        return iterator(*_M_stream);
    }
    iterator end() const noexcept {
        return {};
    }

    private: 
        std::istream* _M_stream;
};