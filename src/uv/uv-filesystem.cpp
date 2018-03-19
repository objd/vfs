#include <vector>

#include <vfs/path.hpp>
#include <vfs/filesystem.hpp>

#include <vfs/uv/uv-filesystem.hpp>

// <editor-fold desc="using">

using exists_cb = typename vfs::uv::uv_filesystem::exists_cb;
using stat_cb = typename vfs::uv::uv_filesystem::stat_cb;
using mkdir_cb = typename vfs::uv::uv_filesystem::mkdir_cb;
using mkdirs_cb = typename vfs::uv::uv_filesystem::mkdirs_cb;
using create_cb = typename vfs::uv::uv_filesystem::create_cb;
using move_cb = typename vfs::uv::uv_filesystem::move_cb;
using copy_cb = typename vfs::uv::uv_filesystem::copy_cb;
using link_cb = typename vfs::uv::uv_filesystem::link_cb;
using symlink_cb = typename vfs::uv::uv_filesystem::symlink_cb;
using unlink_cb = typename vfs::uv::uv_filesystem::unlink_cb;
using open_cb = typename vfs::uv::uv_filesystem::open_cb;
using fstat_cb = typename vfs::uv::uv_filesystem::fstat_cb;
using read_cb = typename vfs::uv::uv_filesystem::read_cb;
using write_cb = typename vfs::uv::uv_filesystem::write_cb;
using truncate_cb = typename vfs::uv::uv_filesystem::truncate_cb;
using close_cb = typename vfs::uv::uv_filesystem::close_cb;

// </editor-fold>

static auto new_uv_req(void *data)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

    return new uv_fs_t {
        .data = data
    };

#pragma GCC diagnostic pop
}

// <editor-fold desc="exists">

struct exists_cb_data
{
    vfs::uv::_uv_path_t p;
    exists_cb cb;
};

