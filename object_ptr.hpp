#ifndef JSS_OBJECT_PTR_HPP
#define JSS_OBJECT_PTR_HPP
#include <cstddef>
#include <utility>
#include <functional>
#include <memory>
#include <type_traits>

namespace jss {
    namespace detail {
        template <typename Ptr, bool= std::is_class<Ptr>::value>
        struct has_smart_pointer_ops {
            using false_test= char;
            template <typename T> struct true_test { false_test dummy[2]; };

            static false_test test_op_star(...);
            static true_test<decltype(*std::declval<Ptr const &>())>
            test_op_star(Ptr *);

            static false_test test_op_arrow(...);
            static true_test<decltype(std::declval<Ptr const &>().operator->())>
            test_op_arrow(Ptr *);

            static false_test test_get(...);
            static true_test<decltype(std::declval<Ptr const &>().get())>
            test_get(Ptr *);

            static constexpr bool value=
                !std::is_same<decltype(test_get(0)), false_test>::value &&
                !std::is_same<decltype(test_op_arrow(0)), false_test>::value &&
                !std::is_same<decltype(test_op_star(0)), false_test>::value;
        };

        template <typename Ptr>
        struct has_smart_pointer_ops<Ptr, false> : std::false_type {};

        template <typename Ptr>
        struct smart_pointer_ops_consistent
            : std::integral_constant<
                  bool,
                  std::is_pointer<decltype(
                      std::declval<Ptr const &>().get())>::value &&
                      std::is_reference<decltype(
                          *std::declval<Ptr const &>())>::value &&
                      std::is_pointer<decltype(
                          std::declval<Ptr const &>().operator->())>::value &&
                      std::is_same<
                          decltype(std::declval<Ptr const &>().get()),
                          decltype(std::declval<Ptr const &>().
                                   operator->())>::value &&
                      std::is_same<
                          decltype(*std::declval<Ptr const &>().get()),
                          decltype(*std::declval<Ptr const &>())>::value> {};

        template <typename Ptr, bool= has_smart_pointer_ops<Ptr>::value>
        struct is_smart_pointer
            : std::integral_constant<
                  bool, smart_pointer_ops_consistent<Ptr>::value> {};

        template <typename Ptr>
        struct is_smart_pointer<Ptr, false> : std::false_type {};

        template <typename Ptr, typename T, bool= is_smart_pointer<Ptr>::value>
        struct is_convertible_smart_pointer
            : std::integral_constant<
                  bool, std::is_convertible<
                            decltype(std::declval<Ptr const &>().get()),
                            T *>::value> {};

        template <typename Ptr, typename T>
        struct is_convertible_smart_pointer<Ptr, T, false> : std::false_type {};

    }

    template <typename T> class object_ptr {
    public:
        constexpr object_ptr() noexcept : ptr(nullptr) {}
        constexpr object_ptr(std::nullptr_t) noexcept : ptr(nullptr) {}
        constexpr object_ptr(T *ptr_) noexcept : ptr(ptr_) {}
        template <
            typename U,
            typename= std::enable_if_t<std::is_convertible<U *, T *>::value>>
        constexpr object_ptr(U *ptr_) noexcept : ptr(ptr_) {}

        template <
            typename Ptr,
            typename= std::enable_if_t<
                detail::is_convertible_smart_pointer<Ptr, T>::value>>
        constexpr object_ptr(Ptr const &other) noexcept : ptr(other.get()) {}

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
            return !(rhs < lhs);
        }
        friend constexpr bool
        operator>=(object_ptr const &lhs, object_ptr const &rhs) noexcept {
            return rhs <= lhs;
        }

    private:
        T *ptr;
    };

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
