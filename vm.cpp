// vm
vm::vm() {
  devices = vector<shared_ptr<device>>();

  v_cpu = make_shared<cpu>  ();
  v_ram = make_shared<vram> ();

  v_cpu->memory = v_ram;
  v_cpu->machine = this;

  stack = make_shared<vstack>(v_cpu,v_ram);

  devices.push_back(v_cpu);
  devices.push_back(v_ram);
}

//loop through devices and tick each one
void vm::tick() {
  for(auto dev : devices) {
    dev->tick();
  }
}

//power on devices
void vm::power() {
  for(auto dev : devices) {
    dev->power();
  }

  run = true;

  //"clock"
  while (run) {
    tick(); 
  }
}

//reset everything
void vm::reset() {
  for (auto dev : devices) {
    dev->reset();
  }
}
