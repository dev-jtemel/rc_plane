#include <iostream>

#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"
#include "rcplane/common/io/serial.hpp"

int main(int argc, char *argv[]) {
  RCPLANE_SEVERITY(trace);

  RCPLANE_LOG(trace, "test", 1);
  RCPLANE_LOG(debug, "test", 2);
  RCPLANE_LOG(info, "test", 3);
  RCPLANE_LOG(warn, "test", 4);
  RCPLANE_LOG(error, "test", 5);

  rcplane::common::io::serial s;

  s.register_cb([&](std::vector<rcplane::common::io::packet> &packets) {
    auto res = system("clear"); 
    (void)res;

    bool test_complete = (packets[0].data() & 0x2);
    bool flight_mode = (packets[0].data() & 0x1);

    std::cout
      << "FLIGHT_MODE: " << flight_mode << " | TEST COMPLETE: " << test_complete
      << "\nPower: ["
      << std::string(packets[1].data()/2, '#')
      << std::string((255 - packets[1].data())/2, ' ')
      << "]\n"
      << "Aileron: ["
      << packets[2].data() << "*|"
      << (-1 * packets[2].data()) << "*]\n"
      << "Elevator: ["
      << packets[3].data() << "*|"
      << packets[3].data() << "*]\n"
      << "Rudder: [" << packets[4].data() << "*]" << std::endl;
  });

  s.read_serial();
}
