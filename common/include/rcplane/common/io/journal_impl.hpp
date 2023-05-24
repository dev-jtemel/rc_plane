#ifndef __RCPLANE__COMMON__IO__JOURNAL_HPP__
#error "Do not include this file directly, instead include rcplane/common/io/journal.hpp"
#endif

#ifndef __RCPLANE__COMMON__IO__JOURNAL_IMPL_HPP__
#define __RCPLANE__COMMON__IO__JOURNAL_IMPL_HPP__

#include <sstream>
#include <mutex>

namespace rcplane {
namespace common {
namespace io {

#define RCPLANE_SEVERITY(lvl) \
  rcplane::io::journal::instance().lock(); \
  rcplane::io::journal::instance().set_severity(rcplane::io::journal::severity::lvl); \
  rcplane::io::journal::instance().unlock();

#define RCPLANE_LOG(lvl, tag, str) \
  do { \
    rcplane::io::journal::instance().lock(); \
    rcplane::io::journal::instance().stream() << str; \
    rcplane::io::journal::instance().log(rcplane::io::journal::severity::lvl, tag); \
    rcplane::io::journal::instance().unlock(); \
  } while (false)


/**
 * This singleton class handles logging to the console.
 *
 * TODO: Log to file aswell.
 */
class journal {
 public:
  /**
   * The possible severities to log to.
   */
  enum severity {
    trace = 0,
    debug,
    info,
    warn,
    error,

    num_of_severities = 255
  };

  /**
   * Colors for the severities in hex.
   */
  enum colors {
    c_default = 0,
    c_trace = 240,
    c_debug = 247,
    c_info = 15,
    c_warn = 226,
    c_error = 196
  };

  static journal &instance();

  /**
   * Enfore singleton.
   */
  journal(const journal &) = delete;
  journal(journal &&) = delete;


  /**
   * Verify the severity and log the contents in the internal stream to
   * the console under stderr.
   *
   * Internal stream is cleared here.
   */
  void log(severity slvl, const std::string tag);

  void set_severity(severity slvl);

  void lock();
  void unlock();
 
  std::stringstream &stream();
 private:
  journal();

  uint64_t now();
  std::string severity_to_str(severity slvl);
  colors severity_to_color(severity slvl);
 
  severity _slvl; // default is info
  std::stringstream _ss;
  std::mutex _lock;
};

} // namespace io
} // namespace common
} // namespace rcplane

#endif //__RCPLANE__COMMON__COMMON__IO__JOURNAL_IMPL_HPP__
