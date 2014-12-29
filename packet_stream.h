#ifndef BASE_PACKET_STREAM_H_
#define BASE_PACKET_STREAM_H_

#include "uncopyable.h"
#include <memory>
#ifdef WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#include <string.h>
#endif

namespace base {

typedef unsigned char BYTE1;
typedef unsigned short BYTE2;
typedef unsigned long BYTE4;

class PacketReadStream : public utility::Uncopyable {
 public:
  PacketReadStream(const char* packet, int size) : packet_(packet), size_(size), current_position_(0) {}
  bool Read(BYTE1& data) {
    int read_size = sizeof(data);
    if (left() < read_size) {
      return false;
    }
    data = *(BYTE1*)position();
    current_position_ += read_size;
    return true;
  }
  bool Read(BYTE2& data) {
    int read_size = sizeof(data);
    if (left() < read_size) {
      return false;
    }
    data = ntohs(*(BYTE2*)position());
    current_position_ += read_size;
    return true;
  }
  bool Read(BYTE4& data) {
    int read_size = sizeof(data);
    if (left() < read_size) {
      return false;
    }
    data = ntohl(*(BYTE4*)position());
    current_position_ += read_size;
    return true;
  }
  bool Read(BYTE1* data, int size) {
    if (left() < size) {
      return false;
    }
    memcpy(data, position(), size);
    current_position_ += size;
    return true;
  }

 private:
  int left() const { return size_ - current_position_; }
  const char* position() const { return packet_ + current_position_; }

 private:
  const char* packet_;
  int size_;
  int current_position_;
};

class PacketWriteStream {
 public:
  explicit PacketWriteStream(int size) : size_(size), packet_(std::make_unique<char[]>(size)), current_position_(0) {}
  int size() const { return size_; }
  std::unique_ptr<char[]> packet() { return std::move(packet_); }
  bool Write(BYTE1 data) {
    int write_size = sizeof(data);
    if (left() < write_size) {
      return false;
    }
    *(BYTE1*)position() = data;
    current_position_ += write_size;
    return true;
  }
  bool Write(BYTE2 data) {
    int write_size = sizeof(data);
    if (left() < write_size) {
      return false;
    }
    *(BYTE2*)position() = htons(data);
    current_position_ += write_size;
    return true;
  }
  bool Write(BYTE4 data) {
    int write_size = sizeof(data);
    if (left() < write_size) {
      return false;
    }
    *(BYTE4*)position() = htonl(data);
    current_position_ += write_size;
    return true;
  }
  bool Write(const BYTE1* data, int size) {
    if (left() < size) {
      return false;
    }
    memcpy(position(), data, size);
    current_position_ += size;
    return true;
  }

 private:
  int left() const { return size_ - current_position_; }
  char* position() const { return packet_.get() + current_position_; }

 private:
  std::unique_ptr<char[]> packet_;
  int size_;
  int current_position_;
};

class PacketReadWriteInterface {
 public:
  virtual bool Read(PacketReadStream& stream) = 0;
  virtual bool Write(PacketWriteStream& stream) const = 0;
  virtual int CalculateSize() const = 0;
};

} // namespace base

#endif	// BASE_PACKET_STREAM_H_