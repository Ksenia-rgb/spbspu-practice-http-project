CXX = g++

SRC = $(wildcard src/*.cpp src/repl/*.cpp)
EXEC = http

COMMON_FLAGS = -Wall -Wextra -std=c++14 -I./libs -I./src/common -I./src/http
DEBUG_FLAGS = -Weffc++ -Wpedantic -Werror -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor -Wformat=2 -Wnull-dereference -Wdouble-promotion -Wswitch-default -Wfloat-equal -Wcast-qual -Wcast-align -Wlogical-op -fsanitize=address,undefined -g -O0 

debug: out/debug/$(EXEC)
release: out/release/$(EXEC)

out/debug/$(EXEC): $(SRC) | out/debug
	$(CXX) $(SRC) $(COMMON_FLAGS) $(DEBUG_FLAGS) -o $@

out/release/$(EXEC): $(SRC) | out/release
	$(CXX) $(SRC) $(COMMON_FLAGS) -o $@

out/%:
	mkdir -p $@

run-debug: debug
	./out/debug/$(EXEC)

run-release: release
	./out/release/$(EXEC)

clean:
	rm -rf out

.PHONY: debug release run-debug run-release clean
