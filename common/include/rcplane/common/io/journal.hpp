#ifndef __RCPLANE__COMMON__IO__JOURNAL_HPP__
#define __RCPLANE__COMMON__IO__JOURNAL_HPP__

#define BOOST_LOG_DYN_LINK 1

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace rcplane {
namespace common {
namespace io {

/**
* @brief Journaling sink for boost::log.
*/
class journal_sink
  : public boost::log::sinks::basic_formatted_sink_backend<char> {
public:
  /**
   * @brief Custom log formatting.
   * @param rec The record view to get meta data from.
   * @param message The message to log.
   */
  void consume(boost::log::record_view const &rec, string_type const &message) {
    auto severity = rec[boost::log::trivial::severity].get();
    switch (severity) {
      case boost::log::trivial::trace: std::cout << "\x1b[38;5;241m"; break;
      case boost::log::trivial::debug: std::cout << "\x1b[38;5;245m"; break;
      case boost::log::trivial::warning: std::cout << "\x1b[38;5;184m"; break;
      case boost::log::trivial::error: std::cout << "\x1b[38;5;196m"; break;
      case boost::log::trivial::info:
      case boost::log::trivial::fatal: break;
    }

    std::cout << "["
              << boost::log::extract<boost::posix_time::ptime>("TimeStamp", rec)
              << "] ";
    std::cout << message << "\x1b[0m" << std::endl;
  }
};

typedef boost::log::sinks::synchronous_sink<rcplane::common::io::journal_sink>
    colored_sink;
static boost::shared_ptr<colored_sink> _journal_sink =
    boost::make_shared<colored_sink>();

#define RCPLANE_LOG_INIT()                                                         \
  do {                                                                         \
    boost::log::add_common_attributes();                                       \
    boost::log::core::get()->add_sink(rcplane::common::io::_journal_sink);     \
    (void)rcplane::common::io::config_manager::instance();                     \
  } while (false)

#define RCPLANE_SEVERITY_UPDATE(lvl)                                           \
  do {                                                                         \
    boost::log::core::get()->set_filter(boost::log::trivial::severity          \
                                        >= boost::log::trivial::lvl);          \
  } while (false)

#define RCPLANE_LOG(lvl, tag, str)                                             \
  do { BOOST_LOG_TRIVIAL(lvl) << "[" << tag << "] " << str; } while (false)

#define RCPLANE_ENTER(tag)                                                     \
  do { BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__; } while (false)

#define RCPLANE_ENABLE_LOGGING()                                               \
  do { boost::log::core::get()->set_logging_enabled(true); } while (false)

#define RCPLANE_DISABLE_LOGGING()                                              \
  do { boost::log::core::get()->set_logging_enabled(false); } while (false)

}  // namespace io
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__JOURNAL_HPP__
