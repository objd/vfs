#ifndef VFS_T_TMPFS_MOUNT_H
#define VFS_T_TMPFS_MOUNT_H

#include <string>
#include <exception>
#include <cstdlib>

#include "t-utils.hpp"

namespace vfs::test
{
    class tmpfs_mount
    {

      private:

        static const std::size_t default_size = 64 * 1024 * 1024;

        std::string _mount_path;
        std::size_t _size;

        void mount()
        {
            exec_or_throw("sudo mkdir " + _mount_path);
            exec_or_throw("sudo mount -t tmpfs -o size=" + std::to_string(_size) + " tmpfs " + _mount_path);
        }

        void unmount()
        {
            try_exec("sudo umount " + _mount_path);
            try_exec("sudo rmdir " + _mount_path);
        }

        int try_exec(std::string cmd)
        {
            return std::system(cmd.c_str());
        }

        void exec_or_throw(std::string cmd)
        {
            auto result = try_exec(cmd);

            if (result != 0)
            {
                throw std::exception {};
            }
        }

      public:

        explicit tmpfs_mount()
            : tmpfs_mount("/mnt/tmpfs-" + random_string(6))
        {};

        tmpfs_mount(std::string mount_path, std::size_t size = default_size)
            : _mount_path(mount_path), _size(size)
        {
            mount();
        }

        ~tmpfs_mount()
        {
            unmount();
        }

        std::string path()
        {
            return _mount_path;
        }
    };
}

#endif
