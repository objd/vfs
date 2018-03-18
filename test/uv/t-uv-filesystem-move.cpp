#include <gtest/gtest.h>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tmpfs-mount.hpp"
#include "t-uv-filesystem-base.hpp"

namespace
{
    class t_move :
        public vfs::test::t_uv_filesystem_base
    {
      public:

        // <editor-fold name="When">

        void when_move_is_invoked()
        {
            _result = _uv_fs.move(_path, _other_path, [this](vfs::any_path &, vfs::any_path &, int err)
            {
                _error_result = err;
            });

            _uv_fs.loop().run();
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_path_has_not_been_moved()
        {
            struct stat64 stat;

            auto err = stat64(_path.str().c_str(), &stat);

            ASSERT_EQ(0, err);
            ASSERT_TRUE(S_ISREG(stat.st_mode));

            err = stat64(_other_path.str().c_str(), &stat);

            ASSERT_EQ(-1, err);
            ASSERT_EQ(ENOENT, errno);
        }

        void then_path_has_been_moved()
        {
            struct stat64 stat;

            auto err = stat64(_path.str().c_str(), &stat);

            ASSERT_EQ(-1, err);
            ASSERT_EQ(ENOENT, errno);

            err = stat64(_other_path.str().c_str(), &stat);

            ASSERT_EQ(0, err);
            ASSERT_TRUE(S_ISREG(stat.st_mode));
        }

        // </editor-fold>
    };

    // @formatter:off
    TEST(uv_filesystem_move, it_should_move_file_when_other_path_does_not_exist)
    {
        t_move t;

        t.given_an_existing_path();
        t.given_an_unexisting_other_path();

        t.when_move_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_path_has_been_moved();
    }

    // @formatter:off
    TEST(uv_filesystem_move, it_should_move_file_even_if_other_path_exist)
    {
        t_move t;

        t.given_an_existing_path();
        t.given_an_existing_other_path();

        t.when_move_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_path_has_been_moved();
    }

    // @formatter:off
    TEST(uv_filesystem_move, it_should_return_enoent_when_path_does_not_exist)
    {
        t_move t;

        t.given_an_unexisting_path();
        t.given_an_unexisting_other_path();

        t.when_move_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_enoent();
    }
}