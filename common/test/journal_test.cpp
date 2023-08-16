#include "rcplane/common/io/journal.hpp"
#include <gtest/gtest.h>

using namespace rcplane::common::io;

TEST(journal_test, severity_change) {
  RCPLANE_LOG(trace, "journal_test", "message appears");

  RCPLANE_SEVERITY_UPDATE(info);
  RCPLANE_LOG(trace, "journal_test", "message does NOT appear");

  RCPLANE_SEVERITY_UPDATE(trace);
}

TEST(journal_test, all_severities) {
  RCPLANE_SEVERITY_UPDATE(trace);
  RCPLANE_LOG(trace, "journal_test", "trace");
  RCPLANE_LOG(debug, "journal_test", "debug");
  RCPLANE_LOG(info, "journal_test", "info");
  RCPLANE_LOG(warning, "journal_test", "warn");
  RCPLANE_LOG(error, "journal_test", "error");
}
