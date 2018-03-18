#include <gtest/gtest.h>

#include <vfs/unix/unix-path.hpp>

namespace
{
    class test
    {
      private:

        // <editor-fold name="Context">

        std::string _value;
        std::string _other_value;

        vfs::unix_path _path;
        vfs::unix_path _other_path;

        bool _bool_result;
        std::string *_str_ptr_result;
        vfs::unix_path _path_result;
        vfs::unix_path *_path_ptr_result;

        // </editor-fold>

      public:

        // <editor-fold name="Given">

        void given_an_empty_path_value()
        {
            _value = std::string {""};
        }

        void given_an_path_value_with_a_double_separator()
        {
            _value = std::string {"/data//vfs"};
        }

        void given_the_root_path_value()
        {
            _value = std::string {"/"};
        }

        void given_a_path_value_ending_with_sep()
        {
            _value = std::string {"/data/vfs/"};
        }

        void given_an_absolute_path_value()
        {
            _value = std::string {"/data/vfs"};
        }

        void given_a_relative_path_value()
        {
            _value = std::string {"./data/vfs"};
        }

//        void given_an_other_empty_path_value()
//        {
//            _other_value = std::string {""};
//        }

        void given_an_other_path_value_ending_with_sep()
        {
            _other_value = std::string {"/etc/vfs.conf.d/"};
        }

        void given_an_other_absolute_path_value()
        {
            _other_value = std::string {"/etc/vfs.conf"};
        }

        void given_an_other_relative_path_value()
        {
            _other_value = std::string {"./etc/vfs.conf"};
        }

        void given_a_path()
        {
            _path = vfs::unix_path {_value};
        }

        void given_an_other_path()
        {
            _other_path = vfs::unix_path {_other_value};
        }

        // </editor-fold>

        // <editor-fold name="When">

        void when_append_of_path_is_invoked()
        {
            _path_ptr_result = &(_path.append(_other_path));
            _path_result = *_path_ptr_result;
        }

        void when_prepend_of_path_is_invoked()
        {
            _path_ptr_result = &(_path.prepend(_other_path));
            _path_result = *_path_ptr_result;
        }

        void when_parent_of_path_is_invoked()
        {
            _path_result = _path.parent();
            _path_ptr_result = &_path_result;
        }

        void when_is_absolute_of_path_is_invoked()
        {
            _bool_result = _path.is_absolute();
        }

        void when_is_relative_of_path_is_invoked()
        {
            _bool_result = _path.is_relative();
        }

        void when_is_valid_of_path_is_invoked()
        {
            _bool_result = _path.is_valid();
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_true_is_returned()
        {
            ASSERT_TRUE(_bool_result);
        }

        void then_false_is_returned()
        {
            ASSERT_FALSE(_bool_result);
        }

        void then_the_returned_path_is_the_path()
        {
            ASSERT_EQ(&_path, _path_ptr_result);
        }

        void then_the_returned_path_has_appended_the_value_and_the_other_value()
        {
            std::string expected = _value.append(_other_value);

            ASSERT_EQ(expected, _path_result.str());
        }

        void then_the_returned_path_has_appended_the_trimmed_value_and_the_other_value()
        {
            std::string value {_value.begin(), _value.end() - 1};
            std::string expected = value.append(_other_value);

            ASSERT_EQ(expected, _path_result.str());
        }

        void then_the_returned_path_has_appended_the_value_and_the_other_value_with_a_sep()
        {
            std::string expected = _value.append("/").append(_other_value);

            ASSERT_EQ(expected, _path_result.str());
        }

        void then_the_returned_path_has_prepended_the_value_and_the_other_value()
        {
            std::string expected = _other_value.append(_value);

            ASSERT_EQ(expected, _path_result.str());
        }

        void then_the_returned_path_has_prepended_the_value_and_the_other_trimmed_value()
        {
            std::string other_value {_other_value.begin(), _other_value.end() - 1};
            std::string expected = other_value.append(_value);

            ASSERT_EQ(expected, _path_result.str());
        }

        void then_the_returned_path_has_prepended_the_value_and_the_other_value_with_a_sep()
        {
            std::string expected = _other_value.append("/").append(_value);

            ASSERT_EQ(expected, _path_result.str());
        }

        void then_the_returned_path_is_the_parent_of_path()
        {
            auto separator_index = _value.rfind("/");
            std::string expected = _value.substr(0, separator_index);

            ASSERT_EQ(expected, _path_result.str());
        }

        void then_the_returned_path_is_the_root()
        {
            std::string expected {"/"};

            ASSERT_EQ(expected, _path_result.str());
        }

        // </editor-fold>
    };

    // <editor-fold name="Is Absolute">

    TEST(unix_path_is_absolute, it_should_return_the_path_is_absolute_if_value_is_absolute)
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.when_is_absolute_of_path_is_invoked();

        t.then_true_is_returned();
    }

    TEST(unix_path_is_absolute, it_should_return_the_path_is_absolute_if_value_is_root)
    {
        test t;

        t.given_the_root_path_value();
        t.given_a_path();

        t.when_is_absolute_of_path_is_invoked();

        t.then_true_is_returned();
    }

    TEST(unix_path_is_absolute, it_should_return_the_path_is_not_absolute_if_value_is_relative)
    {
        test t;

        t.given_a_relative_path_value();
        t.given_a_path();

        t.when_is_absolute_of_path_is_invoked();

        t.then_false_is_returned();
    }

