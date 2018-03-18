#ifndef VFS_UNIX_PATH_H
#define VFS_UNIX_PATH_H

#include <vfs/path.hpp>

namespace vfs
{
    class unix_path :
        public base_path<unix_path>
    {

      private:
        std::string _value;

      public:
        unix_path() noexcept : _value("")
        {}

        explicit unix_path(std::string &value) noexcept
            : _value(value)
        {}

        explicit unix_path(std::string &&value) noexcept
            : _value(std::forward<std::string>(value))
        {}

        unix_path(const unix_path &lhs) noexcept
            : _value(lhs._value) // NOLINT
        {}

        inline unix_path &operator=(const unix_path &other) noexcept // NOLINT
        {
            _value = other._value;
            return *this;
        }

        inline unix_path &operator=(const path &other) noexcept
        {
            _value = other.str();
            return *this;
        }

        inline any_path to_any() noexcept
        {
            auto &self = *this;
            return vfs::any_path {self};
        }

        inline bool is_root() const noexcept override
        {
            return _value == unix_path::separator();
        }

        inline bool is_absolute() const noexcept override
        {
            return has_leading_separator();
        }

        inline bool is_relative() const noexcept override
        {
            return !has_leading_separator();
        }

        bool is_valid() const noexcept override
        {
            static const std::string double_separator {
                unix_path::separator_c(),
                unix_path::separator_c()
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
                _value.insert(_value.begin(), unix_path::separator_c());
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
                _value.insert(_value.end(), unix_path::separator_c());
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

        unix_path &parent() noexcept override
        {
            if (!is_root())
            {
                if (has_trailing_separator())
                {
                    _value.erase(_value.end() - 1);
                }

                auto last_separator_ix = _value.rfind(unix_path::separator_c());

                if (last_separator_ix != std::string::npos)
                {
                    _value.erase(last_separator_ix);
                }
                else
                {
                    prepend("..");
                }
            }

            return *this;
        }

        inline unix_path &filename() noexcept override
        {
            if (!is_root())
            {
                if (has_trailing_separator())
                {
                    _value.erase(_value.end() - 1);
                }

                auto last_separator_ix = _value.rfind(unix_path::separator_c());

                if (last_separator_ix != std::string::npos)
                {
                    _value.erase(0, last_separator_ix);
                }
            }

            return *this;
        }

        inline const std::string &str() const noexcept override
        {
            return _value;
        }

        static inline constexpr char separator_c()
        {
            return '/';
        }

        static inline const std::string &separator()
        {
            static const std::string s {separator_c()};
            return s;
        }

      private:

        inline bool has_leading_separator() const
        {
            return *_value.begin() == unix_path::separator_c();
        }

        inline bool has_trailing_separator() const
        {
            return *(--_value.end()) == unix_path::separator_c();
        }
    };
}

#endif
