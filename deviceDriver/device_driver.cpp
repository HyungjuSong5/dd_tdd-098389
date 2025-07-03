#include "device_driver.h"
#include "exception.h"
#include <iostream>

using std::cout;

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware)
    : m_hardware(hardware) {}

int DeviceDriver::read(long address) {
  unsigned char value = m_hardware->read(address);

  postConditionCheck(value, address);

  return static_cast<int>(value);
}
void DeviceDriver::postConditionCheck(unsigned char value, long address) {
  for (int i = 1; i < TestTryCount; ++i) {
    if (m_hardware->read(address) != value) {
      throw ReadFailException();
    }
  }
}
void DeviceDriver::write(long address, int data) {
  unsigned char value = m_hardware->read(address);
  if (value != emptyAddress) {
    throw WriteFailException();
  }
  m_hardware->write(address, (unsigned char)data);
}