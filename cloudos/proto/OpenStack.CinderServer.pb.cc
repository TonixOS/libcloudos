// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "OpenStack.CinderServer.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace cloudos {
namespace config {
namespace openstack {

namespace {

const ::google::protobuf::Descriptor* CinderServer_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CinderServer_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_OpenStack_2eCinderServer_2eproto() {
  protobuf_AddDesc_OpenStack_2eCinderServer_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "OpenStack.CinderServer.proto");
  GOOGLE_CHECK(file != NULL);
  CinderServer_descriptor_ = file->message_type(0);
  static const int CinderServer_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CinderServer, host_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CinderServer, port_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CinderServer, volume_group_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CinderServer, protocol_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CinderServer, ssl_settings_),
  };
  CinderServer_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      CinderServer_descriptor_,
      CinderServer::default_instance_,
      CinderServer_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CinderServer, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CinderServer, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(CinderServer));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_OpenStack_2eCinderServer_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    CinderServer_descriptor_, &CinderServer::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_OpenStack_2eCinderServer_2eproto() {
  delete CinderServer::default_instance_;
  delete CinderServer_reflection_;
}

void protobuf_AddDesc_OpenStack_2eCinderServer_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::cloudos::config::os::protobuf_AddDesc_OS_2eNetwork_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\034OpenStack.CinderServer.proto\022\030cloudos."
    "config.openstack\032\020OS.Network.proto\"\272\001\n\014C"
    "inderServer\022\014\n\004host\030\002 \001(\t\022\022\n\004port\030\004 \001(\r:"
    "\0048776\022 \n\014volume_group\030\010 \001(\t:\nvg_storage\022"
    "0\n\010protocol\030\014 \001(\0132\036.cloudos.config.os.We"
    "bProtocol\0224\n\014ssl_settings\030\016 \001(\0132\036.cloudo"
    "s.config.os.SSLSettings", 263);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "OpenStack.CinderServer.proto", &protobuf_RegisterTypes);
  CinderServer::default_instance_ = new CinderServer();
  CinderServer::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_OpenStack_2eCinderServer_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_OpenStack_2eCinderServer_2eproto {
  StaticDescriptorInitializer_OpenStack_2eCinderServer_2eproto() {
    protobuf_AddDesc_OpenStack_2eCinderServer_2eproto();
  }
} static_descriptor_initializer_OpenStack_2eCinderServer_2eproto_;


// ===================================================================

const ::std::string CinderServer::_default_volume_group_("vg_storage");
#ifndef _MSC_VER
const int CinderServer::kHostFieldNumber;
const int CinderServer::kPortFieldNumber;
const int CinderServer::kVolumeGroupFieldNumber;
const int CinderServer::kProtocolFieldNumber;
const int CinderServer::kSslSettingsFieldNumber;
#endif  // !_MSC_VER

CinderServer::CinderServer()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void CinderServer::InitAsDefaultInstance() {
  protocol_ = const_cast< ::cloudos::config::os::WebProtocol*>(&::cloudos::config::os::WebProtocol::default_instance());
  ssl_settings_ = const_cast< ::cloudos::config::os::SSLSettings*>(&::cloudos::config::os::SSLSettings::default_instance());
}

CinderServer::CinderServer(const CinderServer& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void CinderServer::SharedCtor() {
  _cached_size_ = 0;
  host_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  port_ = 8776u;
  volume_group_ = const_cast< ::std::string*>(&_default_volume_group_);
  protocol_ = NULL;
  ssl_settings_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CinderServer::~CinderServer() {
  SharedDtor();
}

void CinderServer::SharedDtor() {
  if (host_ != &::google::protobuf::internal::kEmptyString) {
    delete host_;
  }
  if (volume_group_ != &_default_volume_group_) {
    delete volume_group_;
  }
  if (this != default_instance_) {
    delete protocol_;
    delete ssl_settings_;
  }
}

void CinderServer::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CinderServer::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CinderServer_descriptor_;
}

const CinderServer& CinderServer::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_OpenStack_2eCinderServer_2eproto();  return *default_instance_;
}

CinderServer* CinderServer::default_instance_ = NULL;

CinderServer* CinderServer::New() const {
  return new CinderServer;
}

