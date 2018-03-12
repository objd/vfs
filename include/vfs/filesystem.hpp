#ifndef VFS_CPP_FILESYSTEM_H
#define VFS_CPP_FILESYSTEM_H

#include <functional>
#include <vfs/path.hpp>

namespace vfs
{
    using std::vector;
    using std::function;

    template<typename t_stat, typename t_file, typename t_buf>
    struct filesystem_cbs
    {
        typedef function<void(vfs::path &, int, bool)> exists_cb;
        typedef function<void(vfs::path &, int, t_stat)> stat_cb;
        typedef function<void(vfs::path &, int)> mkdir_cb;
        typedef function<void(vfs::path &, int)> mkdirs_cb;
        typedef function<void(vfs::path &, int)> create_cb;
        typedef function<void(vfs::path &, vfs::path &, int)> move_cb;
        typedef function<void(vfs::path &, vfs::path &, int)> copy_cb;
        typedef function<void(vfs::path &, vfs::path &, int)> link_cb;
        typedef function<void(vfs::path &, vfs::path &, int)> symlink_cb;
        typedef function<void(vfs::path &, int)> unlink_cb;
        typedef function<void(vfs::path &, int, t_file)> open_cb;
        typedef function<void(t_file &, int, t_stat)> fstat_cb;
        typedef function<void(t_file &, int, t_buf &)> read_cb;
        typedef function<void(t_file &, int, t_buf &)> write_cb;
        typedef function<void(t_file &, int, uint64_t)> truncate_cb;
        typedef function<void(t_file &, int)> close_cb;
    };

//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, int, bool)> exists_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, int, t_stat)> stat_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, int)> mkdir_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, int)> mkdirs_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, int)> create_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, vfs::path &, int)> move_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, vfs::path &, int)> copy_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, vfs::path &, int)> link_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, vfs::path &, int)> symlink_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, int)> unlink_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(vfs::path &, int, t_file)> open_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(t_file &, int, t_stat)> fstat_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(t_file &, int, t_buf &)> read_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(t_file &, int, t_buf &)> write_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(t_file &, int, uint64_t)> truncate_cb;
//
//    template<typename t_stat, typename t_file, typename t_buf>
//    typedef function<void(t_file &, int)> close_cb;
//
    template<typename t_stat, typename t_file, typename t_buf>
    class filesystem
    {
    public:
        typedef function<void(vfs::path &, int, bool)> exists_cb;
        typedef function<void(vfs::path &, int, t_stat)> stat_cb;
        typedef function<void(vfs::path &, int)> mkdir_cb;
        typedef function<void(vfs::path &, int)> mkdirs_cb;
        typedef function<void(vfs::path &, int)> create_cb;
        typedef function<void(vfs::path &, vfs::path &, int)> move_cb;
        typedef function<void(vfs::path &, vfs::path &, int)> copy_cb;
        typedef function<void(vfs::path &, vfs::path &, int)> link_cb;
        typedef function<void(vfs::path &, vfs::path &, int)> symlink_cb;
        typedef function<void(vfs::path &, int)> unlink_cb;
        typedef function<void(vfs::path &, int, t_file)> open_cb;
        typedef function<void(t_file &, int, t_stat)> fstat_cb;
        typedef function<void(t_file &, int, t_buf &)> read_cb;
        typedef function<void(t_file &, int, t_buf &)> write_cb;
        typedef function<void(t_file &, int, uint64_t)> truncate_cb;
        typedef function<void(t_file &, int)> close_cb;

        virtual int exists(vfs::path &path, exists_cb cb) noexcept = 0;
        virtual int stat(vfs::path &path, stat_cb cb) noexcept = 0;
        virtual int mkdir(vfs::path &path, int32_t mode, mkdir_cb cb) noexcept = 0;
        virtual int mkdirs(vfs::path &path, int32_t mode, mkdirs_cb cb) noexcept = 0;
        virtual int create(vfs::path &path, int32_t mode, create_cb cb) noexcept = 0;
        virtual int move(vfs::path &path, vfs::path &move_path, move_cb cb) noexcept = 0;
        virtual int copy(vfs::path &path, vfs::path &copy_path, copy_cb cb) noexcept = 0;
        virtual int link(vfs::path &path, vfs::path &other_path, link_cb cb) noexcept = 0;
        virtual int symlink(vfs::path &path, vfs::path &other_path, symlink_cb cb) noexcept = 0;
        virtual int unlink(vfs::path &path, unlink_cb cb) noexcept = 0;
        virtual int open(vfs::path &path, int32_t mode, int32_t flags, open_cb cb) noexcept = 0;
        virtual int stat(t_file &file, fstat_cb cb) noexcept = 0;
        virtual int read(t_file &file, t_buf &buf, off64_t off, read_cb cb) noexcept = 0;
        virtual int write(t_file &file, t_buf &buf, off64_t off, write_cb cb) noexcept = 0;
        virtual int truncate(t_file &file, uint64_t size, off64_t off, truncate_cb cb) noexcept = 0;
        virtual int close(t_file &file, close_cb cb) noexcept = 0;
    };
}

#endif