int vfs::uv::uv_filesystem::exists(vfs::uv::_uv_path_t path, exists_cb cb) noexcept
{
    auto d = new exists_cb_data {
        .p = path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_stat(loop(), r, path.str().c_str(), [](uv_fs_t *req)
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

    if (result != 0)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="stat">

struct stat_cb_data
{
    vfs::uv::_uv_path_t p;
    stat_cb cb;
};

int vfs::uv::uv_filesystem::stat(vfs::uv::_uv_path_t path, stat_cb cb) noexcept
{
    auto d = new stat_cb_data {
        .p = path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_stat(loop(), r, path.str().c_str(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<stat_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, get_uv_error(req), {});
        }
        else
        {
            uv_stat uv_stat {req->statbuf};

            data->cb(data->p, 0, uv_stat);
        }

        delete data;
        delete req;
    });

    if (result != 0)
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
    vfs::uv::_uv_path_t p;
    mkdir_cb cb;
};

int vfs::uv::uv_filesystem::mkdir(vfs::uv::_uv_path_t path, int32_t mode, mkdir_cb cb) noexcept
{
    auto d = new mkdir_cb_data {
        .p = path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_mkdir(loop(), r, path.str().c_str(), mode, [](uv_fs_t *req)
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

    if (result)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="mkdirs">

struct mkdirs_cb_data
{
    vfs::uv::_uv_path_t p;
    mkdir_cb cb;
    int32_t mode;
};

int vfs::uv::uv_filesystem::mkdirs(vfs::uv::_uv_path_t path, int32_t mode, mkdirs_cb cb) noexcept
{
    (void) path;
    (void) mode;
    (void) cb;

    return -1;
}

// </editor-fold>

// <editor-fold desc="create">

struct create_cb_data
{
    vfs::uv::uv_filesystem &fs;
    vfs::uv::_uv_path_t p;
    create_cb cb;
};

int vfs::uv::uv_filesystem::create(vfs::uv::_uv_path_t path, int32_t mode, create_cb cb) noexcept
{
    auto d = new create_cb_data {
        .fs = *this,
        .p = path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto flags = UV_FS_O_CREAT | UV_FS_O_EXCL;

    auto result = uv_fs_open(loop(), r, path.str().c_str(), flags, mode, [](uv_fs_t *req) noexcept
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

        auto file = get_uv_file(req);

        auto other_result = uv_fs_close(data->fs.loop(), req, file, [](uv_fs_t *req)
        {
            uv_fs_req_cleanup(req);

            delete get_uv_data<create_cb_data>(req);
            delete req;
        });

        if (other_result != 0)
        {
            delete data;
            delete req;
        }
    });

    if (result != 0)
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
    vfs::uv::_uv_path_t p;
    vfs::uv::_uv_path_t move_p;
    move_cb cb;
};

int vfs::uv::uv_filesystem::move(vfs::uv::_uv_path_t path, vfs::uv::_uv_path_t move_path, move_cb cb) noexcept
{
    auto d = new move_cb_data {
        .p = path,
        .move_p = move_path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_rename(loop(), r, path.str().c_str(), move_path.str().c_str(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<move_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, data->move_p, get_uv_error(req));
        }
        else
        {
            data->cb(data->p, data->move_p, 0);
        }

        delete data;
        delete req;
    });

    if (result != 0)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="copy">

struct copy_cb_data
{
    vfs::uv::_uv_path_t p;
    vfs::uv::_uv_path_t copy_p;
    copy_cb cb;
};

int vfs::uv::uv_filesystem::copy(vfs::uv::_uv_path_t path, vfs::uv::_uv_path_t copy_path, copy_cb cb) noexcept
{
    auto d = new copy_cb_data {
        .p = path,
        .copy_p = copy_path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_copyfile(loop(), r, path.str().c_str(), copy_path.str().c_str(), 0, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<copy_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, data->copy_p, get_uv_error(req));
        }
        else
        {
            data->cb(data->p, data->copy_p, 0);
        }

        delete data;
        delete req;
    });

    if (result != 0)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="link">

struct link_cb_data
{
    vfs::uv::_uv_path_t p;
    vfs::uv::_uv_path_t link_p;
    link_cb cb;
};

int vfs::uv::uv_filesystem::link(vfs::uv::_uv_path_t path, vfs::uv::_uv_path_t link_path, link_cb cb) noexcept
{
    auto d = new link_cb_data {
        .p = path,
        .link_p = link_path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_link(loop(), r, path.str().c_str(), link_path.str().c_str(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<link_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, data->link_p, get_uv_error(req));
        }
        else
        {
            data->cb(data->p, data->link_p, 0);
        }

        delete data;
        delete req;
    });

    if (result != 0)
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
    vfs::uv::_uv_path_t p;
    vfs::uv::_uv_path_t link_p;
    symlink_cb cb;
};

int vfs::uv::uv_filesystem::symlink(vfs::uv::_uv_path_t path, vfs::uv::_uv_path_t link_path, symlink_cb cb) noexcept
{
    auto d = new symlink_cb_data {
        .p = path,
        .link_p = link_path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_symlink(loop(), r, path.str().c_str(), link_path.str().c_str(), 0, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<symlink_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, data->link_p, get_uv_error(req));
        }
        else
        {
            data->cb(data->p, data->link_p, 0);
        }

        delete data;
        delete req;
    });

    if (result != 0)
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
    vfs::uv::_uv_path_t p;
    unlink_cb cb;
};

int vfs::uv::uv_filesystem::unlink(vfs::uv::_uv_path_t path, unlink_cb cb) noexcept
{
    auto d = new unlink_cb_data {
        .p = path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_unlink(loop(), r, path.str().c_str(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<link_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->p, data->link_p, get_uv_error(req));
        }
        else
        {
            data->cb(data->p, data->link_p, 0);
        }

        delete data;
        delete req;
    });

    if (result != 0)
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
    vfs::uv::_uv_path_t p;
    open_cb cb;
};

int vfs::uv::uv_filesystem::open(vfs::uv::_uv_path_t path, int32_t mode, int32_t flags, open_cb cb) noexcept
{
    auto d = new open_cb_data {
        .p = path,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_open(loop(), r, path.str().c_str(), flags, mode, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<open_cb_data>(req);

        if (req->result < 0)
        {
            vfs::uv::_uv_file_t file {data->p};

            data->cb(data->p, get_uv_error(req), file);
        }
        else
        {
            vfs::uv::_uv_file_t file {data->p, get_uv_file(req)};

            data->cb(data->p, 0, file);
        }

        delete data;
        delete req;
    });

    if (result != 0)
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
    vfs::uv::_uv_file_t file;
    fstat_cb cb;
};

