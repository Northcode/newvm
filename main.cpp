#include <iostream>
#include <vector>
#include <memory>

// typedef these things because we gonna need them for shorthand, also I like these names
typedef unsigned char byte;
typedef unsigned int dword;

// using namespace because im lazy
// will fix this when vm works
// refactoring will be lulz :|
using namespace std;

#include "constants.hpp"

// predef stuffs so I can "use" it in classes before actually defining anything
struct device;
class  vm;
struct cpu;
class  vram;
struct vstack;

#include "vm.hpp"

#include "device.hpp"

#include "registers.hpp"

#include "cpu.hpp"

#include "vram.hpp"

#include "vstack.hpp"

#include "paging.hpp"

#include "bus.hpp"

// --- implement functions

#include "vm.cpp"

#include "cpu.cpp"

#include "vram.cpp"

#include "vstack.cpp"

int main() {
  vm _vm{};
  cout << "test" << endl;
}
