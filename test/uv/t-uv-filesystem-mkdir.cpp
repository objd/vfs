#include <gtest/gtest.h>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tmpfs-mount.hpp"

namespace
{
    class t_mkdir
    {
      private:

        // <editor-fold name="Context">

        vfs::test::tmpfs_mount _mount;

        uv_loop_t *_uv_loop;
        vfs::unix_path _path;
        vfs::uv::uv_filesystem _uv_fs;

        int _result;
        int _error_result;

        // </editor-fold>

      public:

        ~t_mkdir()
        {
            uv_loop_close(_uv_loop);
            free(_uv_loop);
        }

        // <editor-fold name="Given">

        void given_an_uv_loop()
        {
            _uv_loop = uv_loop_new();
        }

        void given_an_uv_filesystem()
        {
            _uv_fs = vfs::uv::uv_filesystem {_uv_loop};
        }

        void given_an_existing_path()
        {
            _path = vfs::unix_path{_mount.path()}
                .append("existing");

            auto result = system(("touch " + _path.str()).c_str());

            if (result != 0)
            {
                throw std::exception {};
            }
        }

        void given_an_unexisting_path()
        {
            _path = vfs::unix_path{_mount.path()}
                .append("unexisting");
        }

        // </editor-fold>

        // <editor-fold name="When">

        void when_mkdir_is_invoked()
        {
            _result = _uv_fs.mkdir(_path.to_any(), 0775, [this](vfs::any_path &, int err)
            {
                _error_result = err;
            });

            uv_run(_uv_loop, UV_RUN_DEFAULT);
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

        void then_error_result_is_eexist()
        {
            ASSERT_EQ(EEXIST, _error_result);
        }

        void then_path_is_a_directory()
        {
            struct stat64 stat;
            stat64(_path.str().c_str(), &stat);

            ASSERT_TRUE(S_ISDIR(stat.st_mode));
        }

        // </editor-fold>
    };

    // @formatter:off
    TEST(uv_filesystem_mkdir, it_should_create_directory_when_directory_does_not_exist)
    {
        t_mkdir t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_unexisting_path();

        t.when_mkdir_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_path_is_a_directory();
    }

    // @formatter:off
    TEST(uv_filesystem_mkdir, it_should_return_eexist_when_path_already_exist)
    {
        t_mkdir t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_path();

        t.when_mkdir_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_eexist();
    }
}