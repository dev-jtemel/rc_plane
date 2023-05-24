
#include <fstream>
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

  s.read_serial();
}
