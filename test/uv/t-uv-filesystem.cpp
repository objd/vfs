#include <random>

#include <uv.h>
#include <catch/catch.hpp>
#include <vfs/uv/uv-filesystem.hpp>

#include "../t-tags.hpp"

namespace
{
    class t_base
    {
      private:

        std::string _mount;

        std::string random_string(std::string::size_type length)
        {
            static auto &chrs = "0123456789"
                "abcdefghijklmnopqrstuvwxyz"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

            thread_local static std::mt19937 rg {std::random_device{}()};
            thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

            std::string s;

            s.reserve(length);

            while (length--)
            {
                s += chrs[pick(rg)];
            }

            return s;
        }

        void mount_tmpfs()
        {
            _mount = "/mnt/tmpfs" + random_string(12);

            int result = std::system(("sudo mkdir " + _mount).c_str());

            if (result != 0)
            {
                throw std::exception {};
            }

            result = std::system(("sudo mount -t tmpfs -o size=128m tmpfs " + _mount).c_str());

            if (result != 0)
            {
                throw std::exception {};
            }
        }

        void unmount_tmpfs()
        {
            int result = std::system(("sudo umount " + _mount).c_str());

            if (result != 0)
            {
                throw std::exception {};
            };

            result = std::system(("sudo rmdir " + _mount).c_str());

            if (result != 0)
            {
                throw std::exception {};
            };
        }

      public:

        t_base()
        {
            mount_tmpfs();
        };

        ~t_base()
        {
            unmount_tmpfs();
        }

        vfs::unix_path mount()
        {
            return vfs::unix_path{_mount};
        }
    };

    // <editor-fold name="Exists">

    class t_exists
    {
      private:

        // <editor-fold name="Context">

        t_base _base {};

        uv_loop_t *_uv_loop;
        vfs::unix_path _path;
        vfs::uv::uv_filesystem _uv_fs {nullptr};

        int _result;
        int _error_result;
        bool _exists_result;

        // </editor-fold>

      public:

        // <editor-fold name="Lifecycle">

        ~t_exists()
        {
            if (_uv_loop)
            {
                uv_loop_close(_uv_loop);

                free(_uv_loop);

                _uv_loop = nullptr;
            }
        }

        // </editor-fold>

        // <editor-fold name="Given">

        void given_an_uv_loop()
        {
            _uv_loop = uv_loop_new();
        }

        void given_an_uv_filesystem()
        {
            _uv_fs = vfs::uv::uv_filesystem {_uv_loop};
        }

        void given_an_existing_file_path()
        {
            _path = _base.mount().append("existing");

            auto result = system(("sudo touch " + _path.str()).c_str());

            if (result != 0)
            {
                throw std::exception {};
            }
        }

        void given_an_unexisting_file_path()
        {
            _path = _base.mount().append("unexisting");
        }

        // </editor-fold>

        // <editor-fold name="When">

        void when_exists_is_invoked()
        {
            _result = _uv_fs.exists(_path, [this](vfs::any_path &path, int err, bool exists)
            {
                _error_result = err;
                _exists_result = exists;
            });

            uv_run(_uv_loop, UV_RUN_DEFAULT);
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_result_is_zero()
        {
            REQUIRE(0 == _result);
        }

        void then_error_result_is_zero()
        {
            REQUIRE(0 == _error_result);
        }

        void then_exist_result_is_true()
        {
            REQUIRE(_exists_result);
        }

        void then_exist_result_is_false()
        {
            REQUIRE_FALSE(_exists_result);
        }

        // </editor-fold>
    };

    TEST_CASE("[exists] It should return true when file exists", INTEGRATION_TEST_TAG)
    {
        t_exists t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_file_path();

        t.when_exists_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();
        t.then_exist_result_is_true();
    }

    TEST_CASE("[exists] It should return false when file does not exist", INTEGRATION_TEST_TAG)
    {
        t_exists t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_unexisting_file_path();

        t.when_exists_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();
        t.then_exist_result_is_false();
    }

    // </editor-fold>

    // <editor-fold name="Stat">

    class t_stat
    {
      private:

        // <editor-fold name="Context">

        t_base _base {};

        uv_loop_t *_uv_loop;
        vfs::unix_path _path;
        vfs::uv::uv_filesystem _uv_fs {nullptr};

        int _result;
        int _error_result;
        uv_stat_t _stat_result;

        // </editor-fold>

      public:

        // <editor-fold name="Lifecycle">

        ~t_stat()
        {
            if (_uv_loop)
            {
                uv_loop_close(_uv_loop);

                free(_uv_loop);

                _uv_loop = nullptr;
            }
        }

        // </editor-fold>

        // <editor-fold name="Given">

        void given_an_uv_loop()
        {
            _uv_loop = uv_loop_new();
        }

