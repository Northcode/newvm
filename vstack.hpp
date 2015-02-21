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
