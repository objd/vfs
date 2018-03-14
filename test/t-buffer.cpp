#include <catch/catch.hpp>
#include <vfs/buffer.hpp>

namespace
{
    class test // NOLINT
    {
    private:

        // <editor-fold name="Context">

        uint64_t _capacity;
        uint64_t _size;

        vfs::buffer _buffer;
        vfs::buffer *_buffer_result;

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

        // </editor-fold>

        // <editor-fold name="When">

        // </editor-fold>

        // <editor-fold name="Then">

        // </editor-fold>
    };

    // <editor-fold name="Put">

    TEST_CASE("It should append as-is if values are absoluteaaf asfasf")
    {
        test t;

    }
    // </editor-fold>
}