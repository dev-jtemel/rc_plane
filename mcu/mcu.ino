#include "lib/utils/logger.hpp"
#include "lib/mcu_manager.hpp"

mcu::lib::logger logger;
mcu::lib::mcu_manager mcu_manager;

void setup() {
  logger.init();
  mcu_manager.init();
  delay(2000);

  mcu_manager.setup();
  delay(2000);
}

void loop() {
  mcu_manager.step();
}
