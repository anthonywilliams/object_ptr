.PHONY: test

ifeq ($(OS),Windows_NT)
EXE_SUFFIX=.exe
RUN_PREFIX=
else
EXE_SUFFIX=
RUN_PREFIX=./
endif

TEST_EXE=test_object_ptr$(EXE_SUFFIX)

test: $(TEST_EXE)
	$(RUN_PREFIX)$(TEST_EXE)

$(TEST_EXE): test_object_ptr.cpp
	$(CXX) -o $@ $^
