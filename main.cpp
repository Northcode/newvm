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

// constants are fancy

// indexes for registers
constexpr byte REG_IP = 1; // instruction pointer
constexpr byte REG_SP = 2; // stack pointer
constexpr byte REG_BP = 3; // stack base pointer
constexpr byte REG_PD = 4; // page directory

//flag registers
constexpr byte REG_AM = 1; // addressing mode

// sizes for stuff
constexpr dword FRAME_SIZE          = 4096; // 4 KiB vram frame size
constexpr dword PAGE_TABLE_SIZE     = 1024; // 1024 pages (4 KiB) fit in one page table
constexpr dword PAGE_DIRECTORY_SIZE = 1024; // 1024 page tables (4 KiB) fit in the page directory

constexpr dword MEM_SIZE = 16*1024;

// predef stuffs so I can "use" it in classes before actually defining anything
struct device;
class  vm;
class  cpu;
class  vram;


// vm class emulates the actual machine
// nvm can contain any number of devices, however the main ones are:
//  cpu (emulates cpu, duh. Contains register information and methods for executing instructions)
//  memory (emulates ram. Contains virtual memory used by program)
//  clock (handles device ticks and timing)
//  psu (handles power management, powering on and off)
//  bus (enables communication between all devices. Devices can setup ports to send and recieve data in serial format,
//       enables generic data transmission between program and machine as there are instructions for transfering different sized data to and from ports)
// extra devices are:
//  io devices (terminal io, virtual hard disk, image reader)
// 
// adding more devices is as simple as making the class/struct and adding it to the devices vector in the vm

class vm
{
  vector<shared_ptr<device>> devices;
  
public:
  
  shared_ptr<cpu>  v_cpu;
  shared_ptr<vram> v_ram;

  vm ();
  
  void tick  ();
  void power ();
  void reset ();
  
};



// In nvm, the vm is buildt from modules called devices
// Everything except the vm is represented as a device, which has 3 shared methods
// They all have pointers back to the parent machine class

struct device
{
  shared_ptr<vm> machine;

  virtual void tick  () {}
  virtual void power () {}
  virtual void reset () {}
};



// registers are saved in vectors based on size
// There are 3 base registers:
//   byte sized: B (unsigned 1 byte)
//   int sized: C (4 byte)
//   data/memory: D (unsigned 4 byte)
//   flags: F(bools, different flags used by the vm)
//
// asm indexes register needed (LD(B|C|D) [reg] [data])
// register assembly should be:
// LD, CL, MV
// is store, load, clear and move respectively

// maybe add commands for casting between sizes?

// helper struct for selecting registers
// reg is register type selector, 0 for B, 1 for C, etc...
// index is the index of the accessed register
union reg_selector {
  struct
  {
    byte reg:4;
    byte index:4;
  };
  byte raw;
};

union cmp_byte
{
  struct {
    bool eq:1;
    bool gt:1;
  };
  byte raw;
};

//cpu class, emulates cpu. Acts as device

struct cpu : public device
{
  // registers
  vector<byte> B{0,0,0,0};
  vector<int> C{0,0,0,0};
  vector<dword> D{0,0,0,0};
  vector<bool> F{false,false,false,false};

  shared_ptr<vram> memory;

  cpu();

  void tick  ();
  void power ();
  void reset ();

  void exec_instruction (dword address);

  // instruction functions (these basically map to assembly, hey easy for me to write asm then!)

  // register instructions
  void mv  (reg_selector src, reg_selector dest);
  void ld  (reg_selector dest, byte  value);
  void ld  (reg_selector dest, dword value);
  void ld  (reg_selector dest, int   value);
  void cl  (reg_selector dest);

  // jumping
  void jmp        (dword address);
  void jmp        (reg_selector reg);
  void call       (dword address);
  void call       (reg_selector reg);
  void interupt   (byte index);
  void ret        ();
  void cjmp       (bool eq, bool gt, dword    address);
  void cjmp       (bool eq, bool gt, reg_selector reg);

