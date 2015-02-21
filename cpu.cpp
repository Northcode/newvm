// cpu
cpu::cpu() {
  B = vector<byte>(15);
  C = vector<int>(15);
  D = vector<dword>(15);
  F = vector<bool>(15);
}

void cpu::tick() {
  exec_instruction(D[REG_IP]);
  D[REG_IP] += 8;
}

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

void cpu::mv  (reg_selector src, reg_selector dest) {
  if(src.reg != dest.reg)
    return;
  switch (dest.reg) {
  case 0:
    B[dest.index] = B[src.index];
    break;
  case 1:
    C[dest.index] = C[src.index];
    break;
  case 2:
    D[dest.index] = D[src.index];
    break;
  case 3:
    F[dest.index] = F[src.index];
    break;
  }
}

void cpu::ld  (reg_selector dest, byte  value)      {
  if(dest.reg == 0)
    B[dest.reg] = value;
}

void cpu::ld  (reg_selector dest, dword value)      {
  if(dest.reg == 2)
    D[dest.reg] = value;
}

void cpu::ld  (reg_selector dest, int   value)      {
  if(dest.reg == 1)
    C[dest.reg] = value;
}

void cpu::cl  (reg_selector dest)                   {
  if      (dest.reg == 0)
    B[dest.index] = 0;
  else if (dest.reg == 1)
    C[dest.index] = 0;
  else if (dest.reg == 2)
    D[dest.index] = 0;
}

void cpu::jmp        (dword address)                      {
  D[REG_IP] = address;
}

void cpu::jmp        (reg_selector reg)                   {
  if(reg.reg == 2)
    D[REG_IP] = D[reg.index];
}

void cpu::call       (dword address)                      {
  machine->stack->push_dword(D[REG_IP]);
  jmp(address);
}

void cpu::call       (reg_selector reg)                   {
  if(reg.reg == 2)
    call(D[reg.index]);
}

void cpu::interupt   (byte index)                         {
  call(D[REG_IVT] + index * 4);
}

void cpu::ret        ()                                   {
  dword addr = machine->stack->pop_dword();
  jmp(addr);
}

void cpu::cjmp       (bool eq, bool gt, dword    address) {
  if(F[REG_EQ] == eq && F[REG_GT] == gt)
    jmp(address);
}

void cpu::cjmp       (bool eq, bool gt, reg_selector reg) {
  if(reg.reg == 2)
    cjmp(eq,gt,D[reg.index]);
}

void cpu::stack_push (reg_selector reg) {
  switch (reg.reg) {
  case 0:
    stack_push(B[reg.index]);
    break;
  case 1:
    stack_push(C[reg.index]);
    break;
  case 2:
    stack_push(D[reg.index]);
    break;
  }
}

void cpu::stack_push (byte value)       {
  machine->stack->push_byte(value);
}

void cpu::stack_push (dword value)      {
  machine->stack->push_dword(value);
}

void cpu::stack_push (int value)        {
  machine->stack->push_int(value);
}

void cpu::stack_pop_byte  (reg_selector reg) {
  B[reg.index] = machine->stack->pop_byte();
}

void cpu::stack_pop_dword (reg_selector reg) {
  D[reg.index] = machine->stack->pop_dword();
}

void cpu::stack_pop_int   (reg_selector reg) {
  C[reg.index] = machine->stack->pop_int();
}

void  cpu::out_byte  (byte port, byte value)       {
  
}

void  cpu::out_byte  (byte port, reg_selector reg) {}

void  cpu::out_dword (byte port, dword value)      {}

void  cpu::out_dword (byte port, reg_selector reg) {}

void  cpu::out_int   (byte port, int value)        {}

void  cpu::out_int   (byte port, reg_selector reg) {}

void  cpu::in_byte   (byte port, reg_selector reg) {  }
void  cpu::in_dword  (byte port, reg_selector reg) {  }
void  cpu::in_int    (byte port, reg_selector reg) {  }

void cpu::add (reg_selector src, reg_selector value) {
  if(src.reg != value.reg)
    return;
  switch (src.reg) {
  case 0:
    B[src.index] += B[value.index];
    break;
  case 1:
    C[src.index] += C[value.index];
    break;
  case 2:
    D[src.index] += D[value.index];
    break;
  }
}

void cpu::sub (reg_selector src, reg_selector value) {
  if(src.reg != value.reg)
    return;
  switch (src.reg) {
  case 0:
    B[src.index] -= B[value.index];
    break;
  case 1:
    C[src.index] -= C[value.index];
    break;
  case 2:
    D[src.index] -= D[value.index];
    break;
  }
}

void cpu::mul (reg_selector src, reg_selector value) {
  if(src.reg != value.reg)
    return;
  switch (src.reg) {
  case 0:
    B[src.index] *= B[value.index];
    break;
  case 1:
    C[src.index] *= C[value.index];
    break;
  case 2:
    D[src.index] *= D[value.index];
    break;
  }
}

void cpu::div (reg_selector src, reg_selector value) {
  if(src.reg != value.reg)
    return;
  switch (src.reg) {
  case 0:
    B[src.index] /= B[value.index];
    break;
  case 1:
    C[src.index] /= C[value.index];
    break;
  case 2:
    D[src.index] /= D[value.index];
    break;
  }
}

void cpu::mod (reg_selector src, reg_selector value) {
  if(src.reg != value.reg)
    return;
  switch (src.reg) {
  case 0:
    B[src.index] %= B[value.index];
    break;
  case 1:
    C[src.index] %= C[value.index];
    break;
  case 2:
    D[src.index] %= D[value.index];
    break;
  }
}


void cpu::cmp (reg_selector a, reg_selector b) {}

void cpu::mwb  (reg_selector reg, byte value)         {
  if(reg.reg == 2)
    memory->write(D[reg.index],value);
}

void cpu::mwb  (reg_selector reg, reg_selector value) {
  if(value.reg == 0)
    mwb(reg,B[value.index]);
}

void cpu::mwdw (reg_selector reg, dword value)        {
  if(reg.reg == 2)
    memory->write_dword(D[reg.index],value);
}

void cpu::mwdw (reg_selector reg, reg_selector value) {
  if(value.reg == 2)
    mwdw(reg,D[value.index]);
}

void cpu::mwi  (reg_selector reg, int value)          {
  if(reg.reg == 2)
    memory->write_int(D[reg.index],value);
}

void cpu::mwi  (reg_selector reg, reg_selector value) {
  if(value.reg == 1)
    mwdw(reg,C[value.index]);  
}

void cpu::mrb  (reg_selector src, reg_selector dst)   {
  if(dst.reg != 0)
    return;
  B[dst.index] = memory->read(D[src.index]);
}

void cpu::mrdw (reg_selector src, reg_selector dst)   {
  if(dst.reg != 2)
    return;
  D[dst.index] = memory->read_dword(D[src.index]);
}

void cpu::mri  (reg_selector src, reg_selector dst)   {
  if(dst.reg != 1)
    return;
  C[dst.index] = memory->read_int(D[src.index]);
}
