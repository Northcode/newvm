#compile newvm source

all: main.cpp
	g++ -g -o main main.cpp --std=c++11

vm: main.cpp constants.hpp vm.hpp device.hpp registers.hpp cpu.hpp vram.hpp vstack.hpp paging.hpp bus.hpp vm.cpp cpu.cpp vram.cpp
	g++ -g -o newvm main.cpp --std=c++11

assembler: assembler.hpp registers.hpp constants.hpp
	g++ -g -o newasm assembler.cpp --std=c++11
