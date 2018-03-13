#define CATCH_CONFIG_MAIN

#include <catch/catch.hpp>
#include <vfs/path.hpp>

namespace
{
    class test // NOLINT
    {
    private:

        // <editor-fold name="Context">

        std::string _value;
        std::string _other_value;

        vfs::path _path;
        vfs::path _other_path;

        bool _bool_result;
        vfs::path _path_result;
        vfs::path *_path_ptr_result;
        std::string *_str_ptr_result;

        // </editor-fold>

    public:

        // <editor-fold name="Given">

//        void given_an_empty_path_value()
//        {
//            _value = std::string {""};
//        }

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
            _path = vfs::path {_value};
        }

        void given_an_other_path()
        {
            _other_path = vfs::path {_other_value};
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

        void then_the_returned_path_is_the_path()
        {
            REQUIRE (&_path == _path_ptr_result);
        }

        void then_the_returned_path_is_not_the_path()
        {
            REQUIRE (&_path != _path_ptr_result);
        }

        void then_the_returned_path_has_appended_the_value_and_the_other_value()
        {
            std::string expected = _value.append(_other_value);

            REQUIRE (expected == _path_result.c_str());
        }

        void then_the_returned_path_has_appended_the_trimmed_value_and_the_other_value()
        {
            std::string value {_value.begin(), _value.end() - 1};
            std::string expected = value.append(_other_value);

            REQUIRE (expected == _path_result.c_str());
        }

        void then_the_returned_path_has_appended_the_value_and_the_other_value_with_a_sep()
        {
            std::string expected = _value.append("/").append(_other_value);

            REQUIRE (expected == _path_result.c_str());
        }

        void then_the_returned_path_has_prepended_the_value_and_the_other_value()
        {
            std::string expected = _other_value.append(_value);

            REQUIRE (expected == _path_result.str());
        }

        void then_the_returned_path_has_prepended_the_value_and_the_other_trimmed_value()
        {
            std::string other_value {_other_value.begin(), _other_value.end() - 1};
            std::string expected = other_value.append(_value);

            REQUIRE (expected == _path_result.str());
        }

        void then_the_returned_path_has_prepended_the_value_and_the_other_value_with_a_sep()
        {
            std::string expected = _other_value.append("/").append(_value);

            REQUIRE (expected == _path_result.str());
        }

        void then_the_returned_path_is_the_parent_of_path()
        {
            auto separator_index = _value.rfind(vfs::paths::separator_c());
            std::string expected = _value.substr(0, separator_index);

            REQUIRE (expected == _path_result.str());
        }

        // </editor-fold>
    };

    // <editor-fold name="Append">

    TEST_CASE("It should append as-is if values are absolute")
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

    TEST_CASE("It should trim value if value ends with separator and other value is absolute")
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

    TEST_CASE("It should add a separator if value does not end with separator and other value is relative")
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

    TEST_CASE("It should prepend as-is if values are absolute")
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

    TEST_CASE("It should trim value if other value ends with separator and value is absolute")
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

    TEST_CASE("It should add a separator if other value does not end with separator and value is relative")
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

    TEST_CASE("It should return the parent path if the path is absolute")
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.when_parent_of_path_is_invoked();

        t.then_the_returned_path_is_not_the_path();
        t.then_the_returned_path_is_the_parent_of_path();
    }

    // </editor-fold>
}