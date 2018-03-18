#ifndef VFS_CPP_UV_FILESYSTEM_H
#define VFS_CPP_UV_FILESYSTEM_H

#include <vector>
#include <uv.h>

#include <vfs/path.hpp>
#include <vfs/filesystem.hpp>
#include "vfs/unix/unix-path.hpp"

namespace vfs
{
    namespace uv
    {
        inline int get_uv_error(uv_fs_t *req)
        {
            return static_cast<int>(-(req->result));
        }

        inline uv_file get_uv_file(uv_fs_t *req)
        {
            return static_cast<uv_file>(req->result);
        }

        template<typename t_data>
        inline t_data *get_uv_data(uv_fs_t *req)
        {
            return static_cast<t_data *>(req->data);
        }

        class uv_filesystem :
            public filesystem<vfs::any_path, uv_stat_t, uv_file, vfs::buffer>
        {

          private:
            uv_loop_t *_uv_loop;

          public:
            uv_filesystem()
                : _uv_loop(nullptr)
            {};

            explicit uv_filesystem(uv_loop_t *uv_loop)
                : _uv_loop(uv_loop)
            {};

            int exists(vfs::any_path &&path, exists_cb cb) noexcept override;
            int stat(vfs::any_path &&path, stat_cb cb) noexcept override;
            int mkdir(vfs::any_path &&path, int32_t mode, mkdir_cb cb) noexcept override;
            int mkdirs(vfs::any_path &&path, int32_t mode, mkdirs_cb cb) noexcept override;
            int create(vfs::any_path &&path, int32_t mode, create_cb cb) noexcept override;
            int move(vfs::any_path &&path, vfs::any_path &&move_path, move_cb cb) noexcept override;
            int copy(vfs::any_path &&path, vfs::any_path &&copy_path, copy_cb cb) noexcept override;
            int link(vfs::any_path &&path, vfs::any_path &&other_path, link_cb cb) noexcept override;
            int symlink(vfs::any_path &&path, vfs::any_path &&link_path, symlink_cb cb) noexcept override;
            int unlink(vfs::any_path &&path, unlink_cb cb) noexcept override;
            int open(vfs::any_path &&path, int32_t mode, int32_t flags, open_cb cb) noexcept override;
            int stat(uv_file &file, fstat_cb cb) noexcept override;
            int read(uv_file &file, vfs::buffer &&buf, off64_t off, read_cb cb) noexcept override;
            int write(uv_file &file, vfs::buffer &&buf, off64_t off, write_cb cb) noexcept override;
            int truncate(uv_file &file, uint64_t size, truncate_cb cb) noexcept override;
            int close(uv_file &file, close_cb cb) noexcept override;

        };
    }
}

#endif

