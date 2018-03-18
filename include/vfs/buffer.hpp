#ifndef VFS_BUFFER_H
#define VFS_BUFFER_H

#include <cstdint>
#include <cstring>
#include <memory>

namespace vfs
{
    template<typename t_mem>
    using enable_if_not_pointer = std::enable_if_t<!std::is_pointer<t_mem>::value>;

    class buffer
    {
      private:
        uint64_t _size;
        uint64_t _capacity;
        std::unique_ptr<uint8_t[]> _ptr;

      public:
        explicit buffer()
            : _size(0), _capacity(0), _ptr(std::unique_ptr<uint8_t[]>{nullptr})
        {}

        explicit buffer(uint64_t capacity)
            : _size(0), _capacity(capacity), _ptr(std::make_unique<uint8_t[]>(capacity))
        {}

        explicit buffer(std::unique_ptr<uint8_t[]> &&ptr, uint64_t capacity)
            : _size(0), _capacity(capacity), _ptr(std::move(ptr))
        {}

        explicit buffer(std::unique_ptr<uint8_t[]> &&ptr, uint64_t size, uint64_t capacity)
            : _size(size), _capacity(capacity), _ptr(std::move(ptr))
        {}

        buffer(const buffer &lhs) = delete;

        buffer(buffer &&rhs) noexcept
            : _size(rhs._size), _capacity(rhs._capacity), _ptr(std::move(rhs._ptr))
        {}

        buffer &operator=(const buffer &lhs) = delete;

        buffer &operator=(buffer &&rhs) noexcept
        {
            this->_ptr = std::move(rhs._ptr);
            this->_size = rhs._size;
            this->_capacity = rhs._capacity;

            return *this;
        };

        uint64_t size()
        {
            return _size;
        }

        uint64_t capacity()
        {
            return _capacity;
        }

        template<typename t_data>
        t_data *data()
        {
            auto *ptr = static_cast<void *>(_ptr.get());
            return static_cast<t_data *>(ptr);
        }

        uint8_t *data()
        {
            return _ptr.get();
        }

        const uint8_t *begin()
        {
            return _ptr.get();
        }

        const uint8_t *end()
        {
            return _ptr.get() + _size;
        }

        template<typename t_mem, enable_if_not_pointer<t_mem> * = nullptr>
        bool put(const t_mem &mem)
        {
            return put(&mem, sizeof(mem));
        }

        bool put(const void *mem, uint64_t size)
        {
            if (_size + size >= _capacity)
            {
                return false;
            }

            std::memcpy(_ptr.get() + _size, mem, size);

            _size += size;

            return true;
        }

        void clear()
        {
            _size = 0;
        }

        void truncate(uint64_t size)
        {
            _size = size;
        }
    };
}

#endif
