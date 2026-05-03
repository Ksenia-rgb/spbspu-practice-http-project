CXX = g++

SRC = $(wildcard src/*.cpp src/repl/*.cpp src/http/*.cpp)
TEST_SRC = $(wildcard tests/*.cpp)
EXEC = http
TEST_EXEC = test_http

ARGS=

COMMON_FLAGS = -Wall -Wextra -std=c++14 -isystem libs -I./src/common -I./src/http -I./src/repl -lboost_filesystem
DEBUG_FLAGS = -Weffc++ -Wpedantic -Werror -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -Wnull-dereference -Wdouble-promotion -Wswitch-default -Wfloat-equal -Wcast-qual -Wcast-align -Wlogical-op -fsanitize=address,undefined -g -O0 

debug: out/debug/$(EXEC)
release: out/release/$(EXEC)
test: out/test/$(TEST_EXEC)

out/debug/$(EXEC): $(SRC) | out/debug
	$(CXX) $(SRC) $(COMMON_FLAGS) $(DEBUG_FLAGS) -o $@

out/release/$(EXEC): $(SRC) | out/release
	$(CXX) $(SRC) $(COMMON_FLAGS) -o $@

out/test/$(TEST_EXEC): $(TEST_SRC) | out/test
	$(CXX) $(filter-out src/main.cpp, $(SRC)) $(TEST_SRC) $(COMMON_FLAGS) -o $@ && ./$@

out/%:
	mkdir -p $@

run-debug: debug
	./out/debug/$(EXEC) $(ARGS)

run-release: release
	./out/release/$(EXEC) $(ARGS)

clean:
	rm -rf out

.PHONY: debug release run-debug run-release test clean
