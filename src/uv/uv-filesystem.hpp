#ifndef VFS_CPP_UV_FILESYSTEM_H
#define VFS_CPP_UV_FILESYSTEM_H

#include <vector>
#include <uv.h>

#include <vfs/path.hpp>
#include <vfs/filesystem.hpp>

namespace vfs
{
    namespace uv
    {
        using std::vector;
        using vfs::path;

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

//        typedef filesystem <uv_stat_t, uv_file, std::vector<uint8_t>> uv_filesystem_i;
//        typedef typename uv_filesystem_i::exists_cb      exists_cb;
//        typedef typename uv_filesystem_i::stat_cb        stat_cb;
//        typedef typename uv_filesystem_i::mkdir_cb       mkdir_cb;
//        typedef typename uv_filesystem_i::mkdirs_cb      mkdirs_cb;
//        typedef typename uv_filesystem_i::create_cb      create_cb;
//        typedef typename uv_filesystem_i::move_cb        move_cb;
//        typedef typename uv_filesystem_i::copy_cb        copy_cb;
//        typedef typename uv_filesystem_i::link_cb        link_cb;
//        typedef typename uv_filesystem_i::symlink_cb     symlink_cb;
//        typedef typename uv_filesystem_i::unlink_cb      unlink_cb;
//        typedef typename uv_filesystem_i::open_cb        open_cb;
//        typedef typename uv_filesystem_i::fstat_cb       fstat_cb;
//        typedef typename uv_filesystem_i::read_cb        read_cb;
//        typedef typename uv_filesystem_i::write_cb       write_cb;
//        typedef typename uv_filesystem_i::truncate_cb    truncate_cb;
//        typedef typename uv_filesystem_i::close_cb       close_cb;

        class uv_filesystem : public filesystem<uv_stat_t, uv_file, std::vector<char>>
        {
        private:
            uv_loop_t *_uv_loop;

        public:

            explicit uv_filesystem(uv_loop_t *uv_loop) : _uv_loop(uv_loop)
            {};

        public:
            int exists(vfs::path &path, exists_cb cb) noexcept override;
            int stat(vfs::path &path, stat_cb cb) noexcept override;
            int mkdir(vfs::path &path, int32_t mode, mkdir_cb cb) noexcept override;
            int mkdirs(vfs::path &path, int32_t mode, mkdirs_cb cb) noexcept override;
            int create(vfs::path &path, int32_t mode, create_cb cb) noexcept override;
            int move(vfs::path &path, vfs::path &move_path, move_cb cb) noexcept override;
            int copy(vfs::path &path, vfs::path &copy_path, copy_cb cb) noexcept override;
            int link(vfs::path &path, vfs::path &other_path, link_cb cb) noexcept override;
            int symlink(vfs::path &path, vfs::path &other_path, symlink_cb cb) noexcept override;
            int unlink(vfs::path &path, unlink_cb cb) noexcept override;
            int open(vfs::path &path, int32_t mode, int32_t flags, open_cb cb) noexcept override;
            int stat(uv_file &file, fstat_cb cb) noexcept override;
            int read(uv_file &file, vector<char> &buf, off64_t off, read_cb cb) noexcept override;
            int write(uv_file &file, vector<char> &buf, off64_t off, write_cb cb) noexcept override;
            int truncate(uv_file &file, uint64_t size, off64_t off, truncate_cb cb) noexcept override;
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
//            vfs::path &p;
//            exists_cb cb;
//        };
//
//        int exists(vfs::path &path, exists_cb cb) noexcept noexcept override
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
//            vfs::path p;
//            stat_cb cb;
//        };
//
//        int stat(vfs::path &path, stat_cb cb) noexcept noexcept override
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
//            vfs::path p;
//            mkdir_cb cb;
//        };
//
//        int mkdir(vfs::path &path, mode_t mode, mkdir_cb cb) noexcept noexcept override
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
//            vfs::path p;
//            vfs::path prev_p;
//            mode_t mode;
//            mkdirs_cb cb;
//            bool is_root;
//            uv_fs_cb uv_fs_mkdir_cb;
//        };
//
//        int mkdirs(vfs::path &path, mode_t mode, mkdirs_cb cb) noexcept noexcept override
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
////            mkdirs_cb recursive_cb = [this, data](objd::fs::vfs::path &p, int err)
////            {
////                if (err == ENOENT)
////                {
////                    objd::fs::vfs::path parent = p.parent();
////                    mkdirs(parent, data->mode, [this, data](objd::fs::vfs::path &p2, int err2)
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
//            vfs::path p;
//            create_cb cb;
//        };
//
//        int create(vfs::path &path, mode_t mode, create_cb cb) noexcept noexcept override
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
//            vfs::path p;
//            open_cb cb;
//        };
//
//        int open(vfs::path &path, mode_t mode, int flags, open_cb cb) noexcept noexcept override
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
