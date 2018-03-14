#ifndef VFS_CPP_PATH_H
#define VFS_CPP_PATH_H

#include <string>
#include <iostream>

namespace vfs
{
//    template<typename t_path, typename>
//    class path;

//    template <typename t_path>
//    using is_path = typename std::enable_if<std::is_base_of<vfs::path<t_path, void>, t_path>::value>::type;

    template<typename t_path>
    class path
    {
    public:

        friend inline std::ostream &operator<<(std::ostream &os, const t_path &path)
        {
            os << path.str();
            return os;
        }

        bool operator==(t_path &other) const noexcept
        {
            return str() == other.str();
        }

        virtual bool is_root() = 0;
        virtual bool is_absolute() noexcept = 0;
        virtual bool is_relative() noexcept = 0;
        virtual bool is_valid() noexcept = 0;

        virtual t_path &prepend(t_path &path) noexcept = 0;
        virtual t_path &prepend(std::string &val) noexcept = 0;
        virtual t_path &prepend(std::string &&val) noexcept = 0;
        virtual t_path &append(t_path &path) noexcept = 0;
        virtual t_path &append(std::string &val) noexcept = 0;
        virtual t_path &append(std::string &&val) noexcept = 0;

        virtual t_path parent() noexcept = 0;
        virtual t_path filename() noexcept = 0;
        virtual t_path clone() noexcept = 0;

        virtual const std::string &str() noexcept = 0;
        virtual const char *const c_str() noexcept = 0;
    };
}

#endif
