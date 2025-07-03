#include <memory>
#include <iostream>
#include "device_driver.h"
#include "exception.h"

using std::cout;

class Application {
 public:
  Application(DeviceDriver *device) : device{device} {}
  void readAndPrint(long startAdd, long endAdd) {
    for (long addr = startAdd; addr <= endAdd; addr++) {
      try {
        int data = device->read(addr);
        cout << " Read [0x" << std::hex << addr << "] = " << data << "\n ";
      } catch (const ReadFailException& e) {
        cout << "Read failed at address 0x" << std::hex << addr << ": "
             << e.what() << "\n";
      }
    }
  }

  void writeAll(int value) {
    for (long addr = 0x00; addr <= 0x04; addr++) {
      try {
        device->write(addr, value);
      } catch (const WriteFailException& e) {
        cout << "Write failed at address 0x" << std::hex << addr << ": "
             << e.what() << "\n";
      }
    }
  }

 private:
  DeviceDriver *device;
};