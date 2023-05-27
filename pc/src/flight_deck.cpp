#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/serial.hpp"

#define TAG "flight-deck"

int main(int argc, char *argv[]) {
  RCPLANE_SEVERITY(trace);

  rcplane::common::io::serial s;
  s.register_cb([&](auto &packets){
    for (auto &p : packets) {
      RCPLANE_LOG(info, p.type_to_str(), p.data());  
    }
  });
  s.read_serial();
  return EXIT_SUCCESS;
}
