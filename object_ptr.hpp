#ifndef JSS_OBJECT_PTR_HPP
#define JSS_OBJECT_PTR_HPP
#include <cstddef>
#include <utility>
#include <functional>
#include <memory>
#include <type_traits>

namespace jss {
    template <typename T> class object_ptr {
    public:
        constexpr object_ptr() noexcept : ptr(nullptr) {}
        constexpr object_ptr(std::nullptr_t) noexcept : ptr(nullptr) {}
        constexpr object_ptr(T *ptr_) noexcept : ptr(ptr_) {}
        constexpr object_ptr(std::shared_ptr<T> const &ptr_) noexcept :
            ptr(ptr_.get()) {}
        constexpr object_ptr(std::unique_ptr<T> const &ptr_) noexcept :
            ptr(ptr_.get()) {}

        template <
            typename U,
            typename= std::enable_if_t<std::is_convertible<U *, T *>::value>>
        constexpr object_ptr(object_ptr<U> const &other) noexcept :
            ptr(other.get()) {}

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

        void swap(object_ptr &other) noexcept {
            std::swap(ptr, other.ptr);
        }

        void reset(T *ptr_= nullptr) noexcept {
            ptr= ptr_;
        }

        friend constexpr bool
        operator==(object_ptr const &lhs, object_ptr const &rhs) noexcept {
            return lhs.ptr == rhs.ptr;
        }

        friend constexpr bool
        operator!=(object_ptr const &lhs, object_ptr const &rhs) noexcept {
            return lhs.ptr != rhs.ptr;
        }

        friend constexpr bool
        operator<(object_ptr const &lhs, object_ptr const &rhs) noexcept {
            return std::less<void>()(lhs.ptr, rhs.ptr);
        }
        friend constexpr bool
        operator>(object_ptr const &lhs, object_ptr const &rhs) noexcept {
            return rhs < lhs;
        }
        friend constexpr bool
        operator<=(object_ptr const &lhs, object_ptr const &rhs) noexcept {
            return (lhs < rhs) || (lhs == rhs);
        }
        friend constexpr bool
        operator>=(object_ptr const &lhs, object_ptr const &rhs) noexcept {
            return rhs <= lhs;
        }

    private:
        T *ptr;
    };

    template <typename T> void swap(object_ptr<T> &lhs, object_ptr<T> &rhs) {
        lhs.swap(rhs);
    }

} // namespace jss

namespace std {
    template <typename T> struct hash<jss::object_ptr<T>> {
        constexpr size_t operator()(jss::object_ptr<T> const &p) const
            noexcept {
            return hash<T *>()(p.get());
        }
    };

} // namespace std

#endif
