#ifndef VFS_CPP_PATH_H
#define VFS_CPP_PATH_H

#include <string>
#include <iostream>

namespace vfs
{
    using std::string;
    using std::ostream;

    class path
    {
    private:
        string _value;

    public:
        path() : _value("")
        {}

        explicit path(string &value) : _value(value)
        {}

        explicit path(string &&value) : _value(value)
        {}

        path(const path &other) : _value(other._value) // NOLINT
        {}

        path(path &&other) noexcept : _value(std::move(other._value))
        {}

        friend inline ostream &operator<<(ostream &os, const path &path)
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

        inline path &prepend(path &path) noexcept
        {
            bool requires_sep = !has_leading_separator() && !path.has_trailing_separator();

            if (requires_sep)
            {
                _value.insert(0, separator());
            }

            _value.insert(0, path._value);
        }

        inline path &prepend(string &val) noexcept
        {
            path path {val};
            return prepend(path);
        }

        inline path &prepend(string &&val) noexcept
        {
            path path {val};
            return prepend(path);
        }

        inline path &append(path &path) noexcept
        {
            bool requires_sep = !has_trailing_separator() && !path.has_leading_separator();

            if (requires_sep)
            {
                _value.append(separator());
            }

            _value.append(path._value);
        }

        inline path &append(string &val) noexcept
        {
            path path {val};
            return append(path);
        }

        inline path &append(string &&val) noexcept
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

        inline path parent() noexcept
        {
            if (is_root())
            {
                return clone();
            }

            path parent {_value};
            string &parent_val = parent._value;

            if (has_trailing_separator())
            {
                parent_val.erase(parent_val.end() - 1);
            }

            auto index = parent_val.rfind(separator_c());
            parent_val.erase(index);

            return parent;
        }

        inline path filename() noexcept
        {
            if (is_root())
            {
                return clone();
            }

            path filename {_value};
            string &filename_val = filename._value;

            if (has_trailing_separator())
            {
                filename_val.erase(filename_val.end() - 1);
            }

            auto index = filename_val.rfind(separator_c());
            filename_val.erase(0, index);

            return filename;
        }

        inline path clone() const noexcept
        {
            return path {*this};
        }

        inline const string str() const noexcept
        {
            return _value;
        }

        inline const char *c_str() const noexcept
        {
            return _value.c_str();
        }

    private:
        constexpr inline const char separator_c() const
        {
            return '/';
        }

        inline const string &separator() const
        {
            static const string s {separator_c()};
            return s;
        }

        inline bool has_leading_separator() const
        {
            return *_value.begin() == separator_c();
        }

        inline bool has_trailing_separator() const
        {
            return *_value.end() == separator_c();
        }

        inline bool is_root()
        {
            return _value == separator();
        }
    };
}

#endif