    // </editor-fold>

    // <editor-fold name="Is Relative">

    TEST(unix_path_is_relative, it_should_return_the_path_is_relative_if_value_is_relative)
    {
        test t;

        t.given_a_relative_path_value();
        t.given_a_path();

        t.when_is_relative_of_path_is_invoked();

        t.then_true_is_returned();
    }

    TEST(unix_path_is_relative, it_should_return_the_path_is_not_relative_if_value_is_root)
    {
        test t;

        t.given_the_root_path_value();
        t.given_a_path();

        t.when_is_relative_of_path_is_invoked();

        t.then_false_is_returned();
    }

    TEST(unix_path_is_relative, it_should_return_the_path_is_not_relative_if_value_is_absolute)
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.when_is_relative_of_path_is_invoked();

        t.then_false_is_returned();
    }

    // </editor-fold>

    // <editor-fold name="Is Valid">

    TEST(unix_path_is_valid, it_should_return_the_path_is_not_valid_if_value_is_empty)
    {
        test t;

        t.given_an_empty_path_value();
        t.given_a_path();

        t.when_is_valid_of_path_is_invoked();

        t.then_false_is_returned();
    }

    TEST(unix_path_is_valid, it_should_return_the_path_is_not_valid_if_value_has_double_separator)
    {
        test t;

        t.given_an_path_value_with_a_double_separator();
        t.given_a_path();

        t.when_is_valid_of_path_is_invoked();

        t.then_false_is_returned();
    }

    TEST(unix_path_is_valid, it_should_return_the_path_is_valid_if_value_is_absolute)
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.when_is_valid_of_path_is_invoked();

        t.then_true_is_returned();
    }

    TEST(unix_path_is_valid, it_should_return_the_path_is_valid_if_value_is_relative)
    {
        test t;

        t.given_a_relative_path_value();
        t.given_a_path();

        t.when_is_valid_of_path_is_invoked();

        t.then_true_is_returned();
    }

    TEST(unix_path_is_valid, it_should_return_the_path_is_valid_if_value_is_the_root)
    {
        test t;

        t.given_the_root_path_value();
        t.given_a_path();

        t.when_is_valid_of_path_is_invoked();

        t.then_true_is_returned();
    }

// </editor-fold>

    // <editor-fold name="Append">

    TEST(unix_path_append, it_should_append_as_is_if_values_are_absolute)
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.given_an_other_absolute_path_value();
        t.given_an_other_path();

        t.when_append_of_path_is_invoked();

        t.then_the_returned_path_is_the_path();
        t.then_the_returned_path_has_appended_the_value_and_the_other_value();
    }

    TEST(unix_path_append, it_should_trim_value_if_value_ends_with_separator_and_other_value_is_absolute)
    {
        test t;

        t.given_a_path_value_ending_with_sep();
        t.given_a_path();

        t.given_an_other_absolute_path_value();
        t.given_an_other_path();

        t.when_append_of_path_is_invoked();

        t.then_the_returned_path_is_the_path();
        t.then_the_returned_path_has_appended_the_trimmed_value_and_the_other_value();
    }

    TEST(unix_path_append, it_should_add_a_separator_if_value_does_not_end_with_separator_and_other_value_is_relative)
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.given_an_other_relative_path_value();
        t.given_an_other_path();

        t.when_append_of_path_is_invoked();

        t.then_the_returned_path_is_the_path();
        t.then_the_returned_path_has_appended_the_value_and_the_other_value_with_a_sep();
    }

    // </editor-fold>

    // <editor-fold name="Prepend">

    TEST(unix_path_prepend, it_should_prepend_as_is_if_values_are_absolute)
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.given_an_other_absolute_path_value();
        t.given_an_other_path();

        t.when_prepend_of_path_is_invoked();

        t.then_the_returned_path_is_the_path();
        t.then_the_returned_path_has_prepended_the_value_and_the_other_value();
    }

    TEST(unix_path_prepend, it_should_trim_value_if_other_value_ends_with_separator_and_value_is_absolute)
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.given_an_other_path_value_ending_with_sep();
        t.given_an_other_path();

        t.when_prepend_of_path_is_invoked();

        t.then_the_returned_path_is_the_path();
        t.then_the_returned_path_has_prepended_the_value_and_the_other_trimmed_value();
    }

    TEST(unix_path_prepend, it_should_add_a_separator_if_other_value_does_not_end_with_separator_and_value_is_relative)
    {
        test t;

        t.given_a_relative_path_value();
        t.given_a_path();

        t.given_an_other_absolute_path_value();
        t.given_an_other_path();

        t.when_prepend_of_path_is_invoked();

        t.then_the_returned_path_is_the_path();
        t.then_the_returned_path_has_prepended_the_value_and_the_other_value_with_a_sep();
    }

    // </editor-fold>

    // <editor-fold name="Parent">

    TEST(unix_path_parent, it_should_return_the_parent_path_if_the_path_is_absolute)
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.when_parent_of_path_is_invoked();

        t.then_the_returned_path_is_the_parent_of_path();
    }

    TEST(unix_path_parent, it_should_return_the_root_path_if_the_path_is_the_root)
    {
        test t;

        t.given_the_root_path_value();
        t.given_a_path();

        t.when_parent_of_path_is_invoked();

        t.then_the_returned_path_is_the_root();
    }

    // </editor-fold>
}