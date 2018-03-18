#include <gtest/gtest.h>

#include <vfs/uv/uv-filesystem.hpp>
#include <uv.h>

#include "../include/t-tmpfs-mount.hpp"
#include "t-uv-filesystem-base.hpp"

namespace
{
    class t_exists :
        public vfs::test::t_uv_filesystem_base
    {
      private:

        // <editor-fold name="Context">

        bool _exists_result;

        // </editor-fold>

      public:

        // <editor-fold name="When">

        void when_exists_is_invoked()
        {
            _result = _uv_fs.exists(_path, [this](vfs::any_path &, int err, bool exists)
            {
                _error_result = err;
                _exists_result = exists;
            });

            _uv_fs.loop().run();
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_exist_result_is_true()
        {
            ASSERT_TRUE(_exists_result);
        }

        void then_exist_result_is_false()
        {
            ASSERT_FALSE(_exists_result);
        }

        // </editor-fold>
    };

    // @formatter:off
    TEST(uv_filesystem_exists, it_should_return_true_when_file_exists)
    {
        t_exists t;

        t.given_an_existing_path();

        t.when_exists_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();
        t.then_exist_result_is_true();
    }

    TEST(uv_filesystem_exists, It_should_return_false_when_file_does_not_exist)
    {
        t_exists t;

        t.given_an_unexisting_path();

        t.when_exists_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();
        t.then_exist_result_is_false();
    }
}