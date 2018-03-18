#ifndef OBJD_CPP_STAT_HPP
#define OBJD_CPP_STAT_HPP

#include <cstdint>

namespace vfs
{
    class stat
    {
      public:

        virtual ~stat()
        {};

        virtual uint64_t inode() const noexcept = 0;
        virtual uint64_t size() const noexcept = 0;
        virtual uint64_t atime() const noexcept = 0;
        virtual uint64_t mtime() const noexcept = 0;
        virtual uint64_t ctime() const noexcept = 0;
        virtual uint32_t uid() const noexcept = 0;
        virtual uint32_t gid() const noexcept = 0;

        virtual bool is_file() const noexcept = 0;
        virtual bool is_link() const noexcept = 0;
        virtual bool is_dir() const noexcept = 0;
        virtual bool is_block() const noexcept = 0;
        virtual bool is_fifo() const noexcept = 0;
        virtual bool is_sock() const noexcept = 0;
    };
}

#endif
