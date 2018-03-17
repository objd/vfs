#include <catch/catch.hpp>
#include <vfs/buffer.hpp>

#include "t-tags.hpp"

namespace
{
    class test // NOLINT
    {
      private:

        // <editor-fold name="Context">

        struct data
        {
            int a;
            float b;
            std::string c;
        };

        uint64_t _capacity;
        uint64_t _size;
        vfs::buffer _buffer;

        data _data_struct;
        std::vector<data> _data_struct_vec;
        std::unique_ptr<char[]> _data_ptr;
        size_t _data_ptr_sz;

        bool _bool_result;

        // </editor-fold>

      public:

        // <editor-fold name="Given">

        void given_a_positive_capacity()
        {
            _capacity = 512;
        }

        void given_a_positive_size()
        {
            _capacity = 256;
        }

        void given_a_buffer()
        {
            _buffer = vfs::buffer{_capacity};
        }

        void given_the_buffer_is_truncated()
        {
            _buffer.truncate(_size);
        }

        void given_a_data_struct()
        {
            _data_struct = {
                .a = 1,
                .b = 2.25,
                .c = std::string {"data"}
            };
        }

        void given_a_data_struct_vector()
        {
            _data_struct_vec = {
                data {.a = 1, .b = 2.25, .c = std::string {"data1"}},
                data {.a = 2, .b = 4.50, .c = std::string {"data2"}},
                data {.a = 3, .b = 6.75, .c = std::string {"data3"}},
            };
        }

        void given_a_data_ptr()
        {
            _data_ptr_sz = 128;
            _data_ptr = std::make_unique<char[]>(_data_ptr_sz);
        }

        // </editor-fold>

        // <editor-fold name="When">

        void when_the_data_struct_is_put_into_the_buffer()
        {
            _buffer.put(_data_struct);
        }

        void when_the_data_struct_vec_is_put_into_the_buffer()
        {
            _buffer.put(_data_struct_vec);
        }

        void when_the_data_ptr_is_put_into_the_buffer()
        {
            _buffer.put(_data_ptr.get(), _data_ptr_sz);
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_the_capacity_of_the_buffer_is_the_capacity()
        {
            REQUIRE (_capacity == _buffer.capacity());
        }

        void then_the_size_of_the_buffer_is_zero()
        {

            REQUIRE (0 == _buffer.size());
        }

        void then_the_size_of_the_buffer_is_the_data_struct_size()
        {
            REQUIRE (sizeof(_data_struct) == _buffer.size());
        }

        void then_the_size_of_the_buffer_is_the_data_ptr_size()
        {
            REQUIRE (_data_ptr_sz == _buffer.size());
        }

        void then_the_data_in_the_buffer_is_the_data_struct()
        {
            REQUIRE (0 == memcmp(&_data_struct, _buffer.data(), _buffer.size()));
        }

        // </editor-fold>
    };

    // <editor-fold name="Put">

    TEST_CASE("It should append as-is if values are absoluteaaf asfasf", UNIT_TEST_TAG)
    {
        test t;

        t.given_a_positive_capacity();
        t.given_a_buffer();

        t.then_the_capacity_of_the_buffer_is_the_capacity();
        t.then_the_size_of_the_buffer_is_zero();
    }

    // </editor-fold>
}