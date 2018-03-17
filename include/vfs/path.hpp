#ifndef VFS_CPP_PATH_H
#define VFS_CPP_PATH_H

#include <string>
#include <iostream>

namespace vfs
{
    template<typename t_allocator>
    class path;

    template<typename t_path, typename t_allocator>
    class base_path;

    template<typename t_allocator>
    class any_path;

    template<typename t_path, typename t_allocator>
    using is_path = std::is_base_of<path<t_allocator>, t_path>;

    template<typename t_path, typename t_allocator>
    using is_base_path = std::is_base_of<base_path<t_path, t_allocator>, t_path>;

    template<typename t_path, typename t_allocator>
    using enable_if_path = std::enable_if_t<is_path<t_path, t_allocator>::value>;

    template<typename t_path, typename t_allocator>
    using enable_if_base_path = std::enable_if_t<is_base_path<t_path, t_allocator>::value, t_path>;

    template<typename t_allocator>
    class path
    {
      public:

        using pointer = typename std::allocator_traits<t_allocator>::pointer;
        using reference = typename std::allocator_traits<t_allocator>::value_type &;
        using const_reference = typename std::allocator_traits<t_allocator>::value_type const &;

        virtual ~path() noexcept
        {};

        virtual bool is_root() const noexcept = 0;
        virtual bool is_absolute() const noexcept = 0;
        virtual bool is_relative() const noexcept = 0;
        virtual bool is_valid() const noexcept = 0;

        virtual const std::string &str() const noexcept = 0;
        virtual const char *const c_str() const noexcept = 0;

        virtual path &prepend(path &path) noexcept = 0;
        virtual path &prepend(std::string &str) noexcept = 0;
        virtual path &prepend(std::string &&str) noexcept = 0;

        virtual reference append(const_reference path) noexcept = 0;
        virtual path &append(std::string &str) noexcept = 0;
        virtual path &append(std::string &&str) noexcept = 0;

        virtual path &parent() noexcept = 0;
        virtual path &filename() noexcept = 0;
    };

    template<typename t_path, typename t_allocator = std::allocator<t_path>>
    class base_path : public path<t_allocator>
    {
      public:
//        using pointer = std::allocator_traits<t_path>::pointer;
//        using reference = std::allocator_traits<t_path>::value_type &;
//        using const_reference = const std::allocator_traits<t_path>::value_type &;

        virtual typename t_path::reference append(typename t_path::const_reference path) noexcept = 0;

//        virtual t_path &prepend(t_path &path) noexcept = 0;
//
//        inline path::reference append(path::const_reference _path) noexcept override
//        {
//            auto &path = static_cast<t_path &>(_path);
//            return append(_path);
//        }
//
//        inline path &prepend(path &_path) noexcept override
//        {
//            auto &path = static_cast<t_path &>(_path);
//            return prepend(_path);
//        }

        friend inline std::ostream &operator<<(std::ostream &os, base_path<t_path> &path)
        {
            os << path.str();
            return os;
        }
    };

    template<typename t_allocator = std::allocator<any_path>>
    class any_path final : public base_path<any_path<t_allocator>, t_allocator>
    {
      private:
        path<t_allocator> &_path;

      public:

        template<typename t_path>
        explicit any_path(t_path &path)
            : _path(path)
        {}

//        template<typename t_path>
//        explicit any_path(t_path &&path)
//            : _path(std::move(path))
//        {}

        template<typename t_path, typename = enable_if_base_path<t_path, t_allocator>>
        inline any_path &operator=(const t_path &path)
        {
            _path = path;
            return *this;
        }

        template<typename t_path, typename = enable_if_base_path<t_path, t_allocator>>
        inline any_path &operator=(t_path &&path) noexcept
        {
            _path = std::move(path);
            return *this;
        }

        template<typename t_path>
        inline operator t_path &() const
        {
            return static_cast<t_path &>(_path);
        }

        inline bool is_root() const noexcept override
        {
            return _path.is_root();
        };

        inline bool is_absolute() const noexcept override
        {
            return _path.is_absolute();
        };

        inline bool is_relative() const noexcept override
        {
            return _path.is_relative();
        };

        inline bool is_valid() const noexcept override
        {
            return _path.is_valid();
        };

        inline any_path &prepend(any_path &path) noexcept override
        {
            _path.prepend(path._path);
            return *this;
        };

        inline any_path &prepend(std::string &val) noexcept override
        {
            _path.prepend(val);
            return *this;
        };

        inline any_path &prepend(std::string &&val) noexcept override
        {
            _path.prepend(val);
            return *this;
        };

        inline reference append(const_reference path) noexcept override
        {
            _path.append(path._path);
            return *this;
        };

        inline any_path &append(std::string &val) noexcept override
        {
            _path.append(val);
            return *this;
        };

        inline any_path &append(std::string &&val) noexcept override
        {
            _path.append(val);
            return *this;
        };

        inline any_path &parent() noexcept override
        {
            _path.parent();
            return *this;
        };

        inline any_path &filename() noexcept override
        {
            _path.filename();
            return *this;
        };

        inline any_path &&copy()
        {
            auto p = _path;
            auto any_p = static_cast<any_path &>(p);
            return std::move(any_p);
        }

        inline const std::string &str() const noexcept override
        {
            return _path.str();
        };

        inline const char *const c_str() const noexcept override
        {
            return _path.c_str();
        };
    };
}

#endif
