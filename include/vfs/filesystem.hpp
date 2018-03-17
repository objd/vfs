#ifndef VFS_CPP_FILESYSTEM_H
#define VFS_CPP_FILESYSTEM_H

#include <functional>
#include <vfs/path.hpp>
#include <vfs/buffer.hpp>

namespace vfs
{
    template<typename t_path, typename t_stat, typename t_file, typename t_buffer>
    class filesystem
    {
    public:
        typedef std::function<void(t_path &, int, bool)> exists_cb;
        typedef std::function<void(t_path &, int, t_stat)> stat_cb;
        typedef std::function<void(t_path &, int)> mkdir_cb;
        typedef std::function<void(t_path &, int)> mkdirs_cb;
        typedef std::function<void(t_path &, int)> create_cb;
        typedef std::function<void(t_path &, t_path &, int)> move_cb;
        typedef std::function<void(t_path &, t_path &, int)> copy_cb;
        typedef std::function<void(t_path &, t_path &, int)> link_cb;
        typedef std::function<void(t_path &, t_path &, int)> symlink_cb;
        typedef std::function<void(t_path &, int)> unlink_cb;
        typedef std::function<void(t_path &, int, t_file &)> open_cb;
        typedef std::function<void(t_file &, int, t_stat)> fstat_cb;
        typedef std::function<void(t_file &, int, vfs::buffer &)> read_cb;
        typedef std::function<void(t_file &, int, vfs::buffer &)> write_cb;
        typedef std::function<void(t_file &, int, uint64_t)> truncate_cb;
        typedef std::function<void(t_file &, int)> close_cb;

        virtual int exists(t_path &path, exists_cb cb) noexcept = 0;
        virtual int stat(t_path &path, stat_cb cb) noexcept = 0;
        virtual int mkdir(t_path &path, int32_t mode, mkdir_cb cb) noexcept = 0;
        virtual int mkdirs(t_path &path, int32_t mode, mkdirs_cb cb) noexcept = 0;
        virtual int create(t_path &path, int32_t mode, create_cb cb) noexcept = 0;
        virtual int move(t_path &path, t_path &move_path, move_cb cb) noexcept = 0;
        virtual int copy(t_path &path, t_path &copy_path, copy_cb cb) noexcept = 0;
        virtual int link(t_path &path, t_path &other_path, link_cb cb) noexcept = 0;
        virtual int symlink(t_path &path, t_path &other_path, symlink_cb cb) noexcept = 0;
        virtual int unlink(t_path &path, unlink_cb cb) noexcept = 0;
        virtual int open(t_path &path, int32_t mode, int32_t flags, open_cb cb) noexcept = 0;
        virtual int stat(t_file &file, fstat_cb cb) noexcept = 0;
        virtual int read(t_file &file, t_buffer &&buf, off64_t off, read_cb cb) noexcept = 0;
        virtual int write(t_file &file, t_buffer &&buf, off64_t off, write_cb cb) noexcept = 0;
        virtual int truncate(t_file &file, uint64_t size, off64_t off, truncate_cb cb) noexcept = 0;
        virtual int close(t_file &file, close_cb cb) noexcept = 0;
    };
}

#endif
