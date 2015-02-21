// In nvm, the vm is buildt from modules called devices
// Everything except the vm is represented as a device, which has 3 shared methods
// They all have pointers back to the parent machine class

struct device
{
  vm* machine;

  virtual void tick  () {}
  virtual void power () {}
  virtual void reset () {}
};
