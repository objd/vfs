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
    using is_path = std::is_base_of<
        path, std::remove_reference_t<t_path>>;

    template<typename t_path>
    using enable_if_path = std::enable_if_t<
        is_path<t_path>::value>;

    template<typename t_path>
    using disable_if_path = std::enable_if_t<
        !is_path<t_path>::value>;

    template<typename t_path>
    using is_base_path = std::is_base_of<
        base_path<t_path>, std::remove_reference_t<t_path>>;

    template<typename t_path>
    using enable_if_base_path = std::enable_if_t<
        is_base_path<t_path>::value>;

    template<typename t_path>
    using is_any_path = std::is_base_of<
        any_path, std::remove_reference_t<t_path>>;

    template<typename t_path>
    using disable_if_any_path = std::enable_if_t<
        !is_any_path<t_path>::value>;

    class path
    {
      public:

        virtual ~path() noexcept
        {};

        virtual bool is_root() const noexcept = 0;
        virtual bool is_absolute() const noexcept = 0;
        virtual bool is_relative() const noexcept = 0;
        virtual bool is_valid() const noexcept = 0;

        virtual const std::string &str() const noexcept = 0;

        virtual path &prepend(path &path) noexcept = 0;
        virtual path &prepend(std::string &str) noexcept = 0;
        virtual path &prepend(std::string &&str) noexcept = 0;

        virtual path &append(path &path) noexcept = 0;
        virtual path &append(std::string &str) noexcept = 0;
        virtual path &append(std::string &&str) noexcept = 0;

        virtual path &parent() noexcept = 0;
        virtual path &filename() noexcept = 0;

//        template<typename t_path>
//        inline t_path copy() const noexcept
//        {
//            return static_cast<t_path &>(*this);
//        };
    };

    template<typename t_path>
    class base_path : public path
    {
      public:

        virtual t_path &append(t_path &path) noexcept = 0;
        virtual t_path &prepend(t_path &path) noexcept = 0;

        inline path &append(path &_path) noexcept override
        {
            auto &path = static_cast<t_path &>(_path);
            return append(_path);
        }

        inline path &&prepend(path &_path) noexcept override
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

    class any_path final : public base_path<any_path>
    {
      private:
        path &_path;

      public:

        template<typename t_path/*, typename = enable_if_base_path<t_path>, typename = disable_if_any_path<t_path>*/>
        explicit any_path(t_path &path)
            : _path(path)
        {}

//        template<typename t_path/*, typename = enable_if_base_path<t_path>*/>
//        any_path(t_path *path)
//            : _path(*path)
//        {}

//        template<typename t_path, typename = enable_if_base_path<t_path>, typename = disable_if_any_path<t_path>>
//        any_path(t_path &&path)
//            : _path(path)
//        {}

//        any_path(const any_path &lhs) noexcept
//            : _path(lhs._path)
//        {}
//
//        any_path(any_path &&rhs) noexcept
//            : _path(rhs._path)
//        {}

        template<typename t_path, typename = enable_if_base_path<t_path>>
        inline any_path &operator=(const t_path &path)
        {
            _path = path;
            return *this;
        }

        template<typename t_path/*, typename = enable_if_base_path<t_path>*/>
        inline any_path &operator=(t_path &&path) noexcept
        {
            _path = path;
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
    };
}

#endif
