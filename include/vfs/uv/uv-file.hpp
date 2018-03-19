#ifndef VFS_UV_FILE_HPP
#define VFS_UV_FILE_HPP

#include <uv.h>
#include <vfs/file.hpp>

namespace vfs::uv
{
    template<typename t_path>
    class uv_file : public file_base<t_path>
    {
      public:

        uv_file(t_path &path)
            : file_base<t_path>(path)
        {};

        uv_file(t_path &path, ::uv_file fd)
            : file_base<t_path>(path, static_cast<uint64_t >(fd))
        {};

        inline ::uv_file uv_fd() const
        {
            return static_cast<::uv_file>(this->fd());
        }
    };
}

#endif
