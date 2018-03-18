#include <gtest/gtest.h>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tmpfs-mount.hpp"
#include "t-uv-filesystem-base.hpp"

namespace
{
    class t_link :
        public vfs::test::t_uv_filesystem_base
    {
      public:

        // <editor-fold name="When">

        void when_link_is_invoked()
        {
            _result = _uv_fs.link(_path, _other_path, [this](vfs::any_path &, vfs::any_path &, int err)
            {
                _error_result = err;
            });

            _uv_fs.loop().run();
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_path_has_been_linked()
        {
            struct stat64 stat;

            auto err = lstat64(_path.str().c_str(), &stat);

            ASSERT_EQ(0, err);
            ASSERT_TRUE(S_ISREG(stat.st_mode));

            struct stat64 other_stat;

            err = lstat64(_other_path.str().c_str(), &other_stat);

            ASSERT_EQ(0, err);
            ASSERT_TRUE(S_ISREG(other_stat.st_mode));

            ASSERT_EQ(stat.st_ino, other_stat.st_ino);
        }

        // </editor-fold>
    };

    // @formatter:off
    TEST(uv_filesystem_link, it_should_link_file_when_other_path_does_not_exist)
    {
        t_link t;

        t.given_an_existing_path();
        t.given_an_unexisting_other_path();

        t.when_link_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_path_has_been_linked();
    }

    // @formatter:off
    TEST(uv_filesystem_link, it_should_return_eexist_when_other_path_exist)
    {
        t_link t;

        t.given_an_existing_path();
        t.given_an_existing_other_path();

        t.when_link_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_eexist();
    }

    // @formatter:off
    TEST(uv_filesystem_link, it_should_return_enoent_when_path_does_not_exist)
    {
        t_link t;

        t.given_an_unexisting_path();
        t.given_an_unexisting_other_path();

        t.when_link_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_enoent();
    }
}