// data port
// construct for connecting io of a device to the bus
struct data_port
{
  virtual void  send      (byte  value);
  virtual void  send      (dword value);
  virtual void  send      (int value);

  virtual byte  get_byte  ();
  virtual dword get_dword (); 
  virtual int   get_int   ();
};

// data bus
// device used to communicate between devices
struct bus : public device
{
  vector<shared_ptr<data_port>> ports;
  
  void add_port(byte port_num, shared_ptr<data_port> port);

  void send_byte  (byte port, byte  data);
  void send_dword (byte port, dword data);
  void send_int   (byte port, int   data);

  byte  get_byte  (byte port);
  dword get_dword (byte port);
  int   get_int   (byte port);
};
