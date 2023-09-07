#ifndef RCPLANE_COMMON_HELPERS_HPP
#define RCPLANE_COMMON_HELPERS_HPP

#include "rcplane/io/Journal.hpp"

#define RCPLANE_ASSERT(cond)                                                   \
  do {                                                                         \
    if (!(cond)) {                                                             \
      RCPLANE_LOG(error, "Assertion failed :: " << #cond);                     \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

#define RCPLANE_ASSERT_EXTERNAL(cond, tag)                                     \
  do {                                                                         \
    if (!(cond)) {                                                             \
      RCPLANE_LOG_EXTERNAL(error, tag, "Assertion failed :: " << #cond);       \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

#endif  // RCPLANE_COMMON_HELPERS_HPP