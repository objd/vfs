#ifndef VFS_UNIX_PATH_H
#define VFS_UNIX_PATH_H

#include <vfs/path.hpp>

namespace vfs
{
    namespace _unix
    {
        constexpr inline const char separator_c()
        {
            return '/';
        }

        inline const std::string &separator()
        {
            static const std::string s {separator_c()};
            return s;
        }

        class unix_path :
            public vfs::path<unix_path>
        {

        private:
            std::string _value;

        public:
            unix_path() : _value("")
            {}

            explicit unix_path(std::string &value) : _value(value)
            {}

            explicit unix_path(std::string &&value) : _value(std::move(value))
            {}

            unix_path(const unix_path &other) : _value(other._value) // NOLINT
            {}

            unix_path(unix_path &&other) noexcept : _value(std::move(other._value))
            {}

            inline unix_path &operator=(const unix_path &other) // NOLINT
            {
                _value = other._value;
                return *this;
            }

            inline unix_path &operator=(unix_path &&other) noexcept
            {
                _value = other._value;
                return *this;
            }

            inline bool is_root() noexcept override
            {
                return _value == vfs::_unix::separator();
            }

            inline bool is_absolute() noexcept override
            {
                return has_leading_separator();
            }

            inline bool is_relative() noexcept override
            {
                return !has_leading_separator();
            }

            bool is_valid() noexcept override
            {
                static const std::string double_separator {
                    vfs::_unix::separator_c(),
                    vfs::_unix::separator_c()
                };

                bool is_empty = _value.empty();
                bool has_double_separator = _value.find(double_separator) != std::string::npos;

                return !is_empty && !has_double_separator;
            }

            unix_path &prepend(unix_path &path) noexcept override
            {
                bool requires_sep = !has_leading_separator() && !path.has_trailing_separator();
                bool requires_trim = has_leading_separator() && path.has_trailing_separator();

                if (requires_sep)
                {
                    _value.insert(_value.begin(), vfs::_unix::separator_c());
                }
                else if (requires_trim)
                {
                    _value.erase(0, 1);
                }

                _value.insert(0, path._value);

                return *this;
            }

            inline unix_path &prepend(std::string &val) noexcept override
            {
                unix_path path {val};
                return prepend(path);
            }

            inline unix_path &prepend(std::string &&val) noexcept override
            {
                unix_path path {val};
                return prepend(path);
            }

            unix_path &append(unix_path &path) noexcept override
            {
                bool requires_sep = !has_trailing_separator() && !path.has_leading_separator();
                bool requires_trim = has_trailing_separator() && path.has_leading_separator();

                if (requires_sep)
                {
                    _value.insert(_value.end(), vfs::_unix::separator_c());
                }
                else if (requires_trim)
                {
                    _value.resize(_value.size() - 1);
                }

                _value.append(path._value);

                return *this;
            }

            inline unix_path &append(std::string &val) noexcept override
            {
                unix_path path {val};
                return append(path);
            }

            inline unix_path &append(std::string &&val) noexcept override
            {
                unix_path path {val};
                return append(path);
            }

            unix_path parent() noexcept override
            {
                unix_path parent {_value};

                if (!is_root())
                {
                    std::string &parent_val = parent._value;

                    if (has_trailing_separator())
                    {
                        parent_val.erase(parent_val.end() - 1);
                    }

                    auto last_separator_ix = parent_val.rfind(vfs::_unix::separator_c());

                    if (last_separator_ix != std::string::npos)
                    {
                        parent_val.erase(last_separator_ix);
                    }
                    else
                    {
                        parent.prepend("..");
                    }
                }

                return parent;
            }

            inline unix_path filename() noexcept override
            {
                unix_path filename {_value};

                if (!is_root())
                {
                    std::string &filename_val = filename._value;

                    if (has_trailing_separator())
                    {
                        filename_val.erase(filename_val.end() - 1);
                    }

                    auto last_separator_ix = filename_val.rfind(vfs::_unix::separator_c());

                    if (last_separator_ix != std::string::npos)
                    {
                        filename_val.erase(0, last_separator_ix);
                    }
                }

                return filename;
            }

            inline unix_path clone() noexcept override
            {
                return unix_path {*this};
            }

            inline const std::string &str() noexcept override
            {
                return _value;
            }

            inline const char *const c_str() noexcept override
            {
                return _value.c_str();
            }

        private:

            inline bool has_leading_separator() const
            {
                return *_value.begin() == vfs::_unix::separator_c();
            }

            inline bool has_trailing_separator() const
            {
                return *(--_value.end()) == vfs::_unix::separator_c();
            }
        };
    }
}

#endif
