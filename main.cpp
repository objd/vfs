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
        auto buf = new std::vector<char>(8192);

        fs.read(f, *buf, 0l, [&fs](uv_file &f, int err, std::vector<char> &buf)
        {
            std::string s {buf.begin(), buf.end()};
            std::cout << s << std::endl;

            fs.close(f, [&fs](uv_file &f, int err)
            {
                std::cout << "closed" << std::endl;
            });
        });
    });

    uv_run(uv_loop, UV_RUN_DEFAULT);

    std::cout << std::endl;
}