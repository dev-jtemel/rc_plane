
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

int main(int argc, char *argv[]) {
  RCPLANE_SEVERITY(trace);

  RCPLANE_LOG(trace, "test", 1);
  RCPLANE_LOG(debug, "test", 2);
  RCPLANE_LOG(info, "test", 3);
  RCPLANE_LOG(warn, "test", 4);
  RCPLANE_LOG(error, "test", 5);

  const uint32_t test[] = {
    0x01000001,
    0x020000FF,
    0x82000022, 
    0x04000004, 
    0x84000004, 
    0x08000008, 
    0x88000008, 
    0x10000010, 
    0x90000010, 
  };

  for (auto b : test) {
    rcplane::common::io::packet packet(b);
    RCPLANE_LOG(info, "packet", packet.type_to_str() << " " << packet.data());
  }
}
