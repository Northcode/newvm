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
  vector<byte> B;
  vector<int> C;
  vector<dword> D;
  vector<bool> F;

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

  void  stack_pop_byte  (reg_selector reg);
  void  stack_pop_dword (reg_selector reg);
  void  stack_pop_int   (reg_selector reg);

  // bus IO
  void  out_byte  (byte port, byte value);
  void  out_byte  (byte port, reg_selector reg);
  void  out_dword (byte port, dword value);
  void  out_dword (byte port, reg_selector reg);
  void  out_int   (byte port, int value);
  void  out_int   (byte port, reg_selector reg);

  void  in_byte   (byte port, reg_selector reg);
  void  in_dword  (byte port, reg_selector reg);
  void  in_int    (byte port, reg_selector reg);

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
