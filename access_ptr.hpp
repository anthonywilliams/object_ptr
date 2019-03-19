#ifndef JSS_ACCESS_PTR_HPP
#define JSS_ACCESS_PTR_HPP
#include <cstddef>
#include <utility>
#include <functional>

namespace jss {
    template <typename T> class access_ptr {
    public:
        constexpr access_ptr() noexcept : ptr(nullptr) {}
        constexpr access_ptr(std::nullptr_t) noexcept : ptr(nullptr) {}
        constexpr access_ptr(T *ptr_) noexcept : ptr(ptr_) {}

        template <
            typename U,
            typename= std::enable_if_t<std::is_convertible<U *, T *>::value>>
        constexpr access_ptr(access_ptr<U> const &other) : ptr(other.get()) {}

        constexpr T *get() const noexcept {
            return ptr;
        }

        constexpr T &operator*() const noexcept {
            return *ptr;
        }

        constexpr T *operator->() const noexcept {
            return ptr;
        }

        constexpr explicit operator bool() const noexcept {
            return ptr != nullptr;
        }

        constexpr explicit operator T *() const noexcept {
            return ptr;
        }

        constexpr bool operator!() const noexcept {
            return !ptr;
        }

        void swap(access_ptr &other) noexcept {
            std::swap(ptr, other.ptr);
        }

        void reset(T *ptr_= nullptr) noexcept {
            ptr= ptr_;
        }

        friend constexpr bool
        operator==(access_ptr const &lhs, access_ptr const &rhs) noexcept {
            return lhs.ptr == rhs.ptr;
        }

        friend constexpr bool
        operator!=(access_ptr const &lhs, access_ptr const &rhs) noexcept {
            return lhs.ptr != rhs.ptr;
        }

        friend constexpr bool
        operator<(access_ptr const &lhs, access_ptr const &rhs) noexcept {
            return std::less<void>()(lhs.ptr, rhs.ptr);
        }
        friend constexpr bool
        operator>(access_ptr const &lhs, access_ptr const &rhs) noexcept {
            return rhs < lhs;
        }
        friend constexpr bool
        operator<=(access_ptr const &lhs, access_ptr const &rhs) noexcept {
            return (lhs < rhs) || (lhs == rhs);
        }
        friend constexpr bool
        operator>=(access_ptr const &lhs, access_ptr const &rhs) noexcept {
            return rhs <= lhs;
        }

    private:
        T *ptr;
    };

    template <typename T> void swap(access_ptr<T> &lhs, access_ptr<T> &rhs) {
        lhs.swap(rhs);
    }

} // namespace jss

namespace std {
    template <typename T> struct hash<jss::access_ptr<T>> {
        constexpr size_t operator()(jss::access_ptr<T> const &p) const
            noexcept {
            return hash<T *>()(p.get());
        }
    };

} // namespace std

#endif
