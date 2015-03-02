struct text_io_port : public data_port
{
  void  send      (byte  value); 
  void  send      (dword value); 
  void  send      (int   value); 
  
  byte  get_byte  (); 
  dword get_dword (); 
  int   get_int   (); 
};

void text_io_port::send(byte value) {
  cout << (char)value;
}

void text_io_port::send(dword value) {
  cout << (int)value;
}

void text_io_port::send(int value) {
  cout << value;
}

byte text_io_port::get_byte() {
  byte b;
  cin >> b;
  return b;
}

dword text_io_port::get_dword() {
  dword d;
  cin >> d;
  return d;
}

int text_io_port::get_int() {
  int i;
  cin >> i;
  return i;
}
