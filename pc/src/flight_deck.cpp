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

  s.motor_cb([&](rcplane::common::io::packet &p) {
    (void)system("clear"); 
    std::cout
      << "Power: ["
      << std::string(p.data()/2, '#')
      << std::string((255 - p.data())/2, ' ')
      << "]\n";
  });

  s.aileron_cb([&](rcplane::common::io::packet &p) {
    std::cout
      << "Aileron: ["
      << p.data() << "*|"
      << (-1 * p.data()) << "*]\n";
  });

  s.elevator_cb([&](rcplane::common::io::packet &p) {
    std::cout
      << "Elevator: ["
      << p.data() << "*|"
      << p.data() << "*]\n";
  });

  s.rudder_cb([&](rcplane::common::io::packet &p) {
    std::cout
      << "Rudder: [" << p.data() << "*]" << std::endl;
  });

  s.read_serial();
}
