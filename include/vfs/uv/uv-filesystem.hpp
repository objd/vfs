#ifndef VFS_CPP_UV_FILESYSTEM_H
#define VFS_CPP_UV_FILESYSTEM_H

#include <vector>
#include <uv.h>

#include <vfs/path.hpp>
#include <vfs/filesystem.hpp>

#include <vfs/unix/unix-path.hpp>
#include <vfs/uv/uv-stat.hpp>

namespace vfs::uv
{
    inline int get_uv_error(uv_fs_t *req)
    {
        return static_cast<int>(-(req->result));
    }

    inline uv_file get_uv_file(uv_fs_t *req)
    {
        return static_cast<uv_file>(req->result);
    }

    template<typename t_data>
    inline t_data *get_uv_data(uv_fs_t *req)
    {
        return static_cast<t_data *>(req->data);
    }

    class uv_filesystem :
        public filesystem<vfs::any_path, vfs::uv::uv_stat, uv_file, vfs::buffer>
    {

      private:
        uv_loop_t *_uv_loop;

      public:
        uv_filesystem()
            : _uv_loop(nullptr)
        {};

        explicit uv_filesystem(uv_loop_t *uv_loop)
            : _uv_loop(uv_loop)
        {};

        int exists(vfs::any_path &&path, exists_cb cb) noexcept override;
        int stat(vfs::any_path &&path, stat_cb cb) noexcept override;
        int mkdir(vfs::any_path &&path, int32_t mode, mkdir_cb cb) noexcept override;
        int mkdirs(vfs::any_path &&path, int32_t mode, mkdirs_cb cb) noexcept override;
        int create(vfs::any_path &&path, int32_t mode, create_cb cb) noexcept override;
        int move(vfs::any_path &&path, vfs::any_path &&move_path, move_cb cb) noexcept override;
        int copy(vfs::any_path &&path, vfs::any_path &&copy_path, copy_cb cb) noexcept override;
        int link(vfs::any_path &&path, vfs::any_path &&other_path, link_cb cb) noexcept override;
        int symlink(vfs::any_path &&path, vfs::any_path &&link_path, symlink_cb cb) noexcept override;
        int unlink(vfs::any_path &&path, unlink_cb cb) noexcept override;
        int open(vfs::any_path &&path, int32_t mode, int32_t flags, open_cb cb) noexcept override;
        int stat(uv_file &file, fstat_cb cb) noexcept override;
        int read(uv_file &file, vfs::buffer &&buf, off64_t off, read_cb cb) noexcept override;
        int write(uv_file &file, vfs::buffer &&buf, off64_t off, write_cb cb) noexcept override;
        int truncate(uv_file &file, uint64_t size, truncate_cb cb) noexcept override;
        int close(uv_file &file, close_cb cb) noexcept override;
    };
}

#endif