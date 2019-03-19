#include "access_ptr.hpp"
#include <assert.h>
#include <type_traits>

void test_access_ptr_default_constructs_to_null() {
    jss::access_ptr<int> ap;
    assert(ap.get() == nullptr);
}

void test_access_ptr_can_be_constructed_from_nullptr() {
    jss::access_ptr<int> ap(nullptr);
    assert(ap.get() == nullptr);

    jss::access_ptr<int> ap2= nullptr;
    assert(ap2.get() == nullptr);

    [](jss::access_ptr<int> ap3) { assert(ap3.get() == nullptr); }(nullptr);
}

void test_access_ptr_can_be_constructed_from_raw_pointer() {
    int x;
    jss::access_ptr<int> ap(&x);
    assert(ap.get() == &x);

    jss::access_ptr<int> ap2= &x;
    assert(ap2.get() == &x);

    [&](jss::access_ptr<int> ap3) { assert(ap3.get() == &x); }(&x);
}

void test_access_ptr_can_be_dereferenced() {
    int x;
    jss::access_ptr<int> ap(&x);
    static_assert((std::is_same<decltype(*ap), int &>::value));

    assert(&*ap == &x);
}

void test_access_ptr_has_operator_arrow() {
    struct X {
        int data;
    };

    X x{42};

    jss::access_ptr<X> ap(&x);
    static_assert((std::is_same<decltype(ap.operator->()), X *>::value));

    assert(ap.operator->() == &x);
    assert(ap->data == 42);
}

void test_access_ptr_can_be_converted_to_bool() {
    int x;
    jss::access_ptr<int> ap(&x);
    assert(static_cast<bool>(ap));

    jss::access_ptr<int> ap2;
    assert(!static_cast<bool>(ap2));

    jss::access_ptr<int> ap3(nullptr);
    assert(!static_cast<bool>(ap3));
}

void test_access_ptr_can_be_copied() {
    class X {};

    static_assert(
        std::is_nothrow_copy_constructible<jss::access_ptr<X>>::value);

    X x;

    jss::access_ptr<X> ap(&x);
    jss::access_ptr<X> ap2(ap);

    assert(ap.get() == &x);
    assert(ap2.get() == &x);
    assert(ap);
    assert(ap2);
}

void test_access_ptr_can_be_assigned() {
    class X {};

    static_assert(std::is_nothrow_copy_assignable<jss::access_ptr<X>>::value);

    X x;

    jss::access_ptr<X> ap(&x);
    jss::access_ptr<X> ap2;

    assert(!ap2);

    ap2= ap;

    assert(ap.get() == &x);
    assert(ap2.get() == &x);
    assert(ap);
    assert(ap2);
}

void test_access_ptr_can_be_swapped() {
    class Y {};
    Y y1, y2;

    jss::access_ptr<Y> ap1(&y1), ap2(&y2);

    ap1.swap(ap2);

    assert(ap1.get() == &y2);
    assert(ap2.get() == &y1);

    using std::swap;
    swap(ap1, ap2);
    assert(ap1.get() == &y1);
    assert(ap2.get() == &y2);
}

void test_access_ptr_equality() {
    class Z {};

    Z z1, z2;

    jss::access_ptr<Z> ap1(&z1), ap2(&z2);

    static_assert((std::is_same<decltype(ap1 == ap2), bool>::value));
    assert(ap1 != ap2);
    assert(!(ap1 == ap2));

    ap1= ap2;

    assert(ap1 == ap2);
    assert(!(ap1 != ap2));

    assert(ap1 != nullptr);
    assert(nullptr != ap1);
    assert(!(ap1 == nullptr));
    assert(!(nullptr == ap1));

    ap2= nullptr;

    assert(ap2 == nullptr);
    assert(nullptr == ap2);
    assert(!(ap2 != nullptr));
    assert(!(nullptr != ap2));
}

void test_access_ptr_can_be_reset() {
    class A {};

    A a;

    jss::access_ptr<A> ap;

    ap.reset(&a);
    assert(ap.get() == &a);
    assert(&*ap == &a);

    ap.reset();
    assert(ap.get() == nullptr);

    ap.reset(&a);
    assert(ap.get() == &a);
    ap.reset(nullptr);
    assert(ap.get() == nullptr);
}

void test_access_ptr_has_not_operator() {
    int x;
    jss::access_ptr<int> ap(&x);
    static_assert(std::is_same<decltype(!ap), bool>::value);
    assert(!!ap);

    jss::access_ptr<int> ap2;
    assert(!ap2);

    jss::access_ptr<int> ap3(nullptr);
    assert(!ap3);
}

void test_access_ptr_has_ordering_comparisons() {
    int x[2];

    jss::access_ptr<int> ap1(&x[0]), ap2(&x[1]);

    assert(ap1 < ap2);
    assert(ap1 <= ap2);
    assert(!(ap2 < ap1));
    assert(!(ap2 <= ap1));
    assert(ap2 > ap1);
    assert(ap2 >= ap1);
    assert(!(ap1 > ap2));
    assert(!(ap1 >= ap2));

    jss::access_ptr<int> ap3(ap1);

    assert(!(ap1 < ap3));
    assert(!(ap3 < ap1));
    assert(ap1 <= ap3);
    assert(ap3 <= ap1);
    assert(!(ap1 > ap3));
    assert(!(ap3 > ap1));
    assert(ap1 >= ap3);
    assert(ap3 >= ap1);

    jss::access_ptr<int> np;

    assert((np < ap1) != (ap1 < np));
    assert((np > ap1) != (ap1 > np));
    assert((np < ap1) == (ap1 > np));
    assert((np > ap1) == (ap1 < np));
    assert((np <= ap1) == (ap1 >= np));
    assert((np >= ap1) == (ap1 <= np));
    assert((np >= ap1) == (ap1 < np));
    assert((np <= ap1) == (ap1 > np));
}

void test_access_ptr_implicit_conversions() {
    class Base {};
    class Derived : public Base {};

    Derived d;

    jss::access_ptr<Base> ap(&d);

    assert(ap.get() == &d);

    jss::access_ptr<Derived> ap2(&d);
    jss::access_ptr<Base> ap3(ap2);

    assert(ap3.get() == &d);
    assert(ap3 == ap2);

    ap3.reset();
    ap3= ap2;

    assert(ap3.get() == &d);
    assert(ap3 == ap2);
}

void test_access_ptr_can_be_explicitly_converted_to_raw_pointer() {
    int x;
    jss::access_ptr<int> ap(&x);

    assert(static_cast<int *>(ap) == &x);

    static_assert(!std::is_convertible<jss::access_ptr<int>, int *>::value);
}

int main() {
    test_access_ptr_default_constructs_to_null();
    test_access_ptr_can_be_constructed_from_nullptr();
    test_access_ptr_can_be_constructed_from_raw_pointer();
    test_access_ptr_can_be_dereferenced();
    test_access_ptr_has_operator_arrow();
    test_access_ptr_can_be_converted_to_bool();
    test_access_ptr_can_be_copied();
    test_access_ptr_can_be_assigned();
    test_access_ptr_can_be_swapped();
    test_access_ptr_equality();
    test_access_ptr_can_be_reset();
    test_access_ptr_has_not_operator();
    test_access_ptr_has_ordering_comparisons();
    test_access_ptr_implicit_conversions();
    test_access_ptr_can_be_explicitly_converted_to_raw_pointer();
}
