#ifndef VFS_CPP_PATH_H
#define VFS_CPP_PATH_H

#include <string>
#include <iostream>

namespace vfs
{
    class path;
    class any_path;

    template<typename t_path>
    class base_path;

    template<typename t_path>
    using is_path = std::is_base_of<path, t_path>;

    template<typename t_path>
    using is_base_path = std::is_base_of<base_path<t_path>, t_path>;

    template<typename t_path>
    using enable_if_path = typename std::enable_if<is_path<t_path>::value>::type;

    template<typename t_path>
    using enable_if_base_path = typename std::enable_if<is_base_path<t_path>::value, t_path>::type;

    class path
    {
      public:
        virtual bool is_root() const noexcept = 0;
        virtual bool is_absolute() const noexcept = 0;
        virtual bool is_relative() const noexcept = 0;
        virtual bool is_valid() const noexcept = 0;

        virtual path &prepend(path &path) noexcept = 0;
        virtual path &prepend(std::string &str) noexcept = 0;
        virtual path &prepend(std::string &&str) noexcept = 0;

        virtual path &append(path &path) noexcept = 0;
        virtual path &append(std::string &str) noexcept = 0;
        virtual path &append(std::string &&str) noexcept = 0;

        virtual path &parent() noexcept = 0;
        virtual path &filename() noexcept = 0;

        virtual const std::string &str() const noexcept = 0;
        virtual const char *const c_str() const noexcept = 0;
    };

    template<typename t_path>
    class base_path : public path
    {
//        static_assert(std::is_base_of<path, t_path>::value, "t_path must be of type path");

      public:

        virtual t_path &append(t_path &path) noexcept = 0;
        virtual t_path &prepend(t_path &path) noexcept = 0;

        inline path &append(path &_path) noexcept override
        {
            auto &path = static_cast<t_path &>(_path);
            return append(_path);
        }

        inline path &prepend(path &_path) noexcept override
        {
            auto &path = static_cast<t_path &>(_path);
            return prepend(_path);
        }

        friend inline std::ostream &operator<<(std::ostream &os, base_path<t_path> &path)
        {
            os << path.str();
            return os;
        }
    };

    class any_path :
        public base_path<any_path>
    {
      private:
        path &_path;

      public:

        template<typename t_path>
        explicit any_path(t_path &path)
            : _path(path)
        {}

        template<typename t_path, typename = enable_if_base_path<t_path>>
        inline any_path &operator=(const t_path &path)
        {
            _path = path;
            return *this;
        }

        template<typename t_path, typename = enable_if_base_path<t_path>>
        inline any_path &operator=(t_path &&path) noexcept
        {
            _path = std::move(path);
            return *this;
        }

        template<typename t_path>
        inline explicit operator t_path &() const
        {
            return static_cast<t_path &>(_path);
        }

//        template<typename t_path>
//        inline explicit operator t_path &&() const
//        {
//            return std::move(_path);
//        }

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

        inline any_path &append(any_path &path) noexcept override
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
