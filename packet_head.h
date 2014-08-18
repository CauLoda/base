#ifndef BASE_PACKET_HEAD_H_
#define BASE_PACKET_HEAD_H_

#include "packet_stream.h"

namespace base {

class PacketHead : public PacketReadWriteInterface {
 public:
  PacketHead(BYTE4 sequence_number, BYTE4 type, BYTE4 size) : 
    sequence_number_(sequence_number), type_(type), size_(size) {}
  virtual ~PacketHead() {}
  virtual bool Read(PacketReadStream& stream) override {
    if (stream.Read(sequence_number_) &&
      stream.Read(type_) &&
      stream.Read(size_)) {
      return true;
    }
    return false;
  }
  virtual bool Write(PacketWriteStream& stream) const override {
    if (stream.Write(sequence_number_) &&
      stream.Write(type_) &&
      stream.Write(size_)) {
      return true;
    }
    return false;
  }
  virtual int CalculateSize() const override {
    auto total_size = 0;
    total_size += sizeof(sequence_number_);
    total_size += sizeof(type_);
    total_size += sizeof(size_);
    return total_size;
  }
  void AdjustPacketSize() { set_size(CalculateSize()); }

  static std::unique_ptr<PacketHead> Create(PacketReadStream& stream);

  BYTE4 size() const { return size_; }
  void set_size(BYTE4 value) { size_ = value; }
  BYTE4 sequence_number() const { return sequence_number_; }
  void set_sequence_number(BYTE4 value) { sequence_number_ = value; }
  BYTE4 type() const { return type_; }
  void set_type(BYTE4 value) { type_ = value; }

 private:
  BYTE4 sequence_number_;
  BYTE4 type_;
  BYTE4 size_;
};

template<unsigned long PACKET_TYPE>
class SimplePacket : public PacketHead {
 public:
  SimplePacket() : PacketHead(0, PACKET_TYPE, 0) {}
  SimplePacket(const PacketHead& head) : PacketHead(head) {}
  virtual ~SimplePacket() override {}
  virtual bool Read(PacketReadStream& stream) override {
    return true;
  }
  virtual bool Write(PacketWriteStream& stream) const override {
    return PacketHead::Write(stream);
  }
  virtual int CalculateSize() const override {
    return PacketHead::CalculateSize();
  }
};

} // namespace base

#endif	// BASE_PACKET_HEAD_H_