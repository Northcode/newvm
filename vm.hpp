
// vm class emulates the actual machine
// nvm can contain any number of devices, however the main ones are:
//  cpu (emulates cpu, duh. Contains register information and methods for executing instructions)
//  memory (emulates ram. Contains virtual memory used by program)
//  clock (handles device ticks and timing)
//  psu (handles power management, powering on and off)
//  bus (enables communication between all devices. Devices can setup ports to send and recieve data in serial format,
//       enables generic data transmission between program and machine as there are instructions for transfering different sized data to and from ports)
// extra devices are:
//  io devices (terminal io, virtual hard disk, image reader)
// 
// adding more devices is as simple as making the class/struct and adding it to the devices vector in the vm

class vm
{
  vector<shared_ptr<device>> devices;
  
public:
  
  shared_ptr<cpu>  v_cpu;
  shared_ptr<vram> v_ram;

  shared_ptr<vstack> stack;

  vm ();
  
  void tick  ();
  void power ();
  void reset ();
  
};