  // stack stuff
  void stack_push (reg_selector reg);
  void stack_push (byte value);
  void stack_push (dword value);
  void stack_push (int value);

  byte  stack_pop_byte  (reg_selector reg);
  dword stack_pop_dword (reg_selector reg);
  int   stack_pop_int   (reg_selector reg);

  // bus IO
  void  out_byte  (byte port, byte value);
  void  out_byte  (byte port, reg_selector reg);
  void  out_dword (byte port, dword value);
  void  out_dword (byte port, reg_selector reg);
  void  out_int   (byte port, int value);
  void  out_int   (byte port, reg_selector reg);

  byte  in_byte   (byte port, reg_selector reg);
  dword in_dword  (byte port, reg_selector reg);
  int   in_int    (byte port, reg_selector reg);

  // math stuffs
  void add (reg_selector src, reg_selector value);
  void sub (reg_selector src, reg_selector value);
  void mul (reg_selector src, reg_selector value);
  void div (reg_selector src, reg_selector value);
  void mod (reg_selector src, reg_selector value);
  
  // comparison
  void cmp (reg_selector a, reg_selector b);  

  //memory io
  void mwb  (reg_selector reg, byte value);
  void mwb  (reg_selector reg, reg_selector value);
  void mwdw (reg_selector reg, dword value);
  void mwdw (reg_selector reg, reg_selector value);
  void mwi  (reg_selector reg, int value);
  void mwi  (reg_selector reg, reg_selector value);

  void mrb  (reg_selector src, reg_selector dst);
  void mrdw (reg_selector src, reg_selector dst);
  void mri  (reg_selector src, reg_selector dst);
};


// virtual memory class
// contains all virtual ram used by vm, used as storage outside registers
// contains methods for reading and writing basic sized units

class vram : public device
{
  vector<byte> data;

public:

  vram();
  
  void  write       (dword address,byte stuff);
  byte  read        (dword address);

  void  write_dword (dword address, dword stuff);
  dword read_dword  (dword address);

  void  write_int   (dword address, int stuff);
  int   read_int    (dword address);

};


// virtual stack helper class
// provides methods for stack operations on memory

struct vstack
{
  shared_ptr<cpu> _cpu; //pointer to cpu for register access
  shared_ptr<vram> _ram; //pointer to ram for memory access

  vstack (shared_ptr<cpu> _CPU, shared_ptr<vram> _RAM);

  void  push_byte  (byte value);
  void  push_dword (dword value);
  void  push_int   (int value);

  byte  pop_byte   ();
  dword pop_dword  ();
  int   pop_int    ();
};


// Paging stuffs!
//
// unions for making paging ALOT easier!
// splitting dwords has never been easier! 

union page_table 
{
  struct {
    bool  present : 1; // is page table present? 
    bool  mode    : 1; // true for user mode, false for kernel mode
    bool  write   : 1; // true for write enabled, false for readonly
    dword address :29; // index of page table, 4 KiB multiples (so if this value is 1, the address is: 1 * 4 KiB = 4096)
  };
  dword raw;
};

union page_entry
{
  struct {
    bool  present : 1; // is page present?
    bool  mode    : 1; // true for user mode, false for kernel mode
    bool  write   : 1; // true for write enabled, false for readonly
    dword address :29; // 4 KiB index of page address
  };
  dword raw;
};

union page_address
{
  struct {
    dword page_table_index : 10;
    dword page_entry_index : 10;
    dword address_offset   : 12;
  };
  dword raw;
};

// helper class for paging
// helps deal with paging in oses
struct pager
{
  shared_ptr<cpu>  _cpu;
  shared_ptr<vram> _ram;
  
  pager (shared_ptr<cpu> _CPU, shared_ptr<vram> _RAM);

  dword translate_address(dword address);

  dword create_page_table();

};


