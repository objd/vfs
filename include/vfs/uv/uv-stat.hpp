#ifndef VFS_UV_STAT_HPP
#define VFS_UV_STAT_HPP

#include <vfs/stat.hpp>
#include <uv.h>
#include <cmath>
#include <utility>

namespace vfs::uv
{
    class uv_stat : public stat
    {
      private:
        uv_stat_t _uv_stat;

      public:
        uv_stat() noexcept
            : _uv_stat({})
        {}

        uv_stat(const uv_stat_t &uv_stat) noexcept
            : _uv_stat(uv_stat)
        {}

        uv_stat(const uv_stat &lhs) noexcept
            : _uv_stat(lhs._uv_stat)
        {}

        uv_stat(uv_stat &&rhs) noexcept
            : _uv_stat(std::move(rhs._uv_stat))
        {}

        inline uv_stat &operator=(const uv_stat &other) noexcept
        {
            _uv_stat = other._uv_stat;
            return *this;
        }

        inline uv_stat &operator=(uv_stat &&other) noexcept
        {
            _uv_stat = std::move(other._uv_stat);
            return *this;
        }

        inline bool operator==(const uv_stat &other) const noexcept
        {
            return inode() == other.inode();
        }

        inline uint64_t inode() const noexcept override
        {
            return _uv_stat.st_ino;
        }

        inline uint64_t size() const noexcept override
        {
            return _uv_stat.st_size;
        }

        inline uint64_t atime() const noexcept override
        {
            return timestamp_from_timespec(_uv_stat.st_atim);
        }

        inline uint64_t mtime() const noexcept override
        {
            return timestamp_from_timespec(_uv_stat.st_mtim);
        }

        inline uint64_t ctime() const noexcept override
        {
            return timestamp_from_timespec(_uv_stat.st_ctim);
        }

        inline uint32_t uid() const noexcept override
        {
            return static_cast<uint32_t>(_uv_stat.st_uid);
        }

        inline uint32_t gid() const noexcept override
        {
            return static_cast<uint32_t>(_uv_stat.st_gid);
        }

        inline bool is_file() const noexcept override
        {
            return S_ISREG(_uv_stat.st_mode);
        }

        inline bool is_link() const noexcept override
        {
            return S_ISLNK(_uv_stat.st_mode);
        }

        inline bool is_dir() const noexcept override
        {
            return S_ISDIR(_uv_stat.st_mode);
        }

        inline bool is_block() const noexcept override
        {
            return S_ISBLK(_uv_stat.st_mode);
        }

        inline bool is_fifo() const noexcept override
        {
            return S_ISFIFO(_uv_stat.st_mode);
        }

        inline bool is_sock() const noexcept override
        {
            return S_ISSOCK(_uv_stat.st_mode);
        }

      private:

        inline uint64_t timestamp_from_timespec(uv_timespec_t timespec) const
        {
            auto sec = timespec.tv_sec;
            auto ms = std::round(timespec.tv_nsec / 1.0e6);

            if (ms > 999)
            {
                ++sec;
                ms = 0;
            }

            auto timestamp = static_cast<uint64_t>((sec * 1000) + ms);

            return timestamp;
        }
    };
}

#endif
