#ifndef __RCPLANE__HW__IMU_MANAGER_HPP__
#define __RCPLANE__HW__IMU_MANAGER_HPP__

#include <boost/noncopyable.hpp>

#include "rcplane/base_controller.hpp"
#include "rcplane/common/packet.hpp"

namespace rcplane {
namespace hw {

/**
 * @brief Handle and manage imu data from the MCU.
 * 
 * The imu is used to predict and control the pose and position
 * of the plane.
 */
class imu_manager : public interface::base_controller,
                    public ::boost::noncopyable {
public:
  /**
  * @brief Construct a new imu manager object.
  */
  explicit imu_manager();
  ~imu_manager();

  /**
   * @brief nop.
   * @return true always.
   */
  bool init() override;

  /**
   * @brief nop.
   */
  void start() override;

  /**
   * @brief nop.
   */
  void terminate() override;

  /**
   * @brief Handle an incoming imu_packet, applying
   * and filters and/or limiters.
   * 
   * @param _imu_packet Pointer to the most recent packet received.
   */
  int8_t on(common::imu_packet *_imu_packet);

private:
  /**
   * @brief Limit the roll possible within the configured bounds.
   * 
   * @param roll Roll ref in the current packet.
   */
  void roll_limiter(int8_t &roll);

  /**
   * @brief Limit the pitch possible within the configured bounds.
   * 
   * @param pitch Pitch ref in the current packet.
   */
  void pitch_limiter(int8_t &pitch);

  int8_t cROLL_MAX_POS{};
  int8_t cROLL_MAX_NEG{};
  int8_t cPTICH_MAX_POS{};
  int8_t cPTICH_MAX_NEG{};
};

}  // namespace hw
}  // namespace rcplane

#endif  //__RCPLANE__HW__IMU_MANAGER_HPP__