// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: OpenStack.GlanceServer.proto

#ifndef PROTOBUF_OpenStack_2eGlanceServer_2eproto__INCLUDED
#define PROTOBUF_OpenStack_2eGlanceServer_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "OS.Network.pb.h"
// @@protoc_insertion_point(includes)

namespace cloudos {
namespace config {
namespace openstack {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_OpenStack_2eGlanceServer_2eproto();
void protobuf_AssignDesc_OpenStack_2eGlanceServer_2eproto();
void protobuf_ShutdownFile_OpenStack_2eGlanceServer_2eproto();

class GlanceServer;

// ===================================================================

class GlanceServer : public ::google::protobuf::Message {
 public:
  GlanceServer();
  virtual ~GlanceServer();
  
  GlanceServer(const GlanceServer& from);
  
  inline GlanceServer& operator=(const GlanceServer& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const GlanceServer& default_instance();
  
  void Swap(GlanceServer* other);
  
  // implements Message ----------------------------------------------
  
  GlanceServer* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GlanceServer& from);
  void MergeFrom(const GlanceServer& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // optional string host = 2;
  inline bool has_host() const;
  inline void clear_host();
  static const int kHostFieldNumber = 2;
  inline const ::std::string& host() const;
  inline void set_host(const ::std::string& value);
  inline void set_host(const char* value);
  inline void set_host(const char* value, size_t size);
  inline ::std::string* mutable_host();
  inline ::std::string* release_host();
  
  // optional uint32 port = 4;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 4;
  inline ::google::protobuf::uint32 port() const;
  inline void set_port(::google::protobuf::uint32 value);
  
  // optional uint32 workers = 6;
  inline bool has_workers() const;
  inline void clear_workers();
  static const int kWorkersFieldNumber = 6;
  inline ::google::protobuf::uint32 workers() const;
  inline void set_workers(::google::protobuf::uint32 value);
  
  // optional string registry_host = 8;
  inline bool has_registry_host() const;
  inline void clear_registry_host();
  static const int kRegistryHostFieldNumber = 8;
  inline const ::std::string& registry_host() const;
  inline void set_registry_host(const ::std::string& value);
  inline void set_registry_host(const char* value);
  inline void set_registry_host(const char* value, size_t size);
  inline ::std::string* mutable_registry_host();
  inline ::std::string* release_registry_host();
  
  // optional uint32 registry_port = 10;
  inline bool has_registry_port() const;
  inline void clear_registry_port();
  static const int kRegistryPortFieldNumber = 10;
  inline ::google::protobuf::uint32 registry_port() const;
  inline void set_registry_port(::google::protobuf::uint32 value);
  
  // optional .cloudos.config.os.WebProtocol registry_client_protocol = 12;
  inline bool has_registry_client_protocol() const;
  inline void clear_registry_client_protocol();
  static const int kRegistryClientProtocolFieldNumber = 12;
  inline const ::cloudos::config::os::WebProtocol& registry_client_protocol() const;
  inline ::cloudos::config::os::WebProtocol* mutable_registry_client_protocol();
  inline ::cloudos::config::os::WebProtocol* release_registry_client_protocol();
  
  // optional .cloudos.config.os.SSLSettings ssl_settings = 14;
  inline bool has_ssl_settings() const;
  inline void clear_ssl_settings();
  static const int kSslSettingsFieldNumber = 14;
  inline const ::cloudos::config::os::SSLSettings& ssl_settings() const;
  inline ::cloudos::config::os::SSLSettings* mutable_ssl_settings();
  inline ::cloudos::config::os::SSLSettings* release_ssl_settings();
  
  // @@protoc_insertion_point(class_scope:cloudos.config.openstack.GlanceServer)
 private:
  inline void set_has_host();
  inline void clear_has_host();
  inline void set_has_port();
  inline void clear_has_port();
  inline void set_has_workers();
  inline void clear_has_workers();
  inline void set_has_registry_host();
  inline void clear_has_registry_host();
  inline void set_has_registry_port();
  inline void clear_has_registry_port();
  inline void set_has_registry_client_protocol();
  inline void clear_has_registry_client_protocol();
  inline void set_has_ssl_settings();
  inline void clear_has_ssl_settings();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* host_;
  ::google::protobuf::uint32 port_;
  ::google::protobuf::uint32 workers_;
  ::std::string* registry_host_;
  ::cloudos::config::os::WebProtocol* registry_client_protocol_;
  ::cloudos::config::os::SSLSettings* ssl_settings_;
  ::google::protobuf::uint32 registry_port_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(7 + 31) / 32];
  
