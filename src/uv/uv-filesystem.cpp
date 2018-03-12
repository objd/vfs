#include <vector>

#include <vfs/path.hpp>
#include <vfs/filesystem.hpp>

#include "uv-filesystem.hpp"

// <editor-fold desc="using">

using std::vector;

using exists_cb = typename vfs::filesystem<uv_stat_t, uv_file>::exists_cb;
using stat_cb = typename vfs::filesystem<uv_stat_t, uv_file>::stat_cb;
using mkdir_cb = typename vfs::filesystem<uv_stat_t, uv_file>::mkdir_cb;
using mkdirs_cb = typename vfs::filesystem<uv_stat_t, uv_file>::mkdirs_cb;
using create_cb = typename vfs::filesystem<uv_stat_t, uv_file>::create_cb;
using move_cb = typename vfs::filesystem<uv_stat_t, uv_file>::move_cb;
using copy_cb = typename vfs::filesystem<uv_stat_t, uv_file>::copy_cb;
using link_cb = typename vfs::filesystem<uv_stat_t, uv_file>::link_cb;
using symlink_cb = typename vfs::filesystem<uv_stat_t, uv_file>::symlink_cb;
using unlink_cb = typename vfs::filesystem<uv_stat_t, uv_file>::unlink_cb;
using open_cb = typename vfs::filesystem<uv_stat_t, uv_file>::open_cb;
using fstat_cb = typename vfs::filesystem<uv_stat_t, uv_file>::fstat_cb;
using read_cb = typename vfs::filesystem<uv_stat_t, uv_file>::read_cb;
using write_cb = typename vfs::filesystem<uv_stat_t, uv_file>::write_cb;
using truncate_cb = typename vfs::filesystem<uv_stat_t, uv_file>::truncate_cb;
using close_cb = typename vfs::filesystem<uv_stat_t, uv_file>::close_cb;

// </editor-fold>

// <editor-fold desc="exists">

struct exists_cb_data
{
    vfs::path &p;
    exists_cb cb;
};

int vfs::uv::uv_filesystem::exists(vfs::path &path, exists_cb cb) noexcept
{
    auto r = new uv_fs_t {
        .data = new exists_cb_data {
            .p = path,
            .cb = cb
        }
    };

    uv_fs_stat(_uv_loop, r, path.c_str(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<exists_cb_data>(req);

        if (req->result < 0)
        {
            int err = get_uv_error(req);

            if (err == ENOENT)
            {
                data->cb(data->p, 0, false);
            }
            else
            {
                data->cb(data->p, err, false);
            }
        }
        else
        {
            data->cb(data->p, 0, true);
        }

        delete data;
        delete req;
    });

    return 0;
}

// </editor-fold>

// <editor-fold desc="stat">

struct stat_cb_data
{
    vfs::path p;
    stat_cb cb;
};

int vfs::uv::uv_filesystem::stat(vfs::path &path, stat_cb cb) noexcept
{
    auto d = new stat_cb_data {
        .p = path,
        .cb = cb
    };

    auto r = new uv_fs_t {.data = d};

    auto result = uv_fs_stat(_uv_loop, r, path.c_str(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<stat_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, get_uv_error(req), {});
        }
        else
        {
            data->cb(data->p, 0, req->statbuf);
        }

        delete data;
        delete req;
    });

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="mkdir">

struct mkdir_cb_data
{
    vfs::path p;
    mkdir_cb cb;
};

int vfs::uv::uv_filesystem::mkdir(vfs::path &path, int32_t mode, mkdir_cb cb) noexcept
{
    auto r = new uv_fs_t {
        .data = new mkdir_cb_data {
            .p = path,
            .cb = cb
        }
    };

    uv_fs_mkdir(_uv_loop, r, path.c_str(), mode, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<mkdir_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, get_uv_error(req));
        }
        else
        {
            data->cb(data->p, 0);
        }

        delete data;
        delete req;
    });

    return 0;
}

// </editor-fold>

// <editor-fold desc="mkdirs">

int vfs::uv::uv_filesystem::mkdirs(vfs::path &path, int32_t mode, mkdirs_cb cb) noexcept
{
    (void) mode;
    (void) cb;

    return -1;
}

// </editor-fold>

// <editor-fold desc="create">

struct create_cb_data
{
    vfs::uv::uv_filesystem &fs;
    vfs::path p;
    create_cb cb;
};

int vfs::uv::uv_filesystem::create(vfs::path &path, int32_t mode, create_cb cb) noexcept
{
    auto d = new create_cb_data {
        .fs = *this,
        .p = path,
        .cb = cb
    };

    auto r = new uv_fs_t {.data =  d};

    int result = uv_fs_open(_uv_loop, r, path.c_str(), UV_FS_O_CREAT, mode, [](uv_fs_t *req) noexcept
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<create_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, get_uv_error(req));
        }
        else
        {
            data->cb(data->p, 0);
        }

        uv_file file = get_uv_file(req);

        int async_result = uv_fs_close(data->fs._uv_loop, req, file, [](uv_fs_t *req)
        {
            uv_fs_req_cleanup(req);

            delete get_uv_data<create_cb_data>(req);
            delete req;
        });

        if (async_result)
        {
            delete data;
            delete req;
        }
    });

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="move">

struct move_cb_data
{
    vfs::path p;
    vfs::path move_p;
    move_cb cb;
};

