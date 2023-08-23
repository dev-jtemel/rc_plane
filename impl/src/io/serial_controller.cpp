/**
 * @file serial_controller.cpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-21
 */
#include "rcplane/io/serial_controller.hpp"
#include "rcplane/io/config_manager.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace io {

serial_controller::serial_controller(config_manager &config_manager,
                                     boost::asio::io_context &io)
  : interface::base_controller("serial_controller"), _io(io), _serial(_io) {
  RCPLANE_ENTER();

  cBLACKBOX = config_manager.get<std::string>(
      "rcplane.io.serial_controller.blackbox_destination");
  cTTY = config_manager.get<std::string>("rcplane.io.serial_controller.dev");
  cBAUDRATE =
      config_manager.get<uint32_t>("rcplane.io.serial_controller.baudrate");
}

serial_controller::~serial_controller() { RCPLANE_ENTER(); }

bool serial_controller::init() {
  RCPLANE_ENTER();

  static_assert(sizeof(float) == 4U);

  if (!open_port()) { return false; }

  if (!handshake_mcu()) { return false; }

  _blackbox_in = std::ofstream(cBLACKBOX + "bb_in.bin", std::ios::binary);
  _blackbox_out = std::ofstream(cBLACKBOX + "bb_out.bin", std::ios::binary);

  RCPLANE_LOG(info, _tag, "initialized");
  return true;
}

void serial_controller::start() {
  RCPLANE_ENTER();

  _running = true;
  _worker = boost::thread(&serial_controller::read_write_serial, this);

  RCPLANE_LOG(info, _tag, "started");
}

void serial_controller::terminate() {
  RCPLANE_ENTER();

  _running = false;
  _worker.join();
  RCPLANE_LOG(info, _tag, "worker collected");
  RCPLANE_LOG(info, _tag, "terminated");
}

void serial_controller::on_write_signal() {
  RCPLANE_ENTER();

  _io.post(boost::bind(&rcplane::io::serial_controller::write_packet, this));
}

boost::signals2::signal<void(common::state_packet *)>
    &serial_controller::state_signal() {
  RCPLANE_ENTER();
  return _state_signal;
}

boost::signals2::signal<void(common::control_surface_packet *,
                             common::imu_packet *)>
    &serial_controller::packet_signal() {
  RCPLANE_ENTER();
  return _packet_signal;
}

void serial_controller::read_write_serial() {
  RCPLANE_ENTER();

  RCPLANE_LOG(info, _tag, "worker started");

  while (_running) {
    _state_packet = read_packet<rcplane::common::state_packet>();
    _cs_packet = read_packet<rcplane::common::control_surface_packet>();
    _imu_packet = read_packet<rcplane::common::imu_packet>();

    _state_signal(_state_packet);
    _packet_signal(_cs_packet, _imu_packet);
  }
}

bool serial_controller::open_port() {
  RCPLANE_ENTER();

  try {
    _serial.open(cTTY);
    _serial.set_option(boost::asio::serial_port_base::baud_rate(cBAUDRATE));

    ::tcflush(_serial.lowest_layer().native_handle(), TCIFLUSH);
    flush();
    return true;
  } catch (...) {
    RCPLANE_LOG(error, _tag, "failed to open serial port");
    return false;
  }
}

bool serial_controller::handshake_mcu() {
  RCPLANE_ENTER();

  boost::asio::write(_serial,
                     boost::asio::buffer(&kHELLO_TX, sizeof(kHELLO_TX)));
  return true;
}

template<typename PACKET_TYPE>
PACKET_TYPE *serial_controller::read_packet() {
  RCPLANE_ENTER();

  boost::asio::read(_serial,
                    _streambuffer,
                    boost::asio::transfer_exactly(sizeof(PACKET_TYPE)));

  PACKET_TYPE *packet = const_cast<PACKET_TYPE *>(
      boost::asio::buffer_cast<const PACKET_TYPE *>(_streambuffer.data()));

  _streambuffer.consume(sizeof(PACKET_TYPE));

  _blackbox_in.write(reinterpret_cast<const char *>(packet),
                     sizeof(PACKET_TYPE));
  return packet;
}

void serial_controller::write_packet() {
  RCPLANE_ENTER();

  RCPLANE_LOG(debug,
              _tag,
              "timestamp = " << _state_packet->timestamp << " |"
                             << " state = "
                             << std::bitset<8>(_state_packet->state));

  RCPLANE_LOG(debug,
              _tag,
              "motor = " << +_cs_packet->motor
                         << " | aileron = " << +_cs_packet->aileron
                         << " | elevator = " << +_cs_packet->elevator
                         << " | rudder = " << +_cs_packet->rudder);

  RCPLANE_LOG(debug,
              _tag,
              "roll = " << _imu_packet->roll
                        << " | pitch = " << _imu_packet->pitch
                        << " | yaw = " << _imu_packet->yaw);

  boost::asio::write(
      _serial,
      boost::asio::buffer(reinterpret_cast<uint8_t *>(_cs_packet),
                          sizeof(common::control_surface_packet)));

  _blackbox_out.write(reinterpret_cast<const char *>(_cs_packet),
                      sizeof(common::control_surface_packet));
}

void serial_controller::flush() {
  RCPLANE_ENTER();

  std::string res{};
  while (res != kHELLO_RX) {
    boost::asio::read_until(_serial, _streambuffer, '\n');
    std::istream is(&_streambuffer);
    std::getline(is, res);
  RCPLANE_LOG(info, _tag, res);
  }
  _streambuffer.consume(1000);
  RCPLANE_LOG(info, _tag, "flushed");
}

template rcplane::common::state_packet *serial_controller::read_packet<
    rcplane::common::state_packet>();
template rcplane::common::control_surface_packet *serial_controller::
    read_packet<rcplane::common::control_surface_packet>();
template rcplane::common::imu_packet *serial_controller::read_packet<
    rcplane::common::imu_packet>();

}  // namespace io
}  // namespace rcplane