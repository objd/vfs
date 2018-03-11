#ifndef OBJD_CPP_STAT_HPP
#define OBJD_CPP_STAT_HPP

#include <cstdint>

namespace objd::fs
{
    class stat
    {
    public:
        virtual uint64_t inode() const = 0;
        virtual uint64_t size() const = 0;
        virtual uint64_t atime() const = 0;
        virtual uint64_t mtime() const = 0;
        virtual uint32_t mode() const = 0;
        virtual uint32_t uid() const = 0;
        virtual uint32_t gid() const = 0;
    };
}

#endif
