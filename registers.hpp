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
