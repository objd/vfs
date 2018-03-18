#include <iostream>
#include <vfs/filesystem.hpp>
#include "vfs/uv/uv-filesystem.hpp"

int main()
{
    uv_loop_t *uv_loop = uv_loop_new();
    vfs::uv::uv_filesystem fs {uv_loop};

    vfs::unix_path p {"/data/objd/test"};
//    vfs::any_path p2 {p};
//
//    vfs::any_path p3 = vfs::unix_path {"/data/objd/test"};



//    vfs::unix_path p3 = static_cast<vfs::unix_path &>(p2);
//    vfs::any_path p4 = static_cast<vfs::any_path>(p3);
//
//    p2.append("some-obj.txt");
//
//    p4.append("loll/some/dir").append("other-obj.txt");
//
//    std::cout << p << std::endl;
//    std::cout << p2 << std::endl;
//    std::cout << p3 << std::endl;
//    std::cout << p4 << std::endl;

    fs.mkdir(p.to_any(), 0775, [&fs](vfs::any_path &path, int err)
    {
        std::cout << path << " created" << std::endl;
    });

    auto flags = UV_FS_O_RDWR | UV_FS_O_CREAT;

    fs.open(p.append("some-obj.txt").to_any(), 0664, flags, [&fs, &p](vfs::any_path &path, int err, uv_file &file)
    {
        vfs::buffer write_buf {8192};

        std::string data {"some data to write"};
        write_buf.put(data.c_str(), data.size() + 1);

        fs.write(file, std::move(write_buf), 0, [&fs, &p](uv_file &f, int err, vfs::buffer &b)
        {
            std::string s {b.begin(), b.end()};
            std::cout << p << " written data: " << s << std::endl;
        });

        vfs::buffer read_buf {8192};

        fs.read(file, std::move(read_buf), 0l, [&fs, &p](uv_file &f, int err, vfs::buffer &b)
        {
            std::string s {b.begin(), b.end()};
            std::cout << p << " read data: " << s << std::endl;
            std::flush(std::cout);
        });

        fs.close(file, [&fs, &p](uv_file &f, int err)
        {
            std::cout << p << " closed" << std::endl;
        });
    });

    uv_run(uv_loop, UV_RUN_DEFAULT);

    uv_loop_close(uv_loop);
    free(uv_loop);
}