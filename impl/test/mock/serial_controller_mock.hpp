#ifndef __RCPLANE__TEST__MOCK__SERIAL_CONTROLLER_MOCK_HPP__
#define __RCPLANE__TEST__MOCK__SERIAL_CONTROLLER_MOCK_HPP__

#include <gmock/gmock.h>

#include "rcplane/io/serial_controller.hpp"
namespace rcplane {
namespace test {

class serial_controller_mock : public rcplane::io::serial_controller {
public:
  serial_controller_mock(rcplane::io::config_manager &config_manager,
                         boost::asio::io_context &io)
    : serial_controller(config_manager, io) {}
};

}  // namespace test
}  // namespace rcplane

#endif  // __RCPLANE__TEST__MOCK__SERIAL_CONTROLLER_MOCK_HPP__