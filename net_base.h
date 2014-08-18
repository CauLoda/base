#ifndef BASE_NET_BASE_H_
#define BASE_NET_BASE_H_

#include "log.h"
#include "net_interface.h"
#include "packet_head.h"

namespace base {

class NetBase : public net::NetInterface {
 public:
  virtual bool OnTcpDisconnected(net::TcpHandle handle) = 0;
  virtual bool OnTcpAccepted(net::TcpHandle handle, net::TcpHandle accept_handle) = 0;
  virtual bool OnTcpReceived(net::TcpHandle handle, std::unique_ptr<PacketHead> packet) = 0;
  virtual bool OnTcpError(net::TcpHandle handle, int error) = 0;
  virtual bool OnUdpReceived(net::UdpHandle handle, std::unique_ptr<PacketHead> packet, std::string ip, int port) = 0;
  virtual bool OnUdpError(net::UdpHandle handle, int error) = 0;

  virtual bool OnTcpReceived(net::TcpHandle handle, const char* packet, int size) override {
    PacketReadStream stream(packet, size);
    auto new_packet = PacketHead::Create(stream);
    if (new_packet == nullptr) {
      LOG(kError, "recv unknow type packet.");
      return false;
    }
    if (new_packet->size() != size) {
      LOG(kError, "recv invalid size packet.");
      return false;
    }
    return OnTcpReceived(handle, std::move(new_packet));
  }
  virtual bool OnUdpReceived(net::UdpHandle handle, const char* packet, int size, std::string ip, int port) override {
    PacketReadStream stream(packet, size);
    auto new_packet = PacketHead::Create(stream);
    if (new_packet == nullptr) {
      LOG(kError, "recv unknow type packet.");
      return false;
    }
    if (new_packet->size() != size) {
      LOG(kError, "recv invalid size packet.");
      return false;
    }
    return OnUdpReceived(handle, std::move(new_packet), std::move(ip), port);
  }
  bool TcpSend(net::TcpHandle handle, const PacketHead& packet) {
    PacketWriteStream stream(packet.size());
    if (!packet.Write(stream)) {
      return false;
    }
    return NetInterface::TcpSend(handle, stream.packet(), stream.size());
  }
  bool UdpSendTo(net::UdpHandle handle, const PacketHead& packet, const std::string& ip, int port) {
    PacketWriteStream stream(packet.size());
    if (!packet.Write(stream)) {
      return false;
    }
    return NetInterface::UdpSendTo(handle, stream.packet(), stream.size(), ip, port);
  }
};

} // namespace base

#endif // BASE_NET_BASE_H_