// data port
// construct for connecting io of a device to the bus
struct data_port
{
  virtual void  send      (byte  value);
  virtual void  send      (dword value);
  virtual void  send      (int value);

  virtual byte  get_byte  ();
  virtual dword get_dword (); 
  virtual int   get_int   ();
};

// data bus
// device used to communicate between devices
struct bus : public device
{
  vector<shared_ptr<data_port>> ports;
  
  void add_port(byte port_num, shared_ptr<data_port> port);

  void send_byte  (byte port, byte  data);
  void send_dword (byte port, dword data);
  void send_int   (byte port, int   data);

  byte  get_byte  (byte port);
  dword get_dword (byte port);
  int   get_int   (byte port);
};


// --- implement functions

// vm
vm::vm() {
  devices = vector<shared_ptr<device>>();

  v_cpu = make_shared<cpu>  ();
  v_ram = make_shared<vram> ();

  v_cpu->memory = v_ram;

  devices.push_back(v_cpu);
  devices.push_back(v_ram);
}

//loop through devices and tick each one
void vm::tick() {
  for(auto dev : devices) {
    dev->tick();
  }
}

//power on devices
void vm::power() {
  for(auto dev : devices) {
    dev->power();
  }
}

//reset everything
void vm::reset() {
  for (auto dev : devices) {
    dev->reset();
  }
}

// cpu
cpu::cpu() {
  
}

void cpu::tick() {}

void cpu::power() {}

void cpu::reset() {}

