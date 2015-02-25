#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <iterator>

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

// --- extra devices



int main(int argc, char* argv[]) {
  string file{"program.dat"};
  if(argc > 1)
    file = string(argv[1]);

  vm _vm{};
  
  ifstream inputfile{file,ios::binary};

  auto buf = vector<char>(istreambuf_iterator<char>(inputfile),istreambuf_iterator<char>());

  for (int i = 0; i < buf.size(); i++) {
    cout << (int)buf[i] << " ";
    _vm.v_ram->write(i,buf[i]);
  }

  cout << endl;

  _vm.power();

  cout << "register B: ";
  for (auto r : _vm.v_cpu->B)
    cout << (int)r << ", ";
  cout << endl << "register D: ";
  for (auto d : _vm.v_cpu->D)
    cout << (int)d << ", ";
  cout << endl;
}
