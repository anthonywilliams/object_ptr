.PHONY: test

ifeq ($(OS),Windows_NT)
EXE_SUFFIX=.exe
RUN_PREFIX=
else
EXE_SUFFIX=
RUN_PREFIX=./
endif

ifeq ($(CXX),cl)
CXXFLAGS=/std:c++17
OUTPUTFLAG=/Fe
else
CXXFLAGS=-std=c++17
OUTPUTFLAG=-o 
endif

TEST_EXE=test_object_ptr$(EXE_SUFFIX)

test: $(TEST_EXE)
	$(RUN_PREFIX)$(TEST_EXE)

$(TEST_EXE): test_object_ptr.cpp object_ptr.hpp
	$(CXX) $(CXXFLAGS) $(OUTPUTFLAG)$@ $<
