#ifndef VFS_UV_LOOP_HPP
#define VFS_UV_LOOP_HPP

#include <uv.h>
#include <exception>
#include <memory>
#include <cassert>

namespace vfs::uv
{
    class uv_loop
    {
      public:

        virtual ~uv_loop() noexcept
        {};

        virtual uv_loop_t *ptr() const noexcept = 0;

        inline operator uv_loop_t *() &noexcept
        {
            return ptr();
        }

        inline int run(uv_run_mode mode = UV_RUN_DEFAULT) noexcept
        {
            return uv_run(ptr(), mode);
        }
    };

    class shared_uv_loop : public uv_loop
    {

      private:

        std::shared_ptr<uv_loop_t> _uv_loop;

      public:

        shared_uv_loop(std::shared_ptr<uv_loop_t> uv_loop) noexcept
            : _uv_loop(uv_loop)
        {}

        shared_uv_loop(const shared_uv_loop &lhs) noexcept
            : _uv_loop(lhs._uv_loop)
        {}

        shared_uv_loop(shared_uv_loop &&rhs) = delete;

        shared_uv_loop &operator=(const shared_uv_loop &lhs) noexcept
        {
            _uv_loop = lhs._uv_loop;
            return *this;
        }

        shared_uv_loop &operator=(shared_uv_loop &&rhs) noexcept = delete;

        uv_loop_t *ptr() const noexcept override
        {
            return _uv_loop.get();
        }
    };

    class unique_uv_loop : public uv_loop
    {
        using uv_loop_deleter = void (*)(uv_loop_t *);

      private:

        std::unique_ptr<uv_loop_t, uv_loop_deleter> _uv_loop;

        static void delete_uv_loop(uv_loop_t *uv_loop) noexcept
        {
            auto result = uv_loop_close(uv_loop);
            assert(0 == result);

            free(uv_loop);
        }

      public:

        explicit unique_uv_loop() noexcept
            : _uv_loop(uv_loop_new(), delete_uv_loop)
        {}

        unique_uv_loop(const unique_uv_loop &lhs) noexcept = delete;

        unique_uv_loop(unique_uv_loop &&rhs) noexcept
            : _uv_loop(std::move(rhs._uv_loop))
        {}

        unique_uv_loop &operator=(const unique_uv_loop &lhs) noexcept = delete;

        unique_uv_loop &operator=(unique_uv_loop &&rhs) noexcept
        {
            _uv_loop = std::move(rhs._uv_loop);
            return *this;
        };

        uv_loop_t *ptr() const noexcept override
        {
            return _uv_loop.get();
        }
    };
}

#endif
