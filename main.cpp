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

  // ld b a0 10
  _vm.v_ram->write(0,2);
  reg_selector a0{};
  a0.reg = 0;
  a0.index = 0;
  _vm.v_ram->write(1,a0.raw);
  _vm.v_ram->write(2,10);
  
  //mv a0 a1
  _vm.v_ram->write(8,1);
  reg_selector a1{};
  a1.reg = 0;
  a1.index = 1;
  _vm.v_ram->write(9,a0.raw);
  _vm.v_ram->write(10,a1.raw);
  
  //cl a1
  _vm.v_ram->write(8*2,5);
  _vm.v_ram->write(8*2 + 1,a1.raw);

  //lddw d0 500
  _vm.v_ram->write(8*3,3);
  reg_selector d0{};
  d0.reg = 2;
  d0.index = 0;
  _vm.v_ram->write(8*3+1,d0.raw);
  _vm.v_ram->write_dword(8*3+2,(dword)500);

  // shutdown
  _vm.v_ram->write(8*4,0);

  _vm.power();

  cout << "register B0: " << (int)_vm.v_cpu->B[0] << endl;
  cout << "register B1: " << (int)_vm.v_cpu->B[1] << endl;
  cout << "register D0: " << (int)_vm.v_cpu->D[0] << endl;
}
