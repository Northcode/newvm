// data port
// construct for connecting io of a device to the bus
struct data_port
{
  virtual void  send      (byte  value) {}
  virtual void  send      (dword value) {}
  virtual void  send      (int   value) {}

  virtual byte  get_byte  () {return 0;}
  virtual dword get_dword () {return 0;}
  virtual int   get_int   () {return 0;}
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

byte bus::get_byte (byte port) {
  if(check_port(port))
    return ports[port]->get_byte();
  return 0;
}

dword bus::get_dword (byte port) {
  if(check_port(port))
    return ports[port]->get_dword();
  return 0;
}

int bus::get_int (byte port) {
  if(check_port(port))
    return ports[port]->get_int();
  return 0;
}


