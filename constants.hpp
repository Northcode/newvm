// constants are fancy

// indexes for registers
constexpr byte REG_IP = 1; // instruction pointer
constexpr byte REG_SP = 2; // stack pointer
constexpr byte REG_BP = 3; // stack base pointer
constexpr byte REG_PD = 4; // page directory
constexpr byte REG_IVT = 5;

//flag registers
constexpr byte REG_AM = 1; // addressing mode
constexpr byte REG_EQ = 2;
constexpr byte REG_GT = 3;

// sizes for stuff
constexpr dword FRAME_SIZE          = 4096; // 4 KiB vram frame size
constexpr dword PAGE_TABLE_SIZE     = 1024; // 1024 pages (4 KiB) fit in one page table
constexpr dword PAGE_DIRECTORY_SIZE = 1024; // 1024 page tables (4 KiB) fit in the page directory

constexpr dword MEM_SIZE = 16*1024;
