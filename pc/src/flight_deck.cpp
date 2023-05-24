
#include <fstream>
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

int main(int argc, char *argv[]) {
  RCPLANE_SEVERITY(trace);

  RCPLANE_LOG(trace, "test", 1);
  RCPLANE_LOG(debug, "test", 2);
  RCPLANE_LOG(info, "test", 3);
  RCPLANE_LOG(warn, "test", 4);
  RCPLANE_LOG(error, "test", 5);

  std::ifstream infile("./log.log");
  
  uint32_t b;
  while (infile >> b) {
    rcplane::common::io::packet packet(b);
    RCPLANE_LOG(info, "packet", packet.type_to_str() << " " << packet.data());
  }
}
