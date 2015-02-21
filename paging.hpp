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
