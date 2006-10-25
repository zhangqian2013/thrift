#include "TBinaryProtocol.h"

using std::string;

namespace facebook { namespace thrift { namespace protocol { 

uint32_t TBinaryProtocol::writeMessageBegin(const std::string name,
					    const TMessageType messageType,
					    const int32_t seqid) {
  return 
    writeString(name) + 
    writeByte((int8_t)messageType) +
    writeI32(seqid);
}

uint32_t TBinaryProtocol::writeMessageEnd() {
  return 0;
}

uint32_t TBinaryProtocol::writeStructBegin(const string& name) {
  return 0;
}

uint32_t TBinaryProtocol::writeStructEnd() {
  return 0;
}

uint32_t TBinaryProtocol::writeFieldBegin(const string& name,
                                          const TType fieldType,
                                          const int16_t fieldId) {
  return
    writeByte((int8_t)fieldType) +
    writeI16(fieldId);
}

uint32_t TBinaryProtocol::writeFieldEnd() {
  return 0;
}

uint32_t TBinaryProtocol::writeFieldStop() {
  return
    writeByte((int8_t)T_STOP);
}  
                               
uint32_t TBinaryProtocol::writeMapBegin(const TType keyType,
                                        const TType valType,
                                        const uint32_t size) {
  return
    writeByte((int8_t)keyType) +
    writeByte((int8_t)valType) +
    writeI32((int32_t)size);
}

uint32_t TBinaryProtocol::writeMapEnd() {
  return 0;
}

uint32_t TBinaryProtocol::writeListBegin(const TType elemType,
                                         const uint32_t size) {
  return
    writeByte((int8_t) elemType) +
    writeI32((int32_t)size);
}

uint32_t TBinaryProtocol::writeListEnd() {
  return 0;
}

uint32_t TBinaryProtocol::writeSetBegin(const TType elemType,
                                        const uint32_t size) {
  return
    writeByte((int8_t)elemType) +
    writeI32((int32_t)size);
}

uint32_t TBinaryProtocol::writeSetEnd() {
  return 0;
}

uint32_t TBinaryProtocol::writeBool(const bool value) {
  uint8_t tmp =  value ? 1 : 0;
  outputTransport_->write(&tmp, 1);
  return 1;
}

uint32_t TBinaryProtocol::writeByte(const int8_t byte) {
  outputTransport_->write((uint8_t*)&byte, 1);
  return 1;
}

uint32_t TBinaryProtocol::writeI16(const int16_t i16) {
  int16_t net = (int16_t)htons(i16);
  outputTransport_->write((uint8_t*)&net, 2);
  return 2;
}

uint32_t TBinaryProtocol::writeI32(const int32_t i32) {
  int32_t net = (int32_t)htonl(i32);
  outputTransport_->write((uint8_t*)&net, 4);
  return 4;
}

uint32_t TBinaryProtocol::writeI64(const int64_t i64) {
  int64_t net = (int64_t)htonll(i64);
  outputTransport_->write((uint8_t*)&net, 8);
  return 8;
}
  
uint32_t TBinaryProtocol::writeDouble(const double dub) {
  uint8_t b[8];
  uint8_t* d = (uint8_t*)&dub;
  b[0] = d[7];
  b[1] = d[6];
  b[2] = d[5];
  b[3] = d[4];
  b[4] = d[3];
  b[5] = d[2];
  b[6] = d[1];
  b[7] = d[0];
  outputTransport_->write((uint8_t*)b, 8);
  return 8;
}

  
uint32_t TBinaryProtocol::writeString(const string& str) {
  uint32_t result = writeI32(str.size());
  outputTransport_->write((uint8_t*)str.data(), str.size());
  return result + str.size();
}

/**
 * Reading functions
 */

uint32_t TBinaryProtocol::readMessageBegin(std::string& name,
					   TMessageType& messageType,
					   int32_t& seqid) {

  uint32_t result = 0;
  int8_t type;
  result+= readString(name);
  result+=  readByte(type);
  messageType = (TMessageType)type;
  result+= readI32(seqid);
  return result;
}

uint32_t TBinaryProtocol::readMessageEnd() {
  return 0;
}

uint32_t TBinaryProtocol::readStructBegin(string& name) {
  name = "";
  return 0;
}

uint32_t TBinaryProtocol::readStructEnd() {
  return 0;
}

uint32_t TBinaryProtocol::readFieldBegin(string& name,
                                         TType& fieldType,
                                         int16_t& fieldId) {
  uint32_t result = 0;
  int8_t type;
  result += readByte(type);
  fieldType = (TType)type;
  if (fieldType == T_STOP) {
    fieldId = 0;
    return result;
  }
  result += readI16(fieldId);
  return result;
}
  
uint32_t TBinaryProtocol::readFieldEnd() {
  return 0;
}
 
uint32_t TBinaryProtocol::readMapBegin(TType& keyType,
                                       TType& valType,
                                       uint32_t& size) {
  int8_t k, v;
  uint32_t result = 0;
  int32_t sizei;
  result += readByte(k);
  keyType = (TType)k;
  result += readByte(v);
  valType = (TType)v;
  result += readI32(sizei);
  // TODO(mcslee): check for negative size
  size = (uint32_t)sizei;
  return result;
}

uint32_t TBinaryProtocol::readMapEnd() {
  return 0;
}

uint32_t TBinaryProtocol::readListBegin(TType& elemType,
                                        uint32_t& size) {
  int8_t e;
  uint32_t result = 0;
  int32_t sizei;
  result += readByte(e);
  elemType = (TType)e;
  result += readI32(sizei);
  // TODO(mcslee): check for negative size
  size = (uint32_t)sizei;
  return result;
}

uint32_t TBinaryProtocol::readListEnd() {
  return 0;
}

uint32_t TBinaryProtocol::readSetBegin(TType& elemType,
                                       uint32_t& size) {
  int8_t e;
  uint32_t result = 0;
  int32_t sizei;
  result += readByte(e);
  elemType = (TType)e;
  result += readI32(sizei);
  // TODO(mcslee): check for negative size
  size = (uint32_t)sizei;
  return result;
}

uint32_t TBinaryProtocol::readSetEnd() {
  return 0;
}

uint32_t TBinaryProtocol::readBool(bool& value) {
  uint8_t b[1];
  inputTransport_->readAll(b, 1);
  value = *(int8_t*)b != 0;
  return 1;
}

uint32_t TBinaryProtocol::readByte(int8_t& byte) {
  uint8_t b[1];
  inputTransport_->readAll(b, 1);
  byte = *(int8_t*)b;
  return 1;
}

uint32_t TBinaryProtocol::readI16(int16_t& i16) {
  uint8_t b[2];
  inputTransport_->readAll(b, 2);
  i16 = *(int16_t*)b;
  i16 = (int16_t)ntohs(i16);
  return 2;
}

uint32_t TBinaryProtocol::readI32(int32_t& i32) {
  uint8_t b[4];
  inputTransport_->readAll(b, 4);
  i32 = *(int32_t*)b;
  i32 = (int32_t)ntohl(i32);
  return 4;
}

uint32_t TBinaryProtocol::readI64(int64_t& i64) {
  uint8_t b[8];
  inputTransport_->readAll(b, 8);
  i64 = *(int64_t*)b;
  i64 = (int64_t)ntohll(i64);
  return 8;
}

uint32_t TBinaryProtocol::readDouble(double& dub) {
  uint8_t b[8];
  uint8_t d[8];
  inputTransport_->readAll(b, 8);
  d[0] = b[7];
  d[1] = b[6];
  d[2] = b[5];
  d[3] = b[4];
  d[4] = b[3];
  d[5] = b[2];
  d[6] = b[1];
  d[7] = b[0];
  dub = *(double*)d;
  return 8;
}

uint32_t TBinaryProtocol::readString(string& str) {
  uint32_t result;
  int32_t size;
  result = readI32(size);

  // TODO(mcslee): check for negative size

  // Use the heap here to prevent stack overflow for v. large strings
  uint8_t *b = new uint8_t[size];
  inputTransport_->readAll(b, size);
  str = string((char*)b, size);
  delete [] b;

  return result + (uint32_t)size;
}

}}} // facebook::thrift::protocol
