#ifndef VFS_CPP_UV_FILESYSTEM_H
#define VFS_CPP_UV_FILESYSTEM_H

#include <vector>
#include <uv.h>

#include <vfs/path.hpp>
#include <vfs/filesystem.hpp>

#include <vfs/unix/unix-path.hpp>
#include <vfs/uv/uv-stat.hpp>
#include "uv-loop.hpp"
#include "uv-file.hpp"

namespace vfs::uv
{
    inline int get_uv_error(uv_fs_t *req)
    {
        return static_cast<int>(-(req->result));
    }

    inline ::uv_file get_uv_file(uv_fs_t *req)
    {
        return static_cast<::uv_file>(req->result);
    }

    template<typename t_data>
    inline t_data *get_uv_data(uv_fs_t *req)
    {
        return static_cast<t_data *>(req->data);
    }

    using _uv_path_t = vfs::any_path;
    using _uv_stat_t = vfs::uv::uv_stat;
    using _uv_file_t = vfs::uv::uv_file<_uv_path_t>;
    using _uv_buf_t = vfs::buffer;

    class uv_filesystem :
        public filesystem<_uv_path_t, _uv_stat_t, _uv_file_t, _uv_buf_t>
    {

      private:
        std::unique_ptr<vfs::uv::uv_loop> _uv_loop;

      public:

        explicit uv_filesystem()
            : _uv_loop(new vfs::uv::unique_uv_loop)
        {};

        explicit uv_filesystem(vfs::uv::shared_uv_loop &uv_loop) noexcept
            : _uv_loop(&uv_loop)
        {};

        explicit uv_filesystem(vfs::uv::unique_uv_loop &&uv_loop) noexcept
            : _uv_loop(new vfs::uv::unique_uv_loop {std::forward<vfs::uv::unique_uv_loop>(uv_loop)})
        {};

        inline vfs::uv::uv_loop &loop() const noexcept
        {
            return *_uv_loop.get();
        }

        int exists(_uv_path_t path, exists_cb cb) noexcept override;
        int stat(_uv_path_t path, stat_cb cb) noexcept override;
        int mkdir(_uv_path_t path, int32_t mode, mkdir_cb cb) noexcept override;
        int mkdirs(_uv_path_t path, int32_t mode, mkdirs_cb cb) noexcept override;
        int create(_uv_path_t path, int32_t mode, create_cb cb) noexcept override;
        int move(_uv_path_t path, _uv_path_t move_path, move_cb cb) noexcept override;
        int copy(_uv_path_t path, _uv_path_t copy_path, copy_cb cb) noexcept override;
        int link(_uv_path_t path, _uv_path_t other_path, link_cb cb) noexcept override;
        int symlink(_uv_path_t path, _uv_path_t link_path, symlink_cb cb) noexcept override;
        int unlink(_uv_path_t path, unlink_cb cb) noexcept override;
        int open(_uv_path_t path, int32_t mode, int32_t flags, open_cb cb) noexcept override;
        int stat(_uv_file_t file, fstat_cb cb) noexcept override;
        int read(_uv_file_t file, _uv_buf_t buf, off64_t off, read_cb cb) noexcept override;
        int write(_uv_file_t file, _uv_buf_t buf, off64_t off, write_cb cb) noexcept override;
        int truncate(_uv_file_t file, uint64_t size, truncate_cb cb) noexcept override;
        int close(_uv_file_t file, close_cb cb) noexcept override;
    };
}

#endif