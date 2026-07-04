.PHONY: setup build run debug clean

# Shortcuts
setup:
	cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug

build:
	cmake --build build

run: build
	./build/shell

debug: build 
	lldb ./build/shell

clean:
	rm -rf build compile_commands.json
