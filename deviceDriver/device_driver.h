#pragma once
#include "flash_memory_device.h"

class DeviceDriver {
 public:
  DeviceDriver(FlashMemoryDevice *hardware);
  int read(long address);
  void postConditionCheck(unsigned char value, long address);
  void write(long address, int data);

 protected:
  FlashMemoryDevice *m_hardware;
  const int TestTryCount = 5;
  const int emptyAddress = 0xFF;
};