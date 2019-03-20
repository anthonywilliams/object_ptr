.PHONY: test

test: test_object_ptr
	test_object_ptr

test_object_ptr: test_object_ptr.cpp
	g++ -std=c++2a -o $@ $^
