#ifndef JSS_ACCESS_PTR_HPP
#define JSS_ACCESS_PTR_HPP
#include <cstddef>

namespace jss {
    template <typename T> class access_ptr {
    public:
        constexpr access_ptr() noexcept : ptr(nullptr) {}
        constexpr access_ptr(std::nullptr_t) noexcept : ptr(nullptr) {}
        constexpr access_ptr(T *ptr_) noexcept : ptr(ptr_) {}

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

    private:
        T *ptr;
    };
} // namespace jss

#endif
