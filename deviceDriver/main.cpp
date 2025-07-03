#include "Application.cpp"
#include "device_driver.h"
#include "exception.h"
#include "gmock/gmock.h"

using namespace testing;
class MockFlashMemoryDevice : public FlashMemoryDevice {
 public:
  MOCK_METHOD(unsigned char, read, (long), (override));
  MOCK_METHOD(void, write, (long, unsigned char), (override));
};

class DeviceDriverFixture : public Test {
 public:
  void EXPECTED_DRIVER_READ(long address, int expected) {
    int result = driver.read(address);
    EXPECT_EQ(result, expected);
  }
  NiceMock<MockFlashMemoryDevice> mock;
  DeviceDriver driver{&mock};
  Application app{&driver};
  long address = 0x200;
  int expectedData = 0x46;
};

TEST_F(DeviceDriverFixture, READ_5TIMES) {
  long address = 0xF2;
  int expected = 0xAB;
  EXPECT_CALL(mock, read(address)).Times(5).WillRepeatedly(Return(expected));

  EXPECTED_DRIVER_READ(address, expected);
}

TEST_F(DeviceDriverFixture, READ_5TIMES_INCORRECT) {
  EXPECT_CALL(mock, read(address))
      .WillOnce(Return(0x11))
      .WillOnce(Return(0x11))
      .WillOnce(Return(0x11))
      .WillOnce(Return(0x11))
      .WillOnce(Return(0x22));  // failed

  EXPECT_THROW(driver.read(address), ReadFailException);
}

TEST_F(DeviceDriverFixture, WRITE_FAILED_ALREADY_WRITEN) {
  EXPECT_CALL(mock, read(address)).WillOnce(Return(0x11));
  EXPECT_CALL(mock, write(address, expectedData)).Times(0);

  EXPECT_THROW(driver.write(address, expectedData), WriteFailException);
}

TEST_F(DeviceDriverFixture, WRITE_PASS) {
  EXPECT_CALL(mock, read(address)).WillOnce(Return(0xFF));
  EXPECT_CALL(mock, write(address, expectedData));

  EXPECT_NO_THROW(driver.write(address, expectedData));
}

TEST_F(DeviceDriverFixture, APP_READ_TEST) {
  int testValue = 0xF2;
  EXPECT_CALL(mock, read(_)).Times(25).WillRepeatedly(Return(testValue));

  app.readAndPrint(0x00, 0x04);
}

TEST_F(DeviceDriverFixture, APP_WRITE_TEST) {
  int testValue = 0xAA;
  EXPECT_CALL(mock, read(_)).Times(5).WillRepeatedly(Return(0xFF));
  EXPECT_CALL(mock, write(_,_)).Times(5).WillRepeatedly(Return(testValue));
  app.writeAll(testValue);
}

TEST_F(DeviceDriverFixture, APP_WRITE_TEST_FAILS_IF_MEMORY_ALREADY_WRITTEN) {
  EXPECT_CALL(mock, read(0x00)).WillOnce(Return(0xFF));
  EXPECT_CALL(mock, write(0x00, 0xAA)).Times(1);

  EXPECT_CALL(mock, read(0x01)).WillOnce(Return(0x11));
  EXPECT_CALL(mock, read(0x02)).WillOnce(Return(0x20));
  EXPECT_CALL(mock, read(0x03)).WillOnce(Return(0x00));
  EXPECT_CALL(mock, read(0x04)).WillOnce(Return(0xAA));

  app.writeAll(0xAA);
}
int main() {
  ::testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}