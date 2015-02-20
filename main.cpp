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

//flag registers
constexpr byte REG_AM = 1; // addressing mode

// sizes for stuff
constexpr dword FRAME_SIZE          = 4096; // 4 KiB vram frame size
constexpr dword PAGE_TABLE_SIZE     = 1024; // 1024 pages (4 KiB) fit in one page table
constexpr dword PAGE_DIRECTORY_SIZE = 1024; // 1024 page tables (4 KiB) fit in the page directory

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

  virtual void tick  ();
  virtual void power ();
  virtual void reset ();
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
struct reg_selector
{
  byte reg,index;
};


//cpu class, emulates cpu. Acts as device

struct cpu : public device
{
  // registers
  vector<byte> B;
  vector<int> C;
  vector<dword> D;
  vector<bool> F;

  void tick  ();
  void power ();
  void reset ();

  void exec_instruction (dword address);

  // instruction functions (these basically map to assembly, hey easy for me to write asm then!)

  // register instructions
  void mov (reg_selector src, reg_selector dest);
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

};


// virtual memory class
// contains all virtual ram used by vm, used as storage outside registers
// contains methods for reading and writing basic sized units

class vram : public device
{
  vector<byte> data;

public:
  
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




int main() {
  cout << "hello world!" << endl;
}
