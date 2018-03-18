#ifndef VFS_FILE_HPP
#define VFS_FILE_HPP

#include <cstdint>
#include "path.hpp"

namespace vfs
{
    class file
    {
      public:
        virtual ~file() noexcept
        {};

        virtual vfs::path &path() const noexcept = 0;
        virtual std::uint64_t fd() const noexcept = 0;
    };

    template<typename t_path>
    class file_base : public file
    {
      private:

        t_path _path;
        uint64_t _fd;

      public:

        file_base(t_path &path)
            : file_base(path, 0)
        {}

        file_base(t_path &path, uint64_t fd)
            : _path(path), _fd(fd)
        {};

        t_path &path() const noexcept override
        {
            return const_cast<t_path &>(_path);
        }

        std::uint64_t fd() const noexcept override
        {
            return _fd;
        }
    };
}

#endif