        void given_an_uv_filesystem()
        {
            _uv_fs = vfs::uv::uv_filesystem {_uv_loop};
        }

        void given_an_existing_file_path()
        {
            _path = _base.mount().append("existing");

            auto result = system(("sudo touch " + _path.str()).c_str());

            if (result != 0)
            {
                throw std::exception {};
            }
        }

        void given_an_unexisting_file_path()
        {
            _path = _base.mount().append("unexisting");
        }

        // </editor-fold>

        // <editor-fold name="When">

        void when_stat_is_invoked()
        {
            _result = _uv_fs.stat(_path, [this](vfs::any_path &path, int err, uv_stat_t stat)
            {
                _error_result = err;
                _stat_result = stat;
            });

            uv_run(_uv_loop, UV_RUN_DEFAULT);
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_result_is_zero()
        {
            REQUIRE(0 == _result);
        }

        void then_error_result_is_zero()
        {
            REQUIRE(0 == _error_result);
        }

        void then_error_result_is_enoent()
        {
            REQUIRE(ENOENT == _error_result);
        }

        void then_stat_result_is_set()
        {
            static uv_stat_t unexpected = {};

            REQUIRE_FALSE(0 == memcmp(&unexpected, &_stat_result, sizeof(uv_stat_t)));
        }

        void then_stat_result_is_zeroed()
        {
            static uv_stat_t expected = {};

            REQUIRE(0 == memcmp(&expected, &_stat_result, sizeof(uv_stat_t)));
        }

        // </editor-fold>
    };

    TEST_CASE("[stat] It should return stat struct when file exists", INTEGRATION_TEST_TAG)
    {
        t_stat t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_file_path();

        t.when_stat_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_stat_result_is_set();
    }

    TEST_CASE("[stat] It should return enoent when file does not exist", INTEGRATION_TEST_TAG)
    {
        t_stat t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_unexisting_file_path();

        t.when_stat_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_enoent();

        t.then_stat_result_is_zeroed();
    }

    // </editor-fold>

    // <editor-fold name="Mkdir">

    class t_mkdir
    {
      private:

        // <editor-fold name="Context">

        t_base _base {};

        uv_loop_t *_uv_loop;
        vfs::unix_path _path;
        vfs::uv::uv_filesystem _uv_fs {nullptr};

        int _result;
        int _error_result;

        // </editor-fold>

      public:

        // <editor-fold name="Lifecycle">

        ~t_mkdir()
        {
            if (_uv_loop)
            {
                uv_loop_close(_uv_loop);

                free(_uv_loop);

                _uv_loop = nullptr;
            }
        }

        // </editor-fold>

        // <editor-fold name="Given">

        void given_an_uv_loop()
        {
            _uv_loop = uv_loop_new();
        }

        void given_an_uv_filesystem()
        {
            _uv_fs = vfs::uv::uv_filesystem {_uv_loop};
        }

        void given_an_existing_dir_path()
        {
            _path = _base.mount().append("existing");

            auto result = system(("sudo mkdir " + _path.str()).c_str());

            if (result != 0)
            {
                throw std::exception {};
            }
        }

        void given_an_unexisting_dir_path()
        {
            _path = _base.mount().append("unexisting");
        }

        // </editor-fold>

        // <editor-fold name="When">

        void when_mkdir_is_invoked()
        {
            _result = _uv_fs.mkdir(_path, 0775, [this](vfs::any_path &path, int err)
            {
                _error_result = err;
            });

            uv_run(_uv_loop, UV_RUN_DEFAULT);
        }

        // </editor-fold>

        // <editor-fold name="Then">

        void then_result_is_zero()
        {
            REQUIRE(0 == _result);
        }

        void then_error_result_is_zero()
        {
            REQUIRE(0 == _error_result);
        }

        void then_error_result_is_eexist()
        {
            REQUIRE(EEXIST == _error_result);
        }

        void then_path_is_a_directory()
        {
            struct stat64 stat;
            stat64(_path.str().c_str(), &stat);

            REQUIRE(S_ISDIR(stat.st_mode));
        }

        // </editor-fold>
    };

    TEST_CASE("[mkdir] It should create directory when directory does not exist", INTEGRATION_TEST_TAG)
    {
        t_mkdir t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_unexisting_dir_path();

        t.when_mkdir_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_zero();

        t.then_path_is_a_directory();
    }

    TEST_CASE("[mkdir] It should return eexist when path already exist", INTEGRATION_TEST_TAG)
    {
        t_mkdir t;

        t.given_an_uv_loop();
        t.given_an_uv_filesystem();

        t.given_an_existing_dir_path();

        t.when_mkdir_is_invoked();

        t.then_result_is_zero();
        t.then_error_result_is_eexist();
    }

    // </editor-fold>
}