#include <iostream>
#include <vfs/filesystem.hpp>
#include "vfs/uv/uv-filesystem.hpp"

int main()
{
    uv_loop_t *uv_loop = uv_loop_new();
    vfs::uv::uv_filesystem fs {uv_loop};

    vfs::unix_path p {"/data/objd/test"};
    vfs::any_path p2 {p};

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

    fs.mkdir(p2, 0775, [&fs, &p2](vfs::any_path &path, int err)
    {
        std::cout << path << " created" << std::endl;
    });

    auto flags = UV_FS_O_RDWR | UV_FS_O_CREAT;

    fs.open(p2.append("some-obj.txt"), 0664, flags, [&fs, &p2](vfs::any_path &path, int err, uv_file &file)
    {
        vfs::buffer write_buf {8192};

        std::string data {"some data to write"};
        write_buf.put(data.c_str(), data.size() + 1);

        fs.write(file, std::move(write_buf), 0, [&fs, &p2](uv_file &f, int err, vfs::buffer &b)
        {
            std::string s {b.begin(), b.end()};
            std::cout << p2 << " written data: " << s << std::endl;
        });

        vfs::buffer read_buf {8192};

        fs.read(file, std::move(read_buf), 0l, [&fs, &p2](uv_file &f, int err, vfs::buffer &b)
        {
            std::string s {b.begin(), b.end()};
            std::cout << p2 << " read data: " << s << std::endl;
            std::flush(std::cout);
        });

        fs.close(file, [&fs, &p2](uv_file &f, int err)
        {
            std::cout << p2 << " closed" << std::endl;
        });
    });

    uv_run(uv_loop, UV_RUN_DEFAULT);

//    vfs::path path {"/data/objd/test"};
//
//    fs.open(path, 0, UV_FS_O_RDONLY, [&fs](vfs::path &p, int err, uv_file &f)
//    {
//        vfs::buffer buf {8192};
//
//        fs.read(f, buf, 0l, [&fs, &buf](uv_file &f, int err, vfs::buffer &b)
//        {
//            std::string s {b.begin(), b.end()};
//            std::cout << s << std::endl;
//
//            fs.close(f, [&fs](uv_file &f, int err)
//            {
//                std::cout << "closed" << std::endl;
//            });
//        });
//    });
//
//    uv_run(uv_loop, UV_RUN_DEFAULT);

    delete uv_loop;
}