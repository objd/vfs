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

        vfs::path *_path_result;
        std::string *_str_result;

        // </editor-fold>

    public:

        // <editor-fold name="Given">

        void given_an_empty_path_value()
        {
            _value = std::string {""};
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

        void given_an_other_empty_path_value()
        {
            _other_value = std::string {""};
        }

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

        void when_the_other_path_is_appended_to_path()
        {
            _path_result = &(_path.append(_other_path));
        }

        void when_the_other_path_is_prepended_to_path()
        {
            _path_result = &(_path.prepend(_other_path));
        }

//        void when_the_other_path_value_is_appended_to_path()
//        {
//            _path_result = &(_path.append(_other_value));
//        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_the_path_is_returned()
        {
            REQUIRE (&_path == _path_result);
        }

        void then_the_path_has_appended_the_value_and_the_other_value()
        {
            std::string expected = _value.append(_other_value);

            REQUIRE (expected == _path.c_str());
        }

        void then_the_path_has_appended_the_trimmed_value_and_the_other_value()
        {
            std::string value {_value.begin(), _value.end() - 1};
            std::string expected = value.append(_other_value);

            REQUIRE (expected == _path.c_str());
        }

        void then_the_path_has_appended_the_value_and_the_other_value_with_a_sep()
        {
            std::string expected = _value.append("/").append(_other_value);

            REQUIRE (expected == _path.c_str());
        }

        void then_the_path_has_prepended_the_value_and_the_other_value()
        {
            std::string expected = _other_value.append(_value);

            REQUIRE (expected == _path.c_str());
        }

        void then_the_path_has_prepended_the_value_and_the_other_trimmed_value()
        {
            std::string other_value {_other_value.begin(), _other_value.end() - 1};
            std::string expected = other_value.append(_value);

            REQUIRE (expected == _path.c_str());
        }

        void then_the_path_has_prepended_the_value_and_the_other_value_with_a_sep()
        {
            std::string expected = _other_value.append("/").append(_value);

            REQUIRE (expected == _path.c_str());
        }

        // </editor-fold>
    };

    // <editor-fold name="Append">

    TEST_CASE("It should append as-is if values are absolute", "append")
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.given_an_other_absolute_path_value();
        t.given_an_other_path();

        t.when_the_other_path_is_appended_to_path();

        t.then_the_path_is_returned();
        t.then_the_path_has_appended_the_value_and_the_other_value();
    }

    TEST_CASE("It should trim value if value ends with separator and other value is absolute", "append")
    {
        test t;

        t.given_a_path_value_ending_with_sep();
        t.given_a_path();

        t.given_an_other_absolute_path_value();
        t.given_an_other_path();

        t.when_the_other_path_is_appended_to_path();

        t.then_the_path_is_returned();
        t.then_the_path_has_appended_the_trimmed_value_and_the_other_value();
    }

    TEST_CASE("It should add a separator if value does not end with separator and other value is relative", "append")
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.given_an_other_relative_path_value();
        t.given_an_other_path();

        t.when_the_other_path_is_appended_to_path();

        t.then_the_path_is_returned();
        t.then_the_path_has_appended_the_value_and_the_other_value_with_a_sep();
    }

    // </editor-fold>

    // <editor-fold name="Prepend">

    TEST_CASE("It should prepend as-is if values are absolute", "prepend")
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.given_an_other_absolute_path_value();
        t.given_an_other_path();

        t.when_the_other_path_is_prepended_to_path();

        t.then_the_path_is_returned();
        t.then_the_path_has_prepended_the_value_and_the_other_value();
    }

    TEST_CASE("It should trim value if other value ends with separator and value is absolute", "prepend")
    {
        test t;

        t.given_an_absolute_path_value();
        t.given_a_path();

        t.given_an_other_path_value_ending_with_sep();
        t.given_an_other_path();

        t.when_the_other_path_is_prepended_to_path();

        t.then_the_path_is_returned();
        t.then_the_path_has_prepended_the_value_and_the_other_trimmed_value();
    }

    TEST_CASE("It should add a separator if other value does not end with separator and value is relative", "prepend")
    {
        test t;

        t.given_a_relative_path_value();
        t.given_a_path();

        t.given_an_other_absolute_path_value();
        t.given_an_other_path();

        t.when_the_other_path_is_prepended_to_path();

        t.then_the_path_is_returned();
        t.then_the_path_has_prepended_the_value_and_the_other_value_with_a_sep();
    }

    // </editor-fold>
}