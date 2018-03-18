#include <gtest/gtest.h>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tmpfs-mount.hpp"
#include "t-uv-filesystem-base.hpp"

namespace
{
    class t_create :
        public vfs::test::t_uv_filesystem_base
    {

      public:

        // <editor-fold name="When">

        void when_create_is_invoked()
        {
            _result = _uv_fs.create(_path, 0664, [this](vfs::any_path &, int err)
            {
                _error_result = err;
            });

            _uv_fs.loop().run();
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_path_is_a_file()
        {
            struct stat64 stat;
            stat64(_path.str().c_str(), &stat);

            ASSERT_TRUE(S_ISREG(stat.st_mode));
        }

        // </editor-fold>
    };

    // @formatter:off
    TEST(uv_filesystem_create, it_should_create_file_when_file_does_not_exist)
    {
        t_create t;

        t.given_an_unexisting_path();

        t.when_create_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_path_is_a_file();
    }

    // @formatter:off
    TEST(uv_filesystem_create, it_should_return_eexist_when_path_exist)
    {
        t_create t;

        t.given_an_existing_path();

        t.when_create_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_eexist();
    }
}