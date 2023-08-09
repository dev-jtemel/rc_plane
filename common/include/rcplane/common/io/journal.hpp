#ifndef __RCPLANE__COMMON__IO__JOURNAL_HPP__
#define __RCPLANE__COMMON__IO__JOURNAL_HPP__

#define BOOST_LOG_DYN_LINK 1

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace rcplane {
namespace common {
namespace io {

#define RCPLANE_SEVERITY(lvl)                                                  \
  do {                                                                         \
    boost::log::core::get()->set_filter(                                       \
        boost::log::trivial::severity >= boost::log::trivial::lvl              \
    );                                                                         \
  } while (false)

#define RCPLANE_LOG(lvl, tag, str)                                             \
  do {                                                                         \
    BOOST_LOG_TRIVIAL(lvl) << "[" << tag << "] " << str;                       \
  } while (false)

#define RCPLANE_ENTER(tag) BOOST_LOG_TRIVIAL(trace) << __PRETTY_FUNCTION__
  


}  // namespace io
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__JOURNAL_HPP__
