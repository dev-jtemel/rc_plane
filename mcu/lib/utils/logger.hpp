#ifndef __MCU__LIB__LOGGER_HPP__
#define __MCU__LIB__LOGGER_HPP__

#include <Arduino.h>

namespace mcu {
namespace lib {

class logger {
 public:
  logger() = default;
  ~logger() = default;

  void init() {
    Serial.begin(115200);
  }

  template <typename T>
  void log(T t) {
    Serial.println(t);
  }
};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__LOGGER_HPP__
