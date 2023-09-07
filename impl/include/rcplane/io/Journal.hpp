#ifndef __RCPLANE__IO__JOURNAL_HPP__
#define __RCPLANE__IO__JOURNAL_HPP__

#include <boost/core/demangle.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <typeinfo>

namespace rcplane {
namespace io {

/**
* @brief Journaling sink for boost::log.
*/
class JournalSink
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
              << "] [" << boost::this_thread::get_id() << "] "
              << "[" << rec[boost::log::trivial::severity] << "] ";
    std::cout << message << "\x1b[0m\n";
  }
};

typedef boost::log::sinks::synchronous_sink<rcplane::io::JournalSink>
    colored_sink;
static boost::shared_ptr<colored_sink> g_journalSink =
    boost::make_shared<colored_sink>();

#define RCPLANE_LOG_INIT()                                                     \
  do {                                                                         \
    boost::log::add_common_attributes();                                       \
    boost::log::core::get()->add_sink(rcplane::io::g_journalSink);             \
    boost::log::core::get()->set_filter(boost::log::trivial::severity          \
                                        >= boost::log::trivial::trace);        \
  } while (false)

#define RCPLANE_LOG(lvl, msg)                                                  \
  do {                                                                         \
    BOOST_LOG_TRIVIAL(lvl) << "["                                              \
                           << boost::core::demangle(typeid(*this).name())      \
                           << "] " << msg;                                     \
  } while (false)

#define RCPLANE_LOG_EXTERNAL(lvl, tag, msg)                                    \
  do { BOOST_LOG_TRIVIAL(lvl) << "[" << tag << "] " << msg; } while (false)

#define RCPLANE_LOG_TEST(lvl, msg)                                             \
  do {                                                                         \
    BOOST_LOG_TRIVIAL(lvl)                                                     \
        << "["                                                                 \
        << ::testing::UnitTest::GetInstance()->current_test_info()->name()     \
        << "] " << msg;                                                        \
  } while (false)

#define RCPLANE_LOG_METHOD()                                                   \
  do {                                                                         \
    BOOST_LOG_TRIVIAL(trace)                                                   \
        << boost::core::demangle(typeid(*this).name()) << "::" << __func__;    \
  } while (false)

#define RCPLANE_LOG_ENABLE()                                                   \
  do { boost::log::core::get()->set_logging_enabled(true); } while (false)

#define RCPLANE_LOG_DISABLE()                                                  \
  do { boost::log::core::get()->set_logging_enabled(false); } while (false)

}  // namespace io
}  // namespace rcplane

#endif  //__RCPLANE__IO__JOURNAL_HPP__