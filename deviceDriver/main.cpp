#include "device_driver.h"
#include "gmock/gmock.h"

TEST(DeviceDriver, ReadFromHW) {
  // TODO : replace hardware with a Test Double
  FlashMemoryDevice* hardware = nullptr;
  DeviceDriver driver{hardware};
  int data = driver.read(0xFF);
  EXPECT_EQ(0, data);
}

int main() {
  ::testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}