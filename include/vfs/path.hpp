#ifndef VFS_CPP_PATH_H
#define VFS_CPP_PATH_H

#include <string>
#include <iostream>

namespace vfs
{
    namespace paths
    {
        constexpr inline const char separator_c()
        {
            return '/';
        }

        inline const std::string &separator()
        {
            static const std::string s {vfs::paths::separator_c()};
            return s;
        }
    }

    class path
    {
    private:
        std::string _value;

    public:
        path() : _value("")
        {}

        explicit path(std::string &value) : _value(value)
        {}

        explicit path(std::string &&value) : _value(std::move(value))
        {}

        path(const path &other) : _value(other._value) // NOLINT
        {}

        path(path &&other) noexcept : _value(std::move(other._value))
        {}

        friend inline std::ostream &operator<<(std::ostream &os, const path &path)
        {
            os << path._value;
            return os;
        }

        inline path &operator=(const path &other)
        {
            _value = other._value;
        }

        inline path &operator=(path &&other) noexcept
        {
            _value = std::move(other._value);
        }

        inline bool operator==(path &other) const noexcept
        {
            return _value == other._value;
        }

        inline bool is_absolute()
        {
            return has_leading_separator();
        }

        inline bool is_relative()
        {
            return !has_leading_separator();
        }

        bool is_valid()
        {
            static const std::string double_separator {
                vfs::paths::separator_c(),
                vfs::paths::separator_c()
            };

            return !_value.empty() && _value.find(double_separator) == -1;
        }

        path &prepend(path &path) noexcept
        {
            bool requires_sep = !has_leading_separator() && !path.has_trailing_separator();
            bool requires_trim = has_leading_separator() && path.has_trailing_separator();

            if (requires_sep)
            {
                _value.insert(_value.begin(), vfs::paths::separator_c());
            }
            else if (requires_trim)
            {
                _value.erase(0, 1);
            }

            _value.insert(0, path._value);

            return *this;
        }

        inline path &prepend(std::string &val) noexcept
        {
            path path {val};
            return prepend(path);
        }

        inline path &prepend(std::string &&val) noexcept
        {
            path path {val};
            return prepend(path);
        }

        path &append(path &path) noexcept
        {
            bool requires_sep = !has_trailing_separator() && !path.has_leading_separator();
            bool requires_trim = has_trailing_separator() && path.has_leading_separator();

            if (requires_sep)
            {
                _value.insert(_value.end(), vfs::paths::separator_c());
            }
            else if (requires_trim)
            {
                _value.resize(_value.size() - 1);
            }

            _value.append(path._value);

            return *this;
        }

        inline path &append(std::string &val) noexcept
        {
            path path {val};
            return append(path);
        }

        inline path &append(std::string &&val) noexcept
        {
            path path {val};
            return append(path);
        }

//        inline path &normalize() const noexcept
//        {
//            static const int has_dot = 1;
//            static const int has_dot_dot = 2;
//            static const int has_leading_separator = 4;
//            static const int has_trailing_separator = 8;
//
//            int state = 0;
//
//            string &begin = _value.begin();
//            string &end = nullptr;
//
//            string &prev_begin = nullptr;
//            string &prev_end = nullptr;
//
//            for (auto it = _value.begin(); it != _value.end(); ++it)
//            {
//                switch (*it)
//                {
//                    case '.':
//
//                        if (state & has_dot)
//                        {
//                            state |= has_dot_dot;
//                        }
//                        else
//                        {
//                            state |= has_dot;
//                        }
//
//                        break;
//
//                    case separator_c():
//
//                        if (state & has_leading_separator)
//                        {
//                            state |= has_trailing_separator;
//                            end = it;
//                        }
//                        else
//                        {
//                            state |= has_leading_separator;
//                        }
//
//                        break;
//
//                    default:
//                        break;
//                }
//
//                if (end)
//                {
//                    if ((state & has_dot_dot) && (state & has_leading_separator))
//                    {
//                        _value.erase(prev_begin, end - 1);
//                    }
//                    else if ((state & has_dot) && (state & has_leading_separator))
//                    {
//                        _value.erase(begin, end - 1);
//                    }
//
//                    prev_begin = begin;
//                    prev_end = end;
//
//                    begin = it;
//                    end = nullptr;
//                }
//            }
//        }

        path parent() noexcept
        {
            path parent {_value};

            if (is_not_root())
            {
                std::string &parent_val = parent._value;

                if (has_trailing_separator())
                {
                    parent_val.erase(parent_val.end() - 1);
                }

                auto last_separator_ix = parent_val.rfind(vfs::paths::separator_c());

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

        inline path &&filename() noexcept
        {
            path filename {_value};

            if (is_not_root())
            {
                std::string &filename_val = filename._value;

                if (has_trailing_separator())
                {
                    filename_val.erase(filename_val.end() - 1);
                }

                auto last_separator_ix = filename_val.rfind(vfs::paths::separator_c());

                if (last_separator_ix != std::string::npos)
                {
                    filename_val.erase(0, last_separator_ix);
                }
            }

            return std::move(filename);
        }

        inline path clone() const noexcept
        {
            return path {*this};
        }

        inline const std::string &str() const noexcept
        {
            return _value;
        }

        inline const char *c_str() const noexcept
        {
            return _value.c_str();
        }

    private:

        inline bool has_leading_separator() const
        {
            return *_value.begin() == vfs::paths::separator_c();
        }

        inline bool has_trailing_separator() const
        {
            return *(_value.end() - 1) == vfs::paths::separator_c();
        }

        inline bool is_root()
        {
            return _value == vfs::paths::separator();
        }

        inline bool is_not_root()
        {
            return _value != vfs::paths::separator();
        }
    };
}

#endif
