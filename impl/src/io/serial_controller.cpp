#include "rcplane/io/serial_controller.hpp"
#include "rcplane/io/config_manager.hpp"
#include "rcplane/io/journal.hpp"
#include <chrono>  //rm
#include <thread>  //rm

namespace rcplane {
namespace io {

serial_controller::serial_controller(boost::asio::io_context &io)
  : interface::base_controller("serial-controller"), _io(io), _serial(_io) {
  RCPLANE_ENTER();

  cBLACKBOX = config_manager::instance().get<std::string>(
      "common.io.serial_controller.blackbox");
  cTTY = config_manager::instance().get<std::string>(
      "common.io.serial_controller.dev");
  cBAUDRATE = config_manager::instance().get<uint32_t>(
      "common.io.serial_controller.baudrate");
}

serial_controller::~serial_controller() { RCPLANE_ENTER(); }

bool serial_controller::init() {
  RCPLANE_ENTER();

  static_assert(sizeof(float) == 4U);

  if (!open_port()) { return false; }

  if (!handshake_mcu()) { return false; }

  // TODO: Enable when BOOST_SERIALIZATION is added.
  //_blackbox = std::ofstream(cBLACKBOX);

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
    read_packets();
    _packet_signal(_cs_packet, _imu_packet);
    write_packet();
    _streambuffer.consume(sizeof(common::control_surface_packet)
                          + sizeof(common::imu_packet));
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

void serial_controller::read_packets() {
  RCPLANE_ENTER();

  boost::asio::read(
      _serial,
      _streambuffer,
      boost::asio::transfer_exactly(sizeof(common::control_surface_packet)));

  _cs_packet = const_cast<common::control_surface_packet *>(
      boost::asio::buffer_cast<const common::control_surface_packet *>(
          _streambuffer.data()));

  _streambuffer.consume(sizeof(common::control_surface_packet));

  boost::asio::read(_serial,
                    _streambuffer,
                    boost::asio::transfer_exactly(sizeof(common::imu_packet)));

  _imu_packet = const_cast<common::imu_packet *>(
      boost::asio::buffer_cast<const common::imu_packet *>(
          _streambuffer.data()));

  _streambuffer.consume(sizeof(common::imu_packet));

  // TODO: BOOST serialization to file.
  //_blackbox << _cs_packet << std::endl;
}

void serial_controller::write_packet() {
  RCPLANE_ENTER();

  RCPLANE_LOG(debug,
              _tag,
              "[" << _cs_packet->timestamp << "]"
                  << " state = " << std::bitset<8>(_cs_packet->state)
                  << " | motor = " << +_cs_packet->motor
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
}

void serial_controller::flush() {
  RCPLANE_ENTER();

  std::string res{};
  while (res != kHELLO_RX) {
    boost::asio::read_until(_serial, _streambuffer, '\n');
    std::istream is(&_streambuffer);
    std::getline(is, res);
  }
  _streambuffer.consume(1000);
  RCPLANE_LOG(info, _tag, "flushed");
}

}  // namespace io
}  // namespace rcplane