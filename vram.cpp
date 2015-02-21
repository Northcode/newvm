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
