#include <iostream>
#include <vfs/filesystem.hpp>
#include "src/uv/uv-filesystem.hpp"

int main()
{
    uv_loop_t *uv_loop = uv_loop_new();
    vfs::uv::uv_filesystem fs {uv_loop};

    vfs::path path {"/data/objd/test"};

    fs.open(path, 0, UV_FS_O_RDONLY, [&fs](vfs::path &p, int err, uv_file &f)
    {
        vfs::buffer buf {8192};

        fs.read(f, buf, 0l, [&fs, &buf](uv_file &f, int err, vfs::buffer &b)
        {
            std::string s {b.begin(), b.end()};
            std::cout << s << std::endl;

            fs.close(f, [&fs](uv_file &f, int err)
            {
                std::cout << "closed" << std::endl;
            });
        });
    });

    uv_run(uv_loop, UV_RUN_DEFAULT);

    delete uv_loop;
}