  friend void  protobuf_AddDesc_OpenStack_2eGlanceServer_2eproto();
  friend void protobuf_AssignDesc_OpenStack_2eGlanceServer_2eproto();
  friend void protobuf_ShutdownFile_OpenStack_2eGlanceServer_2eproto();
  
  void InitAsDefaultInstance();
  static GlanceServer* default_instance_;
};
// ===================================================================


// ===================================================================

// GlanceServer

// optional string host = 2;
inline bool GlanceServer::has_host() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GlanceServer::set_has_host() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GlanceServer::clear_has_host() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GlanceServer::clear_host() {
  if (host_ != &::google::protobuf::internal::kEmptyString) {
    host_->clear();
  }
  clear_has_host();
}
inline const ::std::string& GlanceServer::host() const {
  return *host_;
}
inline void GlanceServer::set_host(const ::std::string& value) {
  set_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    host_ = new ::std::string;
  }
  host_->assign(value);
}
inline void GlanceServer::set_host(const char* value) {
  set_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    host_ = new ::std::string;
  }
  host_->assign(value);
}
inline void GlanceServer::set_host(const char* value, size_t size) {
  set_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    host_ = new ::std::string;
  }
  host_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* GlanceServer::mutable_host() {
  set_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    host_ = new ::std::string;
  }
  return host_;
}
inline ::std::string* GlanceServer::release_host() {
  clear_has_host();
  if (host_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = host_;
    host_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional uint32 port = 4;
inline bool GlanceServer::has_port() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GlanceServer::set_has_port() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GlanceServer::clear_has_port() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GlanceServer::clear_port() {
  port_ = 0u;
  clear_has_port();
}
inline ::google::protobuf::uint32 GlanceServer::port() const {
  return port_;
}
inline void GlanceServer::set_port(::google::protobuf::uint32 value) {
  set_has_port();
  port_ = value;
}

// optional uint32 workers = 6;
inline bool GlanceServer::has_workers() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GlanceServer::set_has_workers() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GlanceServer::clear_has_workers() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GlanceServer::clear_workers() {
  workers_ = 0u;
  clear_has_workers();
}
inline ::google::protobuf::uint32 GlanceServer::workers() const {
  return workers_;
}
inline void GlanceServer::set_workers(::google::protobuf::uint32 value) {
  set_has_workers();
  workers_ = value;
}

// optional string registry_host = 8;
inline bool GlanceServer::has_registry_host() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void GlanceServer::set_has_registry_host() {
  _has_bits_[0] |= 0x00000008u;
}
inline void GlanceServer::clear_has_registry_host() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void GlanceServer::clear_registry_host() {
  if (registry_host_ != &::google::protobuf::internal::kEmptyString) {
    registry_host_->clear();
  }
  clear_has_registry_host();
}
inline const ::std::string& GlanceServer::registry_host() const {
  return *registry_host_;
}
inline void GlanceServer::set_registry_host(const ::std::string& value) {
  set_has_registry_host();
  if (registry_host_ == &::google::protobuf::internal::kEmptyString) {
    registry_host_ = new ::std::string;
  }
  registry_host_->assign(value);
}
inline void GlanceServer::set_registry_host(const char* value) {
  set_has_registry_host();
  if (registry_host_ == &::google::protobuf::internal::kEmptyString) {
    registry_host_ = new ::std::string;
  }
  registry_host_->assign(value);
}
inline void GlanceServer::set_registry_host(const char* value, size_t size) {
  set_has_registry_host();
  if (registry_host_ == &::google::protobuf::internal::kEmptyString) {
    registry_host_ = new ::std::string;
  }
  registry_host_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* GlanceServer::mutable_registry_host() {
  set_has_registry_host();
  if (registry_host_ == &::google::protobuf::internal::kEmptyString) {
    registry_host_ = new ::std::string;
  }
  return registry_host_;
}
inline ::std::string* GlanceServer::release_registry_host() {
  clear_has_registry_host();
  if (registry_host_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = registry_host_;
    registry_host_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional uint32 registry_port = 10;
inline bool GlanceServer::has_registry_port() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void GlanceServer::set_has_registry_port() {
  _has_bits_[0] |= 0x00000010u;
}
inline void GlanceServer::clear_has_registry_port() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void GlanceServer::clear_registry_port() {
  registry_port_ = 0u;
  clear_has_registry_port();
}
inline ::google::protobuf::uint32 GlanceServer::registry_port() const {
  return registry_port_;
}
inline void GlanceServer::set_registry_port(::google::protobuf::uint32 value) {
  set_has_registry_port();
  registry_port_ = value;
}

// optional .cloudos.config.os.WebProtocol registry_client_protocol = 12;
inline bool GlanceServer::has_registry_client_protocol() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void GlanceServer::set_has_registry_client_protocol() {
  _has_bits_[0] |= 0x00000020u;
}
inline void GlanceServer::clear_has_registry_client_protocol() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void GlanceServer::clear_registry_client_protocol() {
  if (registry_client_protocol_ != NULL) registry_client_protocol_->::cloudos::config::os::WebProtocol::Clear();
  clear_has_registry_client_protocol();
}
inline const ::cloudos::config::os::WebProtocol& GlanceServer::registry_client_protocol() const {
  return registry_client_protocol_ != NULL ? *registry_client_protocol_ : *default_instance_->registry_client_protocol_;
}
inline ::cloudos::config::os::WebProtocol* GlanceServer::mutable_registry_client_protocol() {
  set_has_registry_client_protocol();
  if (registry_client_protocol_ == NULL) registry_client_protocol_ = new ::cloudos::config::os::WebProtocol;
  return registry_client_protocol_;
}
inline ::cloudos::config::os::WebProtocol* GlanceServer::release_registry_client_protocol() {
  clear_has_registry_client_protocol();
  ::cloudos::config::os::WebProtocol* temp = registry_client_protocol_;
  registry_client_protocol_ = NULL;
  return temp;
}

// optional .cloudos.config.os.SSLSettings ssl_settings = 14;
inline bool GlanceServer::has_ssl_settings() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void GlanceServer::set_has_ssl_settings() {
  _has_bits_[0] |= 0x00000040u;
}
inline void GlanceServer::clear_has_ssl_settings() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void GlanceServer::clear_ssl_settings() {
  if (ssl_settings_ != NULL) ssl_settings_->::cloudos::config::os::SSLSettings::Clear();
  clear_has_ssl_settings();
}
inline const ::cloudos::config::os::SSLSettings& GlanceServer::ssl_settings() const {
  return ssl_settings_ != NULL ? *ssl_settings_ : *default_instance_->ssl_settings_;
}
inline ::cloudos::config::os::SSLSettings* GlanceServer::mutable_ssl_settings() {
  set_has_ssl_settings();
  if (ssl_settings_ == NULL) ssl_settings_ = new ::cloudos::config::os::SSLSettings;
  return ssl_settings_;
}
inline ::cloudos::config::os::SSLSettings* GlanceServer::release_ssl_settings() {
  clear_has_ssl_settings();
  ::cloudos::config::os::SSLSettings* temp = ssl_settings_;
  ssl_settings_ = NULL;
  return temp;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace openstack
}  // namespace config
}  // namespace cloudos

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_OpenStack_2eGlanceServer_2eproto__INCLUDED
