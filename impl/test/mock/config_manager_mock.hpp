#ifndef __RCPLANE__TEST__MOCK__CONFIG_MANAGER_MOCK_HPP__
#define __RCPLANE__TEST__MOCK__CONFIG_MANAGER_MOCK_HPP__

#include "rcplane/io/config_manager.hpp"
#include <gmock/gmock.h>

namespace rcplane {
namespace test {

class config_manager_mock : public rcplane::io::config_manager {
public:
  MOCK_METHOD(void, read_config, (const std::string &), (override));
  MOCK_METHOD(void, set_log_severity, (const std::string &&severity), ());
};

}  // namespace test
}  // namespace rcplane

#endif  // __RCPLANE__TEST__MOCK__CONFIG_MANAGER_MOCK_HPP__