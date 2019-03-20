#include "object_ptr.hpp"
#include <iostream>

void foo(jss::object_ptr<int> p) {
    if(p) {
        std::cout << *p;
    } else {
        std::cout << "(null)";
    }
    std::cout << "\n";
}

int main() {
    foo(nullptr);
    int x= 42;
    foo(&x);

    auto sp= std::make_shared<int>(123);
    foo(sp);
    auto up= std::make_unique<int>(456);
    foo(up);
}
