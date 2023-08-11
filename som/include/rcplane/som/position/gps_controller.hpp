#ifndef __RCPLANE__SOM__POSITION__GPS_CONTROLLER_HPP__
#define __RCPLANE__SOM__POSITION__GPS_CONTROLLER_HPP__

#define MODE_STR_NUM 4

#include <functional>
#include <gps.h>

#include "rcplane/common/base_controller.hpp"

namespace rcplane {
namespace som {
namespace position {

/**
 * @brief Read incoming gps data.
 */
class gps_controller : public ::rcplane::common::interface::base_controller {
public:
  gps_controller();
  ~gps_controller();

  /**
   * @brief Connect to the gpsd service.
   * @return Status of the initialization.
   */
  bool init() override;

  /**
   * @brief Spawn a _worker thread and read gps data.
   * @pre init()
   */
  void start() override;

  /**
   * @brief Terminate the gps _worker thread.
   * @pre start()
   */
  void terminate() override;

  /**
   * @brief Register a callback to fire when new gps data is available.
   * @param cb Callback to register.
   */
  void register_cb(std::function<void(float, float, float, float)> cb);

private:
  /**
   * @brief Read the first available gnss data.
   * @warning Timesout if no data is read.
   */
  void p_read_gps();

  std::string GPSD_PORT = "2947";
  size_t GPS_DELAY = 5000000;
  std::string MODE_STR[MODE_STR_NUM] = {"n/a", "None", "2D", "3D"};
  std::vector<std::function<void(float, float, float, float)>> _cbs;
  struct gps_data_t _gps_data;
};

}  // namespace position
}  // namespace som
}  // namespace rcplane

#endif  //__RCPLANE__SOM__POSITION__GPS_CONTROLLER_HPP__
