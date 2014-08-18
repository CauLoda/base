#include "packet_head.h"

namespace base {

std::unique_ptr<PacketHead> PacketHead::Create(PacketReadStream& stream) {
  PacketHead head(0, 0, 0);
  if (!head.Read(stream)) {
    return nullptr;
  }
  std::unique_ptr<PacketHead> packet;
  switch (head.type()) {
  //case kPacketTypeKeepAlive:
  //  packet.reset(new PacketKeepAlive(head));
  //  break;
  default:
    return nullptr;
  }
  if (!packet->Read(stream)) {
    return nullptr;
  }
  return std::move(packet);
}

} // namespace base