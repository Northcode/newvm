// data port
// construct for connecting io of a device to the bus
struct data_port
{
  virtual void  send      (byte  value) {}
  virtual void  send      (dword value) {}
  virtual void  send      (int   value) {}

  virtual byte  get_byte  () {}
  virtual dword get_dword () {}
  virtual int   get_int   () {}
};

// data bus
// device used to communicate between devices
struct bus : public device
{
  vector<shared_ptr<data_port>> ports;

  bus();
  
  void add_port(byte port_num, shared_ptr<data_port> port);

  void send_byte  (byte, byte);
  void send_dword (byte, dword);
  void send_int   (byte, int);

  byte  get_byte  (byte);
  dword get_dword (byte);
  int   get_int   (byte);

  bool check_port(byte);
};

bus::bus() {
  ports = vector<shared_ptr<data_port>>(BUS_SIZE);
}

void bus::add_port(byte port_num, shared_ptr<data_port> port) {
  ports[port_num] = port;
}

bool bus::check_port(byte port) {
  return port < BUS_SIZE && ports[port] != NULL;
}

void bus::send_byte (byte port, byte data) {
  if(check_port(port))
    ports[port]->send(data);
}

void bus::send_dword (byte port, dword data) {
  if (check_port(port))
    ports[port]->send(data);
}

void bus::send_int (byte port, int data) {
  if (check_port(port))
    ports[port]->send(data);
}
