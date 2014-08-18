#ifndef BASE_PACKET_ARRAY_H_
#define BASE_PACKET_ARRAY_H_

#include "packet_stream.h"
#include <vector>

namespace base {

typedef BYTE4 SIZE_TYPE;

class PacketArrayBYTE1 : public PacketReadWriteInterface {
 public:
  virtual bool Read(PacketReadStream& stream) override {
    SIZE_TYPE size = 0;
    if (!stream.Read(size)) {
      return false;
    }
    if (size == 0) {
      return true;
    }
    array_.resize(size);
    if (!stream.Read(&array_[0], size)) {
      return false;
    }
    return true;
  }
  virtual bool Write(PacketWriteStream& stream) const override {
    SIZE_TYPE size = array_.size();
    if (!stream.Write(size)) {
      return false;
    }
    if (size == 0) {
      return true;
    }
    if (!stream.Write(&array_[0], size)) {
      return false;
    }
    return true;
  }
  virtual int CalculateSize() const override {
    auto total_size = 0;
    total_size += sizeof(SIZE_TYPE);
    total_size += array_.size() * sizeof(BYTE1);
    return total_size;
  }

  void set_array(const std::string& str) { array_.assign(str.begin(), str.end()); }
  void set_array(const std::vector<BYTE1>& array) { array_ = array; }
  void set_array(std::vector<BYTE1>&& array) { array_ = std::move(array); }
  std::vector<BYTE1> array() { return std::move(array_); }

 private:
  std::vector<BYTE1> array_;
};

class PacketArrayBYTE2 : public PacketReadWriteInterface {
 public:
  virtual bool Read(PacketReadStream& stream) override {
    SIZE_TYPE size = 0;
    if (!stream.Read(size)) {
      return false;
    }
    array_.reserve(size);
    for (SIZE_TYPE i = 0; i < size; ++i) {
      BYTE2 obj = 0;
      if (!stream.Read(obj)) {
        return false;
      }
      array_.push_back(obj);
    }
    return true;
  }
  virtual bool Write(PacketWriteStream& stream) const override {
    SIZE_TYPE size = array_.size();
    if (!stream.Write(size)) {
      return false;
    }
    for (const auto& i : array_) {
      if (!stream.Write(i)) {
        return false;
      }
    }
    return true;
  }
  virtual int CalculateSize() const override {
    auto total_size = 0;
    total_size += sizeof(SIZE_TYPE);
    total_size += array_.size() * sizeof(BYTE2);
    return total_size;
  }

  void set_array(const std::wstring& str) { array_.assign(str.begin(), str.end()); }
  void set_array(const std::vector<BYTE2>& array) { array_ = array; }
  void set_array(std::vector<BYTE2>&& array) { array_ = std::move(array); }
  std::vector<BYTE2> array() { return std::move(array_); }

 private:
  std::vector<BYTE2> array_;
};

class PacketArrayBYTE4 : public PacketReadWriteInterface {
 public:
  virtual bool Read(PacketReadStream& stream) override {
    SIZE_TYPE size = 0;
    if (!stream.Read(size)) {
      return false;
    }
    array_.reserve(size);
    for (SIZE_TYPE i = 0; i < size; ++i) {
      BYTE4 obj = 0;
      if (!stream.Read(obj)) {
        return false;
      }
      array_.push_back(obj);
    }
    return true;
  }
  virtual bool Write(PacketWriteStream& stream) const override {
    SIZE_TYPE size = array_.size();
    if (!stream.Write(size)) {
      return false;
    }
    for (const auto& i : array_) {
      if (!stream.Write(i)) {
        return false;
      }
    }
    return true;
  }
  virtual int CalculateSize() const override {
    auto total_size = 0;
    total_size += sizeof(SIZE_TYPE);
    total_size += array_.size() * sizeof(BYTE4);
    return total_size;
  }

  void set_array(const std::vector<BYTE4>& array) { array_ = array; }
  void set_array(std::vector<BYTE4>&& array) { array_ = std::move(array); }
  std::vector<BYTE4> array() { return std::move(array_); }

 private:
  std::vector<BYTE4> array_;
};

template<typename T>
class PacketArray : public PacketReadWriteInterface {
 public:
  virtual bool Read(PacketReadStream& stream) override {
    SIZE_TYPE size = 0;
    if (!stream.Read(size)) {
      return false;
    }
    array_.reserve(size);
    for (SIZE_TYPE i = 0; i < size; ++i) {
      typename T obj;
      if (!obj.Read(stream)) {
        return false;
      }
      array_.push_back(std::move(obj));
    }
    return true;
  }
  virtual bool Write(PacketWriteStream& stream) const override {
    SIZE_TYPE size = array_.size();
    if (!stream.Write(size)) {
      return false;
    }
    for (const auto& i : array_) {
      if (!i.Write(stream)) {
        return false;
      }
    }
    return true;
  }
  virtual int CalculateSize() const override {
    auto total_size = 0;
    total_size += sizeof(SIZE_TYPE);
    for (const auto& i : array_) {
      total_size += i.CalculateSize();
    }
    return total_size;
  }

 public:
  void set_array(const std::vector<T>& array) { array_ = array; }
  void set_array(std::vector<T>&& array) { array_ = std::move(array); }
  std::vector<T> array() { return std::move(array_); }

 private:
  std::vector<T> array_;
};

} // namespace base

#endif	// BASE_PACKET_ARRAY_H_