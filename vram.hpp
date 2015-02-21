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