void cpu::exec_instruction(dword address) {
  byte opcode = memory->read(address);
  switch (opcode) {
  case 1:
    {
      reg_selector regsrc;
      regsrc.raw = memory->read(address + 1);
      reg_selector regdst;
      regdst.raw = memory->read(address + 2);
      mv(regsrc,regdst);
    } break;
  case 2:
    {
      reg_selector regdst;
      regdst.raw = memory->read(address + 1);
      byte value = memory->read(address + 2);
      ld(regdst,value);
    } break;
  case 3:
    {
      reg_selector regdst;
      regdst.raw = memory->read(address + 1);
      dword value = memory->read_dword(address + 2);
      ld(regdst,value);
    } break;
  case 4:
    {
      reg_selector regdst;
      regdst.raw = memory->read(address + 1);
      int value = memory->read_int(address + 2);
      ld(regdst,value);
    } break; 
  case 5:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      cl(reg);
    } break;
  case 6:
    {
      dword addr = memory->read_dword(address + 1);
      jmp(addr);
    } break;
  case 7:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      jmp(reg);
    } break;
  case 8:
    {
      dword addr = memory->read_dword(address + 1);
      call(addr);
    } break;
  case 9:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      call(reg);
    } break;
  case 10:
    {
      byte index = memory->read(address + 1);
      interupt(index);
    } break;
  case 35:
    {
      ret();
    } break;
  case 11:
    {
      cmp_byte byt;
      byt.raw = memory->read(address+1);
      dword addr = memory->read_dword(address+2);
      cjmp(byt.eq,byt.gt,addr);
    } break;
  case 12:
    {
      cmp_byte byt;
      byt.raw = memory->read(address+1);
      reg_selector reg;
      reg.raw = memory->read(address+2);
      cjmp(byt.eq,byt.gt,reg);
    } break;
  case 13:
    {
      reg_selector reg;
      reg.raw = memory->read(address+1);
      stack_push(reg);
    } break;
  case 14:
    {
      byte value = memory->read(address+1);
      stack_push(value);
    } break;
  case 15:
    {
      dword value = memory->read_dword(address + 1);
      stack_push(value);
    } break;
  case 16:
    {
      int value = memory->read_int(address + 1);
      stack_push(value);
    } break;
  case 17:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      stack_pop_byte(reg);
    } break;
  case 18:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      stack_pop_dword(reg);
    } break;
  case 19:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      stack_pop_int(reg);
    } break;
  case 20:
    {
      byte port = memory->read(address + 1);
      byte data = memory->read(address + 2);
      out_byte(port,data);
    } break;
  case 21:
    {
      byte port = memory->read(address + 1);
      reg_selector reg;
      reg.raw = memory->read(address + 2);
      out_byte(port,reg);
    } break;
  case 22:
    {
      byte port = memory->read(address + 1);
      dword data = memory->read_dword(address + 2);
      out_dword(port,data);
    } break;
  case 23:
    {
      byte port = memory->read(address + 1);
      reg_selector reg;
      reg.raw = memory->read(address + 2);
      out_dword(port,reg);
    } break;
  case 24:
    {
      byte port = memory->read(address + 1);
      int data = memory->read_int(address + 2);
      out_int(port,data);
    } break;
  case 25:
    {
      byte port = memory->read(address + 1);
      reg_selector reg;
      reg.raw = memory->read(address + 2);
      out_int(port,reg);
    } break;
  case 26:
    {
      byte port = memory->read(address + 1);
      reg_selector reg;
      reg.raw = memory->read(address + 2);
      in_byte(port,reg);
    } break;
  case 27:
    {
      byte port = memory->read(address + 1);
      reg_selector reg;
      reg.raw = memory->read(address + 2);
      in_dword(port,reg);
    } break;
  case 28:
    {
      byte port = memory->read(address + 1);
      reg_selector reg;
      reg.raw = memory->read(address + 2);
      in_int(port,reg);
    } break;
  case 29:
    {
      reg_selector rega;
      reg_selector regb;
      rega.raw = memory->read(address + 1);
      regb.raw = memory->read(address + 2);
      add(rega,regb);
    } break;
  case 30:
    {
      reg_selector rega;
      reg_selector regb;
      rega.raw = memory->read(address + 1);
      regb.raw = memory->read(address + 2);
      sub(rega,regb);
    } break;
  case 31:
    {
      reg_selector rega;
      reg_selector regb;
      rega.raw = memory->read(address + 1);
      regb.raw = memory->read(address + 2);
      mul(rega,regb);
    } break;
  case 32:
    {
      reg_selector rega;
      reg_selector regb;
      rega.raw = memory->read(address + 1);
      regb.raw = memory->read(address + 2);
      div(rega,regb);
    } break;
  case 33:
    {
      reg_selector rega;
      reg_selector regb;
      rega.raw = memory->read(address + 1);
      regb.raw = memory->read(address + 2);
      mod(rega,regb);
    } break;
  case 34:
    {
      reg_selector rega;
      reg_selector regb;
      rega.raw = memory->read(address + 1);
      regb.raw = memory->read(address + 2);
      cmp(rega,regb);
    } break;
  case 36:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      byte value = memory->read(address + 2);
      mwb(reg,value);
    } break;
  case 37:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      reg_selector src;
      src.raw = memory->read(address + 2);
      mwb(reg,src);
    } break;
  case 38:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      dword value = memory->read(address + 2);
      mwdw(reg,value);
    } break;
  case 39:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      reg_selector src;
      src.raw = memory->read(address + 2);
      mwdw(reg,src);
    } break;
  case 40:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      int value = memory->read(address + 2);
      mwi(reg,value);
    } break;
  case 41:
    {
      reg_selector reg;
      reg.raw = memory->read(address + 1);
      reg_selector src;
      src.raw = memory->read(address + 2);
      mwi(reg,src);
    } break;
  case 42:
    {
      reg_selector src;
      reg_selector dst;
      src.raw = memory->read(address + 1);
      dst.raw = memory->read(address + 2);
      mrb(src,dst);
    } break;
  case 43:
    {
      reg_selector src;
      reg_selector dst;
      src.raw = memory->read(address + 1);
      dst.raw = memory->read(address + 2);
      mrdw(src,dst);
    } break;
  case 44:
    {
      reg_selector src;
      reg_selector dst;
      src.raw = memory->read(address + 1);
      dst.raw = memory->read(address + 2);
      mri(src,dst);
    } break;
  }
}

