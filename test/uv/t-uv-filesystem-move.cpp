#include <catch/catch.hpp>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tags.hpp"
#include "../include/t-tmpfs-mount.hpp"

#define TEST_TAG "[move]"

namespace
{
    class t_move
    {
      private:

        // <editor-fold name="Context">

        vfs::test::tmpfs_mount _mount;

        uv_loop_t *_uv_loop;
        vfs::unix_path _path;
        vfs::unix_path _other_path;
        vfs::uv::uv_filesystem _uv_fs;

        int _result;
        int _error_result;

        // </editor-fold>

      public:

        ~t_move()
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

        void given_an_existing_other_path()
        {
            _other_path = vfs::unix_path{_mount.path()}
                .append("other_existing");

            auto result = system(("touch " + _other_path.str()).c_str());

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

        void given_an_unexisting_other_path()
        {
            _other_path = vfs::unix_path{_mount.path()}
                .append("other_unexisting");
        }

        // </editor-fold>

        // <editor-fold name="When">

        void when_move_is_invoked()
        {
            _result = _uv_fs.move(
                _path.to_any(), _other_path.to_any(), [this](vfs::any_path &, vfs::any_path &, int err)
                {
                    _error_result = err;
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

        void then_error_result_is_enoent()
        {
            REQUIRE(ENOENT == _error_result);
        }

        void then_error_result_is_eexist()
        {
            REQUIRE(EEXIST == _error_result);
        }

        void then_path_has_not_been_moved()
        {
            struct stat64 stat;

            auto err = stat64(_path.str().c_str(), &stat);

            REQUIRE(0 == err);
            REQUIRE(S_ISREG(stat.st_mode));

            err = stat64(_other_path.str().c_str(), &stat);

            REQUIRE(-1 == err);
            REQUIRE(ENOENT == errno);
        }

        void then_path_has_been_moved()
        {
            struct stat64 stat;

            auto err = stat64(_path.str().c_str(), &stat);

            REQUIRE(-1 == err);
            REQUIRE(ENOENT == errno);

            err = stat64(_other_path.str().c_str(), &stat);

            REQUIRE(0 == err);
            REQUIRE(S_ISREG(stat.st_mode));
        }

        // </editor-fold>
    };

    // @formatter:off
    TEST_CASE("[move] It should move file when other path does not exist", INTEGRATION_TEST_TAG)
    {
        t_move t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_path();
        t.given_an_unexisting_other_path();

        t.when_move_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_path_has_been_moved();
    }

    // @formatter:off
    TEST_CASE("[move] It should move file even if other path exist", INTEGRATION_TEST_TAG)
    {
        t_move t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_path();
        t.given_an_existing_other_path();

        t.when_move_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_path_has_been_moved();
    }

    // @formatter:off
    TEST_CASE("[move] It should return enoent when path does not exist", INTEGRATION_TEST_TAG)
    {
        t_move t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_unexisting_path();
        t.given_an_unexisting_other_path();

        t.when_move_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_enoent();
    }
}