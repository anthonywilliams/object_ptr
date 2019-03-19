#include <assert.h>
#include "access_ptr.hpp"

void test_access_ptr_default_constructs_to_null() {
    jss::access_ptr<int> ap;

    assert(ap.get() == nullptr);
}

int main() {
    test_access_ptr_default_constructs_to_null();
}
