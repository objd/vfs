#ifndef VFS_T_UV_FILESYSTEM_BASE_HPP
#define VFS_T_UV_FILESYSTEM_BASE_HPP

#include <vfs/uv/uv-filesystem.hpp>
#include <gtest/gtest.h>
#include "../include/t-tmpfs-mount.hpp"

namespace vfs::test
{
    class t_uv_filesystem_base
    {
      private:

        vfs::unix_path _unix_path;
        vfs::unix_path _unix_other_path;

      protected:

        // <editor-fold name="Context">

        vfs::test::tmpfs_mount _mount;
        vfs::uv::uv_filesystem _uv_fs;

        vfs::any_path _path {_unix_path};
        vfs::any_path _other_path {_unix_other_path};

        int _result;
        int _error_result;

        // </editor-fold>

        // <editor-fold name="Helpers">

        void create_file(vfs::path &path)
        {
            auto cmd = "touch " + path.str();
            auto result = system(cmd.c_str());

            if (result != 0)
            {
                throw std::exception {};
            }
        }

        void create_dir(vfs::path &path)
        {
            auto cmd = "mkdir -p " + path.str();
            auto result = system(cmd.c_str());

            if (result != 0)
            {
                throw std::exception {};
            }
        }

        // </editor-fold>

      public:

        // <editor-fold name="Given">

        void given_an_existing_file()
        {
            _path.clear()
                .append(_mount.path())
                .append("existing");

            create_file(_path);
        }

        void given_an_existing_dir()
        {
            _path.clear()
                .append(_mount.path())
                .append("existing");

            create_dir(_path);
        }

        void given_an_existing_path()
        {
            given_an_existing_file();
        }

        void given_an_existing_other_file()
        {
            _other_path.clear()
                .append(_mount.path())
                .append("other_existing");

            create_file(_other_path);
        }

        void given_an_existing_other_dir()
        {
            _other_path.clear()
                .append(_mount.path())
                .append("other_existing");

            create_dir(_other_path);
        }

        void given_an_existing_other_path()
        {
            given_an_existing_other_file();
        }

        void given_an_unexisting_path()
        {
            _path.clear()
                .append(_mount.path())
                .append("unexisting");
        }

        void given_an_unexisting_other_path()
        {
            _other_path.clear()
                .append(_mount.path())
                .append("other_unexisting");
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_result_is_zero()
        {
            ASSERT_EQ(0, _result);
        }

        void then_error_result_is_zero()
        {
            ASSERT_EQ(0, _error_result);
        }

        void then_error_result_is_enoent()
        {
            ASSERT_EQ(ENOENT, _error_result);
        }

        void then_error_result_is_eexist()
        {
            ASSERT_EQ(EEXIST, _error_result);
        }

        // </editor-fold>
    };
}

#endif
