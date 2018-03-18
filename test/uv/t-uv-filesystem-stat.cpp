#include <gtest/gtest.h>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tmpfs-mount.hpp"
#include "t-uv-filesystem-base.hpp"

namespace
{
    class t_stat :
        public vfs::test::t_uv_filesystem_base
    {
      private:

        // <editor-fold name="Context">

        vfs::uv::uv_stat _stat_result;

        // </editor-fold>

      public:

        // <editor-fold name="When">

        void when_stat_is_invoked()
        {
            _result = _uv_fs.stat(_path, [this](vfs::any_path &, int err, vfs::uv::uv_stat stat)
            {
                _error_result = err;
                _stat_result = stat;
            });

            _uv_fs.loop().run();
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_stat_result_is_set()
        {
            ASSERT_TRUE(_stat_result.inode() > 0);
        }

        void then_stat_result_is_zeroed()
        {
            ASSERT_EQ(0, _stat_result.inode());
        }

        // </editor-fold>
    };

    // @formatter:off
    TEST(uv_filesystem_stat, it_should_return_stat_struct_when_file_exists)
    {
        t_stat t;

        t.given_an_existing_path();

        t.when_stat_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_stat_result_is_set();
    }

    TEST(uv_filesystem_stat, it_should_return_enoent_when_file_does_not_exist)
    {
        t_stat t;

        t.given_an_unexisting_path();

        t.when_stat_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_enoent();

        t.then_stat_result_is_zeroed();
    }
}