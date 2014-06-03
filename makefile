TEST_SYSTEM_FLAGS = -isystem gtest-1.7.0/include -isystem gmock-1.7.0/include 
TEST_I_FLAGS = -I gtest-1.7.0 -I gmock-1.7.0
TESTS = tests/test_simple.cpp
TEST_OBJECTS = $(TESTS:.cpp=.o)
SOURCES = src/graph.cpp
OBJECTS = $(SOURCES:.cpp=.o)
CFLAGS = -std=c++11
	
all : $(OBJECTS)
	@$(CXX) $(CFLAGS) $(OBJECTS) src/main.cpp -o tournament-fixing-algorithm.o

.cpp.o:
	@$(CXX) $(CFLAGS) -c $< -o $@

test : libgtest.a $(OBJECTS)
	@$(CXX) $(CFLAGS) $(TEST_SYSTEM_FLAGS) -pthread tests/test_main.cpp $(TESTS) $(OBJECTS) libgtest.a -o test.o

libgtest.a: gmock-all.o gtest-all.o
	@ar -rv libgtest.a gmock-all.o gtest-all.o 
	
gmock-all.o:
	@$(CXX) $(TEST_SYSTEM_FLAGS) $(TEST_I_FLAGS) -c gmock-1.7.0/src/gmock-all.cc

gtest-all.o:
	@$(CXX) $(TEST_SYSTEM_FLAGS) $(TEST_I_FLAGS) -c gtest-1.7.0/src/gtest-all.cc

clean:
	@-rm tournament-fixing-algorithm.o 2>/dev/null || true
	@-rm test.o 2>/dev/null || true
	@-rm gmock-all.o 2>/dev/null || true
	@-rm gtest-all.o 2>/dev/null || true
	@-rm libgtest.a 2>/dev/null || true
	@-rm $(OBJECTS) 2>/dev/null || true
	@-rm $(TEST_OBJECTS) 2>/dev/null || true
