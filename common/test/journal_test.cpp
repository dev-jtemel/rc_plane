#include "rcplane/common/io/journal.hpp"
#include <gtest/gtest.h>

using namespace rcplane::common::io;

TEST(journal_test, invalid_severity) {
  RCPLANE_SEVERITY(num_of_severities);
  RCPLANE_LOG(num_of_severities, "journal_test", "bad_value");
  RCPLANE_LOG(trace, "journal_test", "trace");
  RCPLANE_LOG(debug, "journal_test", "debug");
  RCPLANE_LOG(info, "journal_test", "info");
  RCPLANE_LOG(warn, "journal_test", "warn");
  RCPLANE_LOG(error, "journal_test", "error");
}

TEST(journal_test, severity_change) {
  RCPLANE_SEVERITY(trace);
  RCPLANE_LOG(trace, "journal_test", "message appears");

  RCPLANE_SEVERITY(info);
  RCPLANE_LOG(trace, "journal_test", "message does NOT appear");
}

TEST(journal_test, all_severities) {
  RCPLANE_SEVERITY(trace);
  RCPLANE_LOG(trace, "journal_test", "trace");
  RCPLANE_LOG(debug, "journal_test", "debug");
  RCPLANE_LOG(info, "journal_test", "info");
  RCPLANE_LOG(warn, "journal_test", "warn");
  RCPLANE_LOG(error, "journal_test", "error");
}