//#ifndef VFS_CPP_UV_FILESYSTEM_H
//#define VFS_CPP_UV_FILESYSTEM_H
//
//#include <uv.h>
//#include <memory>
//
//#include <vfs/filesystem.hpp>
//
//namespace vfs::uv
//{
//    inline int get_uv_error(uv_fs_t *req)
//    {
//        return static_cast<int>(-(req->result));
//    }
//
//    inline uv_file get_uv_file(uv_fs_t *req)
//    {
//        return static_cast<uv_file>(req->result);
//    }
//
//    template<typename t_data>
//    inline t_data *get_uv_data(uv_fs_t *req)
//    {
//        return static_cast<t_data *>(req->data);
//    }
//
//    typedef uv_file file_t;
//    typedef uv_stat_t stat_t;
//    typedef std::vector<uint8_t> buf_t;
//
//    class uv_filesystem : public filesystem<stat_t, file_t, buf_t>
//    {
//    private:
//        uv_loop_t *_uv_loop;
//
//    public:
//
//        explicit uv_filesystem(uv_loop_t *uv_loop)
//            : _uv_loop(uv_loop)
//        {}
//
//        struct exists_cb_data
//        {
//            vfs::any_path &p;
//            exists_cb cb;
//        };
//
//        int exists(vfs::any_path &path, exists_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new exists_cb_data{
//                    .p = path,
//                    .cb = cb
//                }
//            };
//
//            uv_fs_stat(_uv_loop, r, path.c_str(), [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<exists_cb_data>(req);
//
//                if (req->result < 0)
//                {
//                    int err = get_uv_error(req);
//
//                    if (err == ENOENT)
//                    {
//                        data->cb(data->p, 0, false);
//                    }
//                    else
//                    {
//                        data->cb(data->p, err, false);
//                    }
//                }
//                else
//                {
//                    data->cb(data->p, 0, true);
//                }
//
//                delete data;
//                delete req;
//            });
//
//            return 0;
//        }
//
//        struct stat_cb_data
//        {
//            vfs::any_path p;
//            stat_cb cb;
//        };
//
//        int stat(vfs::any_path &path, stat_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new stat_cb_data{
//                    .p = path,
//                    .cb = cb
//                }
//            };
//
//            uv_fs_stat(_uv_loop, r, path.c_str(), [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<stat_cb_data>(req);
//
//                if (req->result < 0)
//                {
//                    data->cb(data->p, get_uv_error(req), {});
//                }
//                else
//                {
//                    data->cb(data->p, 0, req->statbuf);
//                }
//
//                delete data;
//                delete req;
//            });
//
//            return 0;
//        }
//
//        struct mkdir_cb_data
//        {
//            vfs::any_path p;
//            mkdir_cb cb;
//        };
//
//        int mkdir(vfs::any_path &path, mode_t mode, mkdir_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new mkdir_cb_data{.p = path, .cb = cb}
//            };
//
//            uv_fs_mkdir(_uv_loop, r, path.c_str(), mode, [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<mkdir_cb_data>(req);
//
//                if (req->result < 0)
//                {
//                    data->cb(data->p, get_uv_error(req));
//                }
//                else
//                {
//                    data->cb(data->p, 0);
//                }
//
//                delete data;
//                delete req;
//            });
//
//            return 0;
//        }
//
//        struct mkdirs_cb_data
//        {
//            uv_filesystem &self;
//            vfs::any_path p;
//            vfs::any_path prev_p;
//            mode_t mode;
//            mkdirs_cb cb;
//            bool is_root;
//            uv_fs_cb uv_fs_mkdir_cb;
//        };
//
//        int mkdirs(vfs::any_path &path, mode_t mode, mkdirs_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new mkdirs_cb_data{
//                    .self = *this,
//                    .p = path,
//                    .prev_p = {},
//                    .mode = mode,
//                    .cb = cb,
//                    .is_root = true
//                }
//            };
//
//            uv_fs_cb uv_fs_mkdir_cb = [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<mkdirs_cb_data>(req);
//                auto uv_loop = data->self._uv_loop;
//
//                if (req->result < 0)
//                {
//                    auto err = static_cast<int>(-(req->result));
//
//                    if (err == ENOENT)
//                    {
//                        uv_fs_t *forward_r = new uv_fs_t{
//                            .data = new mkdirs_cb_data{
//                                .self = data->self,
//                                .p = data->p.parent(),
//                                .prev_p = data->p,
//                                .mode = data->mode,
//                                .cb = data->cb,
//                                .is_root = false,
//                                .uv_fs_mkdir_cb = data->uv_fs_mkdir_cb
//                            }
//                        };
//
//                        uv_fs_mkdir(uv_loop, forward_r, data->p.c_str(), data->mode, data->uv_fs_mkdir_cb);
//                    }
//                    else if (err == EEXIST)
//                    {
//                        goto asdf;
//                    }
//                    else
//                    {
//                        data->cb(data->p, err);
//                    }
//                }
//                else
//                {
//asdf:
//                    if (data->prev_p.str().empty())
//                    {
//                        data->cb(data->p, 0);
//                    }
//                    else
//                    {
//                        uv_fs_t *backward_r = new uv_fs_t{
//                            .data = new mkdirs_cb_data{
//                                .self = data->self,
//                                .p = data->prev_p,
//                                .prev_p = data->p,
//                                .mode = data->mode,
//                                .cb = data->cb,
//                                .is_root = false,
//                                .uv_fs_mkdir_cb = data->uv_fs_mkdir_cb
//                            }
//                        };
//
//                        uv_fs_mkdir(uv_loop, backward_r, data->prev_p.c_str(), data->mode, data->uv_fs_mkdir_cb);
//                    }
//                }
//
//                delete data;
//                delete req;
//            };
//
//            auto *d = static_cast<mkdirs_cb_data *>(r->data);
//            d->uv_fs_mkdir_cb = uv_fs_mkdir_cb;
//
//            return uv_fs_mkdir(_uv_loop, r, path.c_str(), mode, uv_fs_mkdir_cb);
//
////            uv_fs_cb (*uv_fs_mkdir_cb)(uv_fs_t *req)
////            {
////                uv_fs_req_cleanup(req);
////
////                mkdirs_cb_data *data = static_cast<mkdirs_cb_data *>(req->data);
////
////                if (req->result < 0)
////                {
////                    int err = static_cast<int>(-(req->result));
////
////                    if (err == ENOENT)
////                    {
////                        uv_fs_t *recursive_r = new uv_fs_t {
////                            .data = new mkdirs_cb_data {
////                                .self = data->self,
////                                .p = data->p.parent(),
////                                .mode = data->mode,
////                                .cb = data->cb
////                            }
////                        };
////
////                        uv_loop_t *uv_loop = data->self->_uv_loop;
////
////                        err = uv_fs_mkdir(uv_loop, recursive_r, data->p.c_str(), data->mode, [](uv_fs_t *recursive_req)
////                        {
////                            uv_fs_req_cleanup(recursive_req);
////
////                            mkdirs_cb_data *recursive_data = static_cast<mkdirs_cb_data *>(req->data);
////
////                            uv_fs_mkdir(uv_loop, recursive_req, data->p.c_str(), data->mode, [](uv_fs_t *recursive_req)
////                            {
////
////                                delete data;
////                                delete req;
////                            });
////                        });
////                    }
////
////                    data->cb(data->p, err);
////                }
////                else
////                {
////                    data->cb(data->p, 0);
////                }
////
////                delete data;
////                delete req;
////            };
//
////            uv_fs_mkdir(_uv_loop, r, path.c_str(), mode, [](uv_fs_t *req)
////            {
////                uv_fs_req_cleanup(req);
////
////                mkdirs_cb_data *data = static_cast<mkdirs_cb_data *>(req->data);
////
////                if (req->result < 0)
////                {
////                    int err = static_cast<int>(-(req->result));
////
////                    if (err == ENOENT)
////                    {
////
////                    }
////
////                    data->cb(data->p, err);
////                }
////                else
////                {
////                    data->cb(data->p, 0);
////                }
////
////                delete data;
////                delete req;
////            });
////
////            return 0;
//
//
//
////            mkdirs_cb_data *data = new mkdirs_cb_data {.p = path, .mode = mode, .cb = cb};
////
////            mkdirs_cb recursive_cb = [this, data](objd::fs::vfs::any_path &p, int err)
////            {
////                if (err == ENOENT)
////                {
////                    objd::fs::vfs::any_path parent = p.parent();
////                    mkdirs(parent, data->mode, [this, data](objd::fs::vfs::any_path &p2, int err2)
////                    {
////                        mkdir(data->p, data->mode, data->cb);
////                        delete data;
////                    });
////                }
////                else if (err)
////                {
////                    data->cb(p, err);
////                }
////                else
////                {
////                    data->cb(p, 0);
////                }
////            };
////
////            return mkdir(path, mode, recursive_cb);
//        }
//
//        struct create_cb_data
//        {
//            uv_filesystem &self;
//            vfs::any_path p;
//            create_cb cb;
//        };
//
//        int create(vfs::any_path &path, mode_t mode, create_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new create_cb_data{.self = *this, .p = path, .cb = cb}
//            };
//
//            uv_fs_open(_uv_loop, r, path.c_str(), UV_FS_O_CREAT, mode, [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<create_cb_data>(req);
//
//                if (req->result < 0)
//                {
//                    data->cb(data->p, get_uv_error(req));
//                }
//                else
//                {
//                    data->cb(data->p, 0);
//                }
//
//                uv_file file = static_cast<uv_file>(req->result);
//
//                uv_fs_close(data->self._uv_loop, req, file, [](uv_fs_t *req)
//                {
//                    uv_fs_req_cleanup(req);
//
//                    auto d = get_uv_data<create_cb_data>(req);
//
//                    delete d;
//                    delete req;
//                });
//            });
//
//            return 0;
//        }
//
//        struct open_cb_data
//        {
//            vfs::any_path p;
//            open_cb cb;
//        };
//
//        int open(vfs::any_path &path, mode_t mode, int flags, open_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new open_cb_data{.p = path, .cb = cb}
//            };
//
//            uv_fs_open(_uv_loop, r, path.c_str(), flags, mode, [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<open_cb_data>(req);
//
//                if (req->result < 0)
//                {
//                    data->cb(data->p, get_uv_error(req), -1);
//                }
//                else
//                {
//                    data->cb(data->p, 0, get_uv_file(req));
//                }
//
//                delete data;
//                delete req;
//            });
//
//            return 0;
//        }
//
//        struct fstat_cb_data
//        {
//            file_t f;
//            fstat_cb cb;
//        };
//
//        int fstat(file_t &file, fstat_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new fstat_cb_data{
//                    .f = file,
//                    .cb = cb
//                }
//            };
//
//            uv_fs_fstat(_uv_loop, r, file, [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<fstat_cb_data>(req);
//
//                if (req->result < 0)
//                {
//                    data->cb(data->f, get_uv_error(req), {});
//                }
//                else
//                {
//                    data->cb(data->f, 0, req->statbuf);
//                }
//
//                delete data;
//                delete req;
//            });
//
//            return 0;
//        }
//
//        struct read_cb_data
//        {
//            file_t f;
//            buf_t buf;
//            read_cb cb;
//        };
//
//        int read(file_t &file, buf_t &buf, read_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new read_cb_data{
//                    .f = file,
//                    .buf = {buf},
//                    .cb = cb
//                }
//            };
//
//            uv_buf_t bufs[] = {
//                {.base = static_cast<char *>(buf.data()), .len = buf.size()}
//            };
//
//            uv_fs_read(_uv_loop, r, file, bufs, sizeof bufs, 0, [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<read_cb_data>(req);
//
//                if (req->result < 0)
//                {
//                    buf_t dummy;
//                    data->cb(data->f, get_uv_error(req), dummy);
//                }
//                else
//                {
//                    data->cb(data->f, 0, data->buf);
//                }
//
//                delete data;
//                delete req;
//            });
//
//            return 0;
//        }
//
//        struct write_cb_data
//        {
//            file_t f;
//            buf_t buf;
//            write_cb cb;
//        };
//
//        int write(file_t &file, buf_t &buf, write_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new write_cb_data{
//                    .f = file,
//                    .buf = buf,
//                    .cb = cb
//                }
//            };
//
//            uv_buf_t bufs[] = {
//                {.base = static_cast<char *>(buf.data()), .len = buf.size()}
//            };
//
//            uv_fs_write(_uv_loop, r, file, bufs, 1, 0, [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<write_cb_data>(req);
//
//                if (req->result < 0)
//                {
//                    buf_t dummy;
//                    data->cb(data->f, get_uv_error(req), dummy);
//                }
//                else
//                {
//                    data->cb(data->f, 0, data->buf);
//                }
//
//                delete data;
//                delete req;
//            });
//
//            return 0;
//        }
//
//        struct close_cb_data
//        {
//            file_t f;
//            close_cb cb;
//        };
//
//        int close(file_t &file, close_cb cb) noexcept noexcept override
//        {
//            uv_fs_t *r = new uv_fs_t{
//                .data = new close_cb_data{
//                    .f = file,
//                    .cb = cb
//                }
//            };
//
//            uv_fs_close(_uv_loop, r, file, [](uv_fs_t *req)
//            {
//                uv_fs_req_cleanup(req);
//
//                auto data = get_uv_data<close_cb_data>(req);
//
//                if (req->result < 0)
//                {
//                    data->cb(data->f, get_uv_error(req));
//                }
//                else
//                {
//                    data->cb(data->f, 0);
//                }
//
//                delete data;
//                delete req;
//            });
//
//            return 0;
//        }
//    };
//}
//
//#endif
