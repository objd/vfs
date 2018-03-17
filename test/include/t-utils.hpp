#ifndef VFS_T_UTILS_H
#define VFS_T_UTILS_H

#include <random>

namespace vfs
{
    namespace test
    {
        std::string random_string(std::string::size_type length)
        {
            static auto &chrs = "0123456789"
                "abcdefghijklmnopqrstuvwxyz"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

            thread_local static std::mt19937 rg {std::random_device{}()};
            thread_local static std::uniform_int_distribution <std::string::size_type> pick(0, sizeof(chrs) - 2);

            std::string s;

            s.reserve(length);

            while (length--)
            {
                s += chrs[pick(rg)];
            }

            return s;
        }
    }
}

#endif //VFS_T_UTILS_H
