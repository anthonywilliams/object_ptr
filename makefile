.PHONY: test

test: test_access_ptr
	test_access_ptr

test_access_ptr: test_access_ptr.cpp
	g++ -std=c++2a -o $@ $^