int vfs::uv::uv_filesystem::stat(vfs::uv::_uv_file_t file, fstat_cb cb) noexcept
{
    auto d = new fstat_cb_data {
        .file = file,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_fstat(loop(), r, file.uv_fd(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<stat_cb_data>(req);

        if (req->result < 0)
        {
            vfs::uv::_uv_stat_t uv_stat;

            data->cb(data->p, get_uv_error(req), uv_stat);
        }
        else
        {
            vfs::uv::_uv_stat_t uv_stat {req->statbuf};

            data->cb(data->p, 0, uv_stat);
        }

        delete data;
        delete req;
    });

    if (result != 0)
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
    vfs::uv::_uv_buf_t buf;
    vfs::uv::_uv_file_t f;
    read_cb cb;
};

int vfs::uv::uv_filesystem::read(vfs::uv::_uv_file_t file, vfs::uv::_uv_buf_t buf, off64_t off, read_cb cb) noexcept
{
    auto d = new read_cb_data {
        .buf = std::move(buf),
        .f = file,
        .cb = cb
    };

    auto r = new_uv_req(d);

    uv_buf_t bufs[] = {
        {.base = d->buf.data<char>(), .len = d->buf.capacity()}
    };

    auto result = uv_fs_read(loop(), r, file.uv_fd(), bufs, 1, off, [](uv_fs_t *req)
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
    vfs::uv::_uv_buf_t buf;
    vfs::uv::_uv_file_t file;
    write_cb cb;
};

int vfs::uv::uv_filesystem::write(vfs::uv::_uv_file_t file, vfs::uv::_uv_buf_t buf, off64_t off, write_cb cb) noexcept
{
    auto d = new write_cb_data {
        .buf = std::move(buf),
        .file = file,
        .cb = cb
    };

    auto r = new_uv_req(d);

    uv_buf_t bufs[] = {
        {.base = d->buf.data<char>(), .len = d->buf.size()}
    };

    auto result = uv_fs_write(loop(), r, file.uv_fd(), bufs, 1, off, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<write_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->file, get_uv_error(req), data->buf);
        }
        else
        {
            auto n_write = static_cast<uint64_t>(req->result);

            data->buf.truncate(n_write);

            data->cb(data->file, 0, data->buf);
        }

        delete data;
        delete req;
    });

    if (result != 0)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="truncate">

struct truncate_cb_data
{
    vfs::uv::_uv_file_t file;
    truncate_cb cb;
};

int vfs::uv::uv_filesystem::truncate(vfs::uv::_uv_file_t file, uint64_t size, truncate_cb cb) noexcept
{
    auto d = new truncate_cb_data {
        .file = file,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_ftruncate(loop(), r, file.uv_fd(), size, [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<truncate_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->file, get_uv_error(req), 0);
        }
        else
        {
            auto n_trunc = static_cast<uint64_t>(req->result);

            data->cb(data->file, 0, n_trunc);
        }

        delete data;
        delete req;
    });

    if (result != 0)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>

// <editor-fold desc="close">

struct close_cb_data
{
    vfs::uv::_uv_file_t file;
    close_cb cb;
};

int vfs::uv::uv_filesystem::close(vfs::uv::_uv_file_t file, close_cb cb) noexcept
{
    auto d = new close_cb_data {
        .file = file,
        .cb = cb
    };

    auto r = new_uv_req(d);

    auto result = uv_fs_close(loop(), r, file.uv_fd(), [](uv_fs_t *req)
    {
        uv_fs_req_cleanup(req);

        auto data = get_uv_data<close_cb_data>(req);

        if (req->result < 0)
        {
            data->cb(data->file, get_uv_error(req));
        }
        else
        {
            data->cb(data->file, 0);
        }

        delete data;
        delete req;
    });

    if (result != 0)
    {
        delete d;
        delete r;
    }

    return result;
}

// </editor-fold>
