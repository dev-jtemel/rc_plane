#include "rcplane/common/base_controller.hpp"
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/network/http_controller.hpp"

#include <chrono>
#include <curl/curl.h>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

using namespace rcplane::common::network;

size_t curl_write(void *contents, size_t size, size_t nmemb, std::string *s) {
  size_t newLength = size * nmemb;
  try {
    s->append((char *)contents, newLength);
  } catch (std::bad_alloc &e) { return 0; }
  return newLength;
}

CURL *init_curl() {
  CURL *curl = curl_easy_init();
  if (!curl) { return nullptr; }
  std::this_thread::sleep_for(std::chrono::seconds(1U));
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curl_write);
  return curl;
}

void kill_server(CURL *curl) {
  curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/stop");
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  curl = NULL;
}

TEST(http_controller, state) {
  std::unique_ptr<http_controller> ctrl =
      std::make_unique<http_controller>([](int) {});
  ASSERT_EQ(ctrl->state(), http_controller::state::uninitialized);

  ASSERT_TRUE(ctrl->init());
  ASSERT_EQ(ctrl->state(), http_controller::state::initialized);

  ctrl->start();
  ASSERT_EQ(ctrl->state(), http_controller::state::running);

  auto curl = init_curl();
  kill_server(curl);

  ctrl->terminate();
  ASSERT_EQ(ctrl->state(), http_controller::state::terminated);
}

TEST(http_controller, gps_cb) {
  std::unique_ptr<http_controller> ctrl =
      std::make_unique<http_controller>([](int) {});
  ASSERT_TRUE(ctrl->init());
  ctrl->start();

  float lt = 1.f;
  float ln = 2.f;
  float track = 3.f;
  float speed = 4.f;

  ctrl->gps_cb(lt, ln, track, speed);

  auto curl = init_curl();

  std::string res;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
  curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/gps");
  curl_easy_perform(curl);

  ASSERT_EQ(res, "{\"latitude\":1,\"longitude\":2,\"track\":3,\"speed\":4}");

  res = "";
  lt = 5.f;
  ln = 6.f;
  track = 7.f;
  speed = 8.f;

  ctrl->gps_cb(lt, ln, track, speed);

  curl_easy_perform(curl);

  ASSERT_EQ(res, "{\"latitude\":5,\"longitude\":6,\"track\":7,\"speed\":8}");

  kill_server(curl);

  ctrl->terminate();
}

TEST(http_controller, gps_cb_skip_first) {
  std::unique_ptr<http_controller> ctrl =
      std::make_unique<http_controller>([](int) {});
  ASSERT_TRUE(ctrl->init());
  ctrl->start();

  float lt = 1.f;
  float ln = 2.f;
  float track = 3.f;
  float speed = 4.f;
  ctrl->gps_cb(lt, ln, track, speed);

  lt = 5.f;
  ln = 6.f;
  track = 7.f;
  speed = 8.f;
  ctrl->gps_cb(lt, ln, track, speed);

  std::string res;
  auto curl = init_curl();
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
  curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/gps");
  curl_easy_perform(curl);

  ASSERT_EQ(res, "{\"latitude\":5,\"longitude\":6,\"track\":7,\"speed\":8}");

  kill_server(curl);

  ctrl->terminate();
}

TEST(http_controller, gyro_cb) {
  std::unique_ptr<http_controller> ctrl =
      std::make_unique<http_controller>([](int) {});
  ASSERT_TRUE(ctrl->init());
  ctrl->start();

  float pitch = 1.f;
  float roll = 2.f;
  float yaw = 3.f;

  ctrl->gyro_cb(pitch, roll, yaw);

  auto curl = init_curl();

  std::string res;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
  curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/gyro");
  curl_easy_perform(curl);

  ASSERT_EQ(res, "{\"pitch\":1,\"roll\":2,\"yaw\":3}");

  res = "";
  pitch = 4.f;
  roll = 5.f;
  yaw = 6.f;

  ctrl->gyro_cb(pitch, roll, yaw);

  curl_easy_perform(curl);

  ASSERT_EQ(res, "{\"pitch\":4,\"roll\":5,\"yaw\":6}");

  kill_server(curl);

  ctrl->terminate();
}

TEST(http_controller, gyro_cb_skip_first) {
  std::unique_ptr<http_controller> ctrl =
      std::make_unique<http_controller>([](int) {});
  ASSERT_TRUE(ctrl->init());
  ctrl->start();

  float pitch = 1.f;
  float roll = 2.f;
  float yaw = 3.f;
  ctrl->gyro_cb(pitch, roll, yaw);

  pitch = 4.f;
  roll = 5.f;
  yaw = 6.f;
  ctrl->gyro_cb(pitch, roll, yaw);

  auto curl = init_curl();

  std::string res;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
  curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/gyro");
  curl_easy_perform(curl);

  ASSERT_EQ(res, "{\"pitch\":4,\"roll\":5,\"yaw\":6}");

  kill_server(curl);

  ctrl->terminate();
}
