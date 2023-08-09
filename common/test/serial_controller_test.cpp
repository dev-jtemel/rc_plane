#include "rcplane/common/io/serial_controller.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace rcplane::common::io;

class serial_controller_mock : public serial_controller {
public:
  MOCK_METHOD(bool, init, ());
  MOCK_METHOD(void, p_read_serial, ());

  void append_line(std::string data) { simulated_data.push_back(data); }

private:
  std::vector<std::string> simulated_data;
};

TEST(serial_controller_test, lifecycle) {
  serial_controller_mock sc;
  EXPECT_CALL(sc, init).Times(1);
  EXPECT_CALL(sc, p_read_serial).Times(1);

  sc.init();
  sc.start();
  sc.terminate();
}
