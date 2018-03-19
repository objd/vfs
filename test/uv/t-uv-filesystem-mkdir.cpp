#include <gtest/gtest.h>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tmpfs-mount.hpp"
#include "t-uv-filesystem-base.hpp"

namespace
{
    class t_mkdir :
        public vfs::test::t_uv_filesystem_base
    {
      public:

        // <editor-fold name="When">

        void when_mkdir_is_invoked()
        {
            _result = _uv_fs.mkdir(_path, [this](vfs::any_path &, int err)
            {
                _error_result = err;
            });

            _uv_fs.loop().run();
        }

        // </editor-fold>

        // <editor-fold name="Then">

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

        t.given_an_existing_path();

        t.when_mkdir_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_eexist();
    }
}