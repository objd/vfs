#include <catch/catch.hpp>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tags.hpp"
#include "../include/t-tmpfs-mount.hpp"

namespace
{
#define TEST_TAG "[link]"

//    const std::string test_tag()
//    {
//        return "[link]";
//    };

    class t_link
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

        ~t_link()
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

        void when_link_is_invoked()
        {
            _result = _uv_fs.link(_path, _other_path, [this](vfs::any_path &path, vfs::any_path &other_path, int err)
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

        void then_path_has_been_linked()
        {
            struct stat64 stat;

            auto err = stat64(_path.str().c_str(), &stat);

            REQUIRE(0 == err);
            REQUIRE(S_ISREG(stat.st_mode));

            struct stat64 other_stat;

            err = stat64(_other_path.str().c_str(), &other_stat);

            REQUIRE(0 == err);
            REQUIRE(S_ISLNK(stat.st_mode));

            REQUIRE(stat.st_ino == other_stat.st_ino);
        }

        // </editor-fold>
    };

    TEST_CASE(TEST_TAG " It should link file when other path does not exist", INTEGRATION_TEST_TAG)
    {
        t_link t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_path();
        t.given_an_unexisting_other_path();

        t.when_link_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_path_has_been_linked();
    }

    TEST_CASE(TEST_TAG " It should return eexist when other path exist", INTEGRATION_TEST_TAG)
    {
        t_link t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_path();
        t.given_an_existing_other_path();

        t.when_link_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_eexist();
    }

    TEST_CASE(TEST_TAG " It should return enoent when path does not exist", INTEGRATION_TEST_TAG)
    {
        t_link t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_unexisting_path();
        t.given_an_unexisting_other_path();

        t.when_link_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_enoent();
    }
}