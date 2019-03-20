.PHONY: test

test: test_object_ptr
	test_object_ptr

test_object_ptr: test_object_ptr.cpp
	$(CXX) -o $@ $^
