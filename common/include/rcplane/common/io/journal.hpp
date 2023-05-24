#ifndef __RCPLANE__COMMON__IO__JOURNAL_HPP__
#define __RCPLANE__COMMON__IO__JOURNAL_HPP__

#include "rc/io/journal_impl.hpp"

namespace rcplane {
namespace common {
namespace io {

#define RCPLANE_SEVERITY(lvl) \
  rcplane::common::io::journal::instance().lock(); \
  rcplane::common::io::journal::instance().set_severity(rcplane::common::io::journal::severity::lvl); \
  rcplane::common::io::journal::instance().unlock();

#define RCPLANE_LOG(lvl, tag, str) \
  do { \
    rcplane::common::io::journal::instance().lock(); \
    rcplane::common::io::journal::instance().stream() << str; \
    rcplane::common::io::journal::instance().log(rcplane::common::io::journal::severity::lvl, tag); \
    rcplane::common::io::journal::instance().unlock(); \
  } while (false)

} // namesapce io
} // namesapce common
} // namesapce rcplane

#endif //__RCPLANE__COMMON__IO__JOURNAL_HPP__
