#ifndef OBJD_CPP_FS_FILESYSTEM_H
#define OBJD_CPP_FS_FILESYSTEM_H

#include <functional>
#include <objd-fs/fs/path.hpp>

namespace objd::fs
{
    using std::vector;
    using std::function;

    using path = objd::fs::path;

    template<typename t_stat, typename t_file, typename t_buf = vector<uint8_t>>
    class filesystem
    {
    public:
        typedef function<void(path &, int, bool)> exists_cb;
        typedef function<void(path &, int, t_stat)> stat_cb;
        typedef function<void(path &, int)> mkdir_cb;
        typedef function<void(path &, int)> mkdirs_cb;
        typedef function<void(path &, int)> create_cb;
        typedef function<void(path &, path &, int)> move_cb;
        typedef function<void(path &, path &, int)> copy_cb;
        typedef function<void(path &, path &, int)> link_cb;
        typedef function<void(path &, path &, int)> symlink_cb;
        typedef function<void(path &, int)> unlink_cb;
        typedef function<void(path &, int, t_file)> open_cb;
        typedef function<void(t_file &, int, t_stat)> fstat_cb;
        typedef function<void(t_file &, int, t_buf &)> read_cb;
        typedef function<void(t_file &, int, t_buf &)> write_cb;
        typedef function<void(t_file &, int, uint64_t)> truncate_cb;
        typedef function<void(t_file &, int)> close_cb;

        virtual int exists(path &path, exists_cb cb) noexcept = 0;
        virtual int stat(path &path, stat_cb cb) noexcept = 0;
        virtual int mkdir(path &path, uint64_t mode, mkdir_cb cb) noexcept = 0;
        virtual int mkdirs(path &path, uint64_t mode, mkdirs_cb cb) noexcept = 0;
        virtual int create(path &path, uint64_t mode, create_cb cb) noexcept = 0;
        virtual int move(path &path, path &move_path, move_cb cb) noexcept = 0;
        virtual int copy(path &path, path &copy_path, copy_cb cb) noexcept = 0;
        virtual int link(path &path, path &other_path, link_cb cb) noexcept = 0;
        virtual int symlink(path &path, path &other_path, symlink_cb cb) noexcept = 0;
        virtual int unlink(path &path, unlink_cb cb) noexcept = 0;
        virtual int open(path &path, uint64_t mode, uint64_t flags, open_cb cb) noexcept = 0;
        virtual int stat(t_file &file, fstat_cb cb) noexcept = 0;
        virtual int read(t_file &file, t_buf &buf, off64_t off, read_cb cb) noexcept = 0;
        virtual int write(t_file &file, t_buf &buf, off64_t off, write_cb cb) noexcept = 0;
        virtual int truncate(t_file &file, uint64_t size, off64_t off, truncate_cb cb) noexcept = 0;
        virtual int close(t_file &file, close_cb cb) noexcept = 0;
    };
}

#endif
