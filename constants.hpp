// typedef these things because we gonna need them for shorthand, also I like these names
typedef char byte;
typedef unsigned int dword;

// constants are fancy

// indexes for registers
constexpr byte REG_IP  = 1; // instruction pointer
constexpr byte REG_SP  = 2; // stack pointer
constexpr byte REG_BP  = 3; // stack base pointer
constexpr byte REG_PD  = 4; // page directory
constexpr byte REG_IVT = 5;

//flag registers
constexpr byte REG_EQ = 2;
constexpr byte REG_GT = 3;

// sizes for stuff
constexpr dword FRAME_SIZE          = 4096; // 4 KiB vram frame size
constexpr dword PAGE_TABLE_SIZE     = 1024; // 1024 pages (4 KiB) fit in one page table
constexpr dword PAGE_DIRECTORY_SIZE = 1024; // 1024 page tables (4 KiB) fit in the page directory

constexpr dword MEM_SIZE = 16*1024;
constexpr dword BUS_SIZE = 256;
 
//structures

union cmp_byte
{
  struct {
    bool eq :1; // value to compare eq flag against
    bool jeq:1; // wether to compare eq flag
    bool gt :1; // value to compare gt flag against
    bool jgt:1; // wether to compare gt flag
    byte pad:4; // padding
  };
  byte raw;
};

union addressing_mode
{
  struct {
    byte Dindex:4;
    bool negate:1;
    byte padding:3;
  };
  byte raw;
};

vector<byte> dword_to_bytes(dword value) {
  //cout << "decoding dword: " << value << endl;
  vector<byte> result = vector<byte>(4);
  result[3] = value >> 24 & 0x000000ff;
  result[2] = value >> 16 & 0x000000ff;
  result[1] = value >> 8  & 0x000000ff;
  result[0] = value       & 0x000000ff;
  return result;
}