int vfs::uv::uv_filesystem::move(vfs::path &path, vfs::path &move_path, move_cb cb) noexcept
{
    auto d = new move_cb_data {
        .p = path,
        .move_p = move_path,
        .cb = cb
    };

    auto r = new uv_fs_t {.data = d};

    int result = uv_fs_rename(_uv_loop, r, path.c_str(), move_path.c_str(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<move_cb_data>(req);

        if (req->result < 0)
        {
            int err = get_uv_error(req);

            data->cb(data->p, data->move_p, err);
        }
        else
        {
            data->cb(data->p, data->move_p, 0);
        }

        delete data;
        delete req;
    });

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="copy">

int vfs::uv::uv_filesystem::copy(vfs::path &path, vfs::path &copy_path, copy_cb cb) noexcept
{
    (void) cb;

    return -1;
}

// </editor-fold>

// <editor-fold desc="link">

struct link_cb_data
{
    vfs::path p;
    vfs::path other_p;
    link_cb cb;
};

int vfs::uv::uv_filesystem::link(vfs::path &path, vfs::path &other_path, link_cb cb) noexcept
{
    auto d = new link_cb_data {
        .p = path,
        .other_p = other_path,
        .cb = cb
    };

    auto r = new uv_fs_t {.data = d};

    auto result = uv_fs_link(_uv_loop, r, path.c_str(), other_path.c_str(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<link_cb_data>(req);

        if (req->result < 0)
        {
            int err = get_uv_error(req);

            data->cb(data->p, data->other_p, err);
        }
        else
        {
            data->cb(data->p, data->other_p, 0);
        }

        delete data;
        delete req;
    });

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="symlink">

struct symlink_cb_data
{
    vfs::path p;
    vfs::path other_p;
    symlink_cb cb;
};

int vfs::uv::uv_filesystem::symlink(vfs::path &path, vfs::path &other_path, symlink_cb cb) noexcept
{
    auto d = new symlink_cb_data {
        .p = path,
        .other_p = other_path,
        .cb = cb
    };

    auto r = new uv_fs_t {.data = d};

    auto result = uv_fs_symlink(_uv_loop, r, path.c_str(), other_path.c_str(), 0, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<link_cb_data>(req);

        if (req->result < 0)
        {
            int err = get_uv_error(req);

            data->cb(data->p, data->other_p, err);
        }
        else
        {
            data->cb(data->p, data->other_p, 0);
        }

        delete data;
        delete req;
    });

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="unlink">

struct unlink_cb_data
{
    vfs::path p;
    unlink_cb cb;
};

int vfs::uv::uv_filesystem::unlink(vfs::path &path, unlink_cb cb) noexcept
{
    auto d = new unlink_cb_data {
        .p = path,
        .cb = cb
    };

    auto r = new uv_fs_t {.data = d};

    auto result = uv_fs_unlink(_uv_loop, r, path.c_str(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<link_cb_data>(req);

        if (req->result < 0)
        {
            int err = get_uv_error(req);

            data->cb(data->p, data->other_p, err);
        }
        else
        {
            data->cb(data->p, data->other_p, 0);
        }

        delete data;
        delete req;
    });

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="open">

struct open_cb_data
{
    vfs::path p;
    open_cb cb;
};

int vfs::uv::uv_filesystem::open(vfs::path &path, int32_t mode, int32_t flags, open_cb cb) noexcept
{
    auto d = new open_cb_data {
        .p = path,
        .cb = cb
    };

    auto r = new uv_fs_t {.data = d};

    auto result = uv_fs_open(_uv_loop, r, path.c_str(), flags, mode, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<open_cb_data>(req);

        if (req->result < 0)
        {
            uv_file file = 0;

            data->cb(data->p, get_uv_error(req), file);
        }
        else
        {
            uv_file file = get_uv_file(req);

            data->cb(data->p, 0, file);
        }

        delete data;
        delete req;
    });

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="fstat">

struct fstat_cb_data
{
    uv_file f;
    fstat_cb cb;
};

int vfs::uv::uv_filesystem::stat(uv_file &file, fstat_cb cb) noexcept
{
    auto d = new fstat_cb_data {
        .f = file,
        .cb = cb
    };

    auto r = new uv_fs_t {.data = d};

    auto result = uv_fs_fstat(_uv_loop, r, file, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<stat_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, get_uv_error(req), {});
        }
        else
        {
            data->cb(data->p, 0, req->statbuf);
        }

        delete data;
        delete req;
    });

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="read">

struct read_cb_data
{
    vfs::buffer buf;
    uv_file f;
    read_cb cb;
};

int vfs::uv::uv_filesystem::read(uv_file &file, vfs::buffer &buf, off64_t off, read_cb cb) noexcept
{
    auto d = new read_cb_data {
        .buf = std::move(buf),
        .f = file,
        .cb = cb
    };

    auto r = new uv_fs_t {.data = d};

    uv_buf_t bufs[] = {
        {.base = d->buf.data<char>(), .len = d->buf.capacity()}
    };

    auto result = uv_fs_read(_uv_loop, r, file, bufs, 1, off, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<read_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->f, get_uv_error(req), data->buf);
        }
        else
        {
            auto n_read = static_cast<uint64_t>(req->result);

            data->buf.truncate(n_read);

            data->cb(data->f, 0, data->buf);
        }

        delete data;
        delete req;
    });

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="write">

struct write_cb_data
{
    uv_file f;
    vector<char> &buf;
    write_cb cb;
};

int vfs::uv::uv_filesystem::write(uv_file &file, vfs::buffer &buf, off64_t off, write_cb cb) noexcept
{
    return 0;
}

// </editor-fold>

// <editor-fold desc="truncate">

int vfs::uv::uv_filesystem::truncate(uv_file &file, uint64_t size, off64_t off, truncate_cb cb) noexcept
{
    return 0;
}

// </editor-fold>

// <editor-fold desc="close">

int vfs::uv::uv_filesystem::close(uv_file &file, close_cb cb) noexcept
{
    return 0;
}

// </editor-fold>