void cpu::mv  (reg_selector src, reg_selector dest) {}
void cpu::ld  (reg_selector dest, byte  value)      {}
void cpu::ld  (reg_selector dest, dword value)      {}
void cpu::ld  (reg_selector dest, int   value)      {}
void cpu::cl  (reg_selector dest)                   {}

void cpu::jmp        (dword address)                      {}
void cpu::jmp        (reg_selector reg)                   {}
void cpu::call       (dword address)                      {}
void cpu::call       (reg_selector reg)                   {}
void cpu::interupt   (byte index)                         {}
void cpu::ret        ()                                   {}
void cpu::cjmp       (bool eq, bool gt, dword    address) {}
void cpu::cjmp       (bool eq, bool gt, reg_selector reg) {}

void cpu::stack_push (reg_selector reg) {}
void cpu::stack_push (byte value)       {}
void cpu::stack_push (dword value)      {}
void cpu::stack_push (int value)        {}

byte  cpu::stack_pop_byte  (reg_selector reg) { return 0; }
dword cpu::stack_pop_dword (reg_selector reg) { return 0; }
int   cpu::stack_pop_int   (reg_selector reg) { return 0; }

void  cpu::out_byte  (byte port, byte value)       {}
void  cpu::out_byte  (byte port, reg_selector reg) {}
void  cpu::out_dword (byte port, dword value)      {}
void  cpu::out_dword (byte port, reg_selector reg) {}
void  cpu::out_int   (byte port, int value)        {}
void  cpu::out_int   (byte port, reg_selector reg) {}

byte  cpu::in_byte   (byte port, reg_selector reg) { return 0; }
dword cpu::in_dword  (byte port, reg_selector reg) { return 0; }
int   cpu::in_int    (byte port, reg_selector reg) { return 0; }

void cpu::add (reg_selector src, reg_selector value) {}
void cpu::sub (reg_selector src, reg_selector value) {}
void cpu::mul (reg_selector src, reg_selector value) {}
void cpu::div (reg_selector src, reg_selector value) {}
void cpu::mod (reg_selector src, reg_selector value) {}

void cpu::cmp (reg_selector a, reg_selector b) {}

void cpu::mwb  (reg_selector reg, byte value)         {}
void cpu::mwb  (reg_selector reg, reg_selector value) {}
void cpu::mwdw (reg_selector reg, dword value)        {}
void cpu::mwdw (reg_selector reg, reg_selector value) {}
void cpu::mwi  (reg_selector reg, int value)          {}
void cpu::mwi  (reg_selector reg, reg_selector value) {}

void cpu::mrb  (reg_selector src, reg_selector dst)   {}
void cpu::mrdw (reg_selector src, reg_selector dst)   {}
void cpu::mri  (reg_selector src, reg_selector dst)   {}


// vram
vram::vram() {
  this->data = vector<byte>(MEM_SIZE);
}

void  vram::write       (dword address,byte value)   {
  if(address < data.size())
    this->data[address] = value;
}

byte  vram::read        (dword address)              { 
  if(address > data.size())
    return 0;
  return data[address];
}

void  vram::write_dword (dword address, dword value) {
  if(address + 4 > data.size())
    return;
  this->data[address + 3] = value >> 24 & 0x000000ff;
  this->data[address + 2] = value >> 16 & 0x000000ff;
  this->data[address + 1] = value >> 8  & 0x000000ff;
  this->data[address + 0] = value       & 0x000000ff;
}

dword vram::read_dword  (dword address)              { 
   if(address + 4 > data.size())
    return 0;
   dword res = (data[address + 3] << 24 | data[address + 2] << 16 | data[address + 1] << 8 | data[address]);
   return res;
}

void  vram::write_int   (dword address, int value)   {
  this->write_dword(address,value);
}
int   vram::read_int    (dword address)              {
  return this->read_dword(address);
}


int main() {
  vm _vm{};
  cout << "test" << endl;
}
