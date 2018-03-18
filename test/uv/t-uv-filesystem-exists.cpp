#include <catch/catch.hpp>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tags.hpp"
#include "../include/t-tmpfs-mount.hpp"

#define TEST_TAG "[exist]"

namespace
{
    class t_exists
    {
      private:

        // <editor-fold name="Context">

        vfs::test::tmpfs_mount _mount;

        uv_loop_t *_uv_loop;
        vfs::unix_path _path;
        vfs::uv::uv_filesystem _uv_fs;

        int _result;
        int _error_result;
        bool _exists_result;

        // </editor-fold>

      public:

        ~t_exists()
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

        void when_exists_is_invoked()
        {
            _result = _uv_fs.exists(_path.to_any(), [this](vfs::any_path &path, int err, bool exists)
            {
                _error_result = err;
                _exists_result = exists;
            });

            uv_run(_uv_loop, UV_RUN_DEFAULT);
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_result_is_zero()
        {
            REQUIRE(0 == _result);
        }

        void then_error_result_is_zero()
        {
            REQUIRE(0 == _error_result);
        }

        void then_exist_result_is_true()
        {
            REQUIRE(_exists_result);
        }

        void then_exist_result_is_false()
        {
            REQUIRE_FALSE(_exists_result);
        }

        // </editor-fold>
    };

    // @formatter:off
    TEST_CASE(TEST_TAG " It should return true when file exists", INTEGRATION_TEST_TAG)
    {
        t_exists t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_path();

        t.when_exists_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();
        t.then_exist_result_is_true();
    }

    TEST_CASE(TEST_TAG " It should return false when file does not exist", INTEGRATION_TEST_TAG)
    {
        t_exists t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_unexisting_path();

        t.when_exists_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();
        t.then_exist_result_is_false();
    }
}