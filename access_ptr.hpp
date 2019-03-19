#ifndef JSS_ACCESS_PTR_HPP
#define JSS_ACCESS_PTR_HPP

namespace jss {
    template <typename T> class access_ptr {
    public:
        constexpr access_ptr() noexcept : ptr(nullptr) {}

        constexpr T *get() const noexcept {
            return ptr;
        }

    private:
        T *ptr;
    };
} // namespace jss

#endif
