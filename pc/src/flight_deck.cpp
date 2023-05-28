#include <bitset>
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/serial.hpp"

#define TAG "flight-deck"

int main(int argc, char *argv[]) {
  RCPLANE_SEVERITY(trace);

  rcplane::common::io::serial s;
  s.register_cb([&](auto timestamp, auto &packets){
    RCPLANE_LOG(
      info,
      TAG,
      "[" << timestamp << "]"
      << " state = " << std::bitset<8>(packets[0].data())
      << " | motor = " << packets[1].data()
      << " | aileron = " << packets[2].data()
      << " | elevator = " << packets[3].data()
      << " | rudder = " << packets[4].data()
    ); 
  });
  s.read_serial();
  return EXIT_SUCCESS;
}
