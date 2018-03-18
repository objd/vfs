#include <gtest/gtest.h>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tmpfs-mount.hpp"

namespace
{
    class t_stat
    {
      private:

        // <editor-fold name="Context">

        vfs::test::tmpfs_mount _mount;

        uv_loop_t *_uv_loop;
        vfs::unix_path _path;
        vfs::uv::uv_filesystem _uv_fs;

        int _result;
        int _error_result;
        uv_stat_t _stat_result;

        // </editor-fold>

      public:

        ~t_stat()
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

        void when_stat_is_invoked()
        {
            _result = _uv_fs.stat(_path.to_any(), [this](vfs::any_path &, int err, uv_stat_t stat)
            {
                _error_result = err;
                _stat_result = stat;
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

        void then_error_result_is_enoent()
        {
            ASSERT_EQ(ENOENT, _error_result);
        }

        void then_stat_result_is_set()
        {
            static uv_stat_t unexpected = {};

            ASSERT_NE(0, memcmp(&unexpected, &_stat_result, sizeof(uv_stat_t)));
        }

        void then_stat_result_is_zeroed()
        {
            static uv_stat_t expected = {};

            ASSERT_EQ(0, memcmp(&expected, &_stat_result, sizeof(uv_stat_t)));
        }

        // </editor-fold>
    };

    // @formatter:off
    TEST(uv_filesystem_stat, it_should_return_stat_struct_when_file_exists)
    {
        t_stat t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_path();

        t.when_stat_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_stat_result_is_set();
    }

    TEST(uv_filesystem_stat, it_should_return_enoent_when_file_does_not_exist)
    {
        t_stat t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_unexisting_path();

        t.when_stat_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_enoent();

        t.then_stat_result_is_zeroed();
    }
}