void CinderServer::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_host()) {
      if (host_ != &::google::protobuf::internal::kEmptyString) {
        host_->clear();
      }
    }
    port_ = 8776u;
    if (has_volume_group()) {
      if (volume_group_ != &_default_volume_group_) {
        volume_group_->assign(_default_volume_group_);
      }
    }
    if (has_protocol()) {
      if (protocol_ != NULL) protocol_->::cloudos::config::os::WebProtocol::Clear();
    }
    if (has_ssl_settings()) {
      if (ssl_settings_ != NULL) ssl_settings_->::cloudos::config::os::SSLSettings::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool CinderServer::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string host = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_host()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->host().data(), this->host().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_port;
        break;
      }
      
      // optional uint32 port = 4 [default = 8776];
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_port:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &port_)));
          set_has_port();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(66)) goto parse_volume_group;
        break;
      }
      
      // optional string volume_group = 8 [default = "vg_storage"];
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_volume_group:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_volume_group()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->volume_group().data(), this->volume_group().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(98)) goto parse_protocol;
        break;
      }
      
      // optional .cloudos.config.os.WebProtocol protocol = 12;
      case 12: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_protocol:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_protocol()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(114)) goto parse_ssl_settings;
        break;
      }
      
      // optional .cloudos.config.os.SSLSettings ssl_settings = 14;
      case 14: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_ssl_settings:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_ssl_settings()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void CinderServer::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string host = 2;
  if (has_host()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->host().data(), this->host().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->host(), output);
  }
  
  // optional uint32 port = 4 [default = 8776];
  if (has_port()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->port(), output);
  }
  
  // optional string volume_group = 8 [default = "vg_storage"];
  if (has_volume_group()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->volume_group().data(), this->volume_group().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      8, this->volume_group(), output);
  }
  
  // optional .cloudos.config.os.WebProtocol protocol = 12;
  if (has_protocol()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      12, this->protocol(), output);
  }
  
  // optional .cloudos.config.os.SSLSettings ssl_settings = 14;
  if (has_ssl_settings()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      14, this->ssl_settings(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* CinderServer::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string host = 2;
  if (has_host()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->host().data(), this->host().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->host(), target);
  }
  
  // optional uint32 port = 4 [default = 8776];
  if (has_port()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->port(), target);
  }
  
  // optional string volume_group = 8 [default = "vg_storage"];
  if (has_volume_group()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->volume_group().data(), this->volume_group().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        8, this->volume_group(), target);
  }
  
  // optional .cloudos.config.os.WebProtocol protocol = 12;
  if (has_protocol()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        12, this->protocol(), target);
  }
  
  // optional .cloudos.config.os.SSLSettings ssl_settings = 14;
  if (has_ssl_settings()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        14, this->ssl_settings(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int CinderServer::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string host = 2;
    if (has_host()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->host());
    }
    
    // optional uint32 port = 4 [default = 8776];
    if (has_port()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->port());
    }
    
    // optional string volume_group = 8 [default = "vg_storage"];
    if (has_volume_group()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->volume_group());
    }
    
    // optional .cloudos.config.os.WebProtocol protocol = 12;
    if (has_protocol()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->protocol());
    }
    
    // optional .cloudos.config.os.SSLSettings ssl_settings = 14;
    if (has_ssl_settings()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->ssl_settings());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CinderServer::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const CinderServer* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const CinderServer*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void CinderServer::MergeFrom(const CinderServer& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_host()) {
      set_host(from.host());
    }
    if (from.has_port()) {
      set_port(from.port());
    }
    if (from.has_volume_group()) {
      set_volume_group(from.volume_group());
    }
    if (from.has_protocol()) {
      mutable_protocol()->::cloudos::config::os::WebProtocol::MergeFrom(from.protocol());
    }
    if (from.has_ssl_settings()) {
      mutable_ssl_settings()->::cloudos::config::os::SSLSettings::MergeFrom(from.ssl_settings());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void CinderServer::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CinderServer::CopyFrom(const CinderServer& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CinderServer::IsInitialized() const {
  
  if (has_protocol()) {
    if (!this->protocol().IsInitialized()) return false;
  }
  return true;
}

void CinderServer::Swap(CinderServer* other) {
  if (other != this) {
    std::swap(host_, other->host_);
    std::swap(port_, other->port_);
    std::swap(volume_group_, other->volume_group_);
    std::swap(protocol_, other->protocol_);
    std::swap(ssl_settings_, other->ssl_settings_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata CinderServer::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CinderServer_descriptor_;
  metadata.reflection = CinderServer_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace openstack
}  // namespace config
}  // namespace cloudos

// @@protoc_insertion_point(global_scope)
