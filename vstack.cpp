//vstack
vstack::vstack (shared_ptr<cpu> _CPU, shared_ptr<vram> _RAM) {
  _cpu = _CPU;
  _ram = _RAM;
}

void vstack::push_byte  (byte value) {
  _ram->write(_cpu->D[REG_SP] , value);
  _cpu->D[REG_SP]++;
}

void vstack::push_dword (dword value) {
  _ram->write(_cpu->D[REG_SP] , value);
  _cpu->D[REG_SP] += 4;
}

void vstack::push_int   (int value) {
  _ram->write(_cpu->D[REG_SP] , value);
  _cpu->D[REG_SP] += 4;
}

byte vstack::pop_byte() {
  _cpu->D[REG_SP]--;
  return _ram->read(_cpu->D[REG_SP]);
}

dword vstack::pop_dword() {
  _cpu->D[REG_SP] -= 4;
  return _ram->read_dword(_cpu->D[REG_SP]);
}

int vstack::pop_int() {
  _cpu->D[REG_SP] -= 4;
  return _ram->read_int(_cpu->D[REG_SP]);
}
