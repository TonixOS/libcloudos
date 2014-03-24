// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: OS.Installer.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "OS.Installer.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace cloudos {
namespace config {
namespace os {

namespace {

const ::google::protobuf::Descriptor* Installer_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  Installer_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_OS_2eInstaller_2eproto() {
  protobuf_AddDesc_OS_2eInstaller_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "OS.Installer.proto");
  GOOGLE_CHECK(file != NULL);
  Installer_descriptor_ = file->message_type(0);
  static const int Installer_offsets_[8] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, install_management_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, install_mariadb_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, install_rabbitmq_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, install_keystone_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, install_quantum_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, install_glance_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, install_cinder_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, install_nova_),
  };
  Installer_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      Installer_descriptor_,
      Installer::default_instance_,
      Installer_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Installer, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(Installer));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_OS_2eInstaller_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    Installer_descriptor_, &Installer::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_OS_2eInstaller_2eproto() {
  delete Installer::default_instance_;
  delete Installer_reflection_;
}

void protobuf_AddDesc_OS_2eInstaller_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022OS.Installer.proto\022\021cloudos.config.os\""
    "\213\002\n\tInstaller\022!\n\022install_management\030\024 \001("
    "\010:\005false\022\036\n\017install_mariadb\030\031 \001(\010:\005false"
    "\022\037\n\020install_rabbitmq\030\036 \001(\010:\005false\022\037\n\020ins"
    "tall_keystone\030( \001(\010:\005false\022\036\n\017install_qu"
    "antum\030- \001(\010:\005false\022\035\n\016install_glance\0302 \001"
    "(\010:\005false\022\035\n\016install_cinder\0307 \001(\010:\005false"
    "\022\033\n\014install_nova\030< \001(\010:\005false", 309);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "OS.Installer.proto", &protobuf_RegisterTypes);
  Installer::default_instance_ = new Installer();
  Installer::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_OS_2eInstaller_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_OS_2eInstaller_2eproto {
  StaticDescriptorInitializer_OS_2eInstaller_2eproto() {
    protobuf_AddDesc_OS_2eInstaller_2eproto();
  }
} static_descriptor_initializer_OS_2eInstaller_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int Installer::kInstallManagementFieldNumber;
const int Installer::kInstallMariadbFieldNumber;
const int Installer::kInstallRabbitmqFieldNumber;
const int Installer::kInstallKeystoneFieldNumber;
const int Installer::kInstallQuantumFieldNumber;
const int Installer::kInstallGlanceFieldNumber;
const int Installer::kInstallCinderFieldNumber;
const int Installer::kInstallNovaFieldNumber;
#endif  // !_MSC_VER

Installer::Installer()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void Installer::InitAsDefaultInstance() {
}

Installer::Installer(const Installer& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void Installer::SharedCtor() {
  _cached_size_ = 0;
  install_management_ = false;
  install_mariadb_ = false;
  install_rabbitmq_ = false;
  install_keystone_ = false;
  install_quantum_ = false;
  install_glance_ = false;
  install_cinder_ = false;
  install_nova_ = false;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

Installer::~Installer() {
  SharedDtor();
}

void Installer::SharedDtor() {
  if (this != default_instance_) {
  }
}

void Installer::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Installer::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Installer_descriptor_;
}

const Installer& Installer::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_OS_2eInstaller_2eproto();
  return *default_instance_;
}

Installer* Installer::default_instance_ = NULL;

Installer* Installer::New() const {
  return new Installer;
}

void Installer::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    install_management_ = false;
    install_mariadb_ = false;
    install_rabbitmq_ = false;
    install_keystone_ = false;
    install_quantum_ = false;
    install_glance_ = false;
    install_cinder_ = false;
    install_nova_ = false;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool Installer::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional bool install_management = 20 [default = false];
      case 20: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &install_management_)));
          set_has_install_management();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(200)) goto parse_install_mariadb;
        break;
      }

      // optional bool install_mariadb = 25 [default = false];
      case 25: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_install_mariadb:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &install_mariadb_)));
          set_has_install_mariadb();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(240)) goto parse_install_rabbitmq;
        break;
      }

      // optional bool install_rabbitmq = 30 [default = false];
      case 30: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_install_rabbitmq:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &install_rabbitmq_)));
          set_has_install_rabbitmq();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(320)) goto parse_install_keystone;
        break;
      }

      // optional bool install_keystone = 40 [default = false];
      case 40: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_install_keystone:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &install_keystone_)));
          set_has_install_keystone();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(360)) goto parse_install_quantum;
        break;
      }

      // optional bool install_quantum = 45 [default = false];
      case 45: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_install_quantum:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &install_quantum_)));
          set_has_install_quantum();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(400)) goto parse_install_glance;
        break;
      }

      // optional bool install_glance = 50 [default = false];
      case 50: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_install_glance:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &install_glance_)));
          set_has_install_glance();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(440)) goto parse_install_cinder;
        break;
      }

      // optional bool install_cinder = 55 [default = false];
      case 55: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_install_cinder:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &install_cinder_)));
          set_has_install_cinder();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(480)) goto parse_install_nova;
        break;
      }

      // optional bool install_nova = 60 [default = false];
      case 60: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_install_nova:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &install_nova_)));
          set_has_install_nova();
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

void Installer::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional bool install_management = 20 [default = false];
  if (has_install_management()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(20, this->install_management(), output);
  }

  // optional bool install_mariadb = 25 [default = false];
  if (has_install_mariadb()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(25, this->install_mariadb(), output);
  }

  // optional bool install_rabbitmq = 30 [default = false];
  if (has_install_rabbitmq()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(30, this->install_rabbitmq(), output);
  }

  // optional bool install_keystone = 40 [default = false];
  if (has_install_keystone()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(40, this->install_keystone(), output);
  }

  // optional bool install_quantum = 45 [default = false];
  if (has_install_quantum()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(45, this->install_quantum(), output);
  }

  // optional bool install_glance = 50 [default = false];
  if (has_install_glance()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(50, this->install_glance(), output);
  }

  // optional bool install_cinder = 55 [default = false];
  if (has_install_cinder()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(55, this->install_cinder(), output);
  }

  // optional bool install_nova = 60 [default = false];
  if (has_install_nova()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(60, this->install_nova(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* Installer::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional bool install_management = 20 [default = false];
  if (has_install_management()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(20, this->install_management(), target);
  }

  // optional bool install_mariadb = 25 [default = false];
  if (has_install_mariadb()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(25, this->install_mariadb(), target);
  }

  // optional bool install_rabbitmq = 30 [default = false];
  if (has_install_rabbitmq()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(30, this->install_rabbitmq(), target);
  }

  // optional bool install_keystone = 40 [default = false];
  if (has_install_keystone()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(40, this->install_keystone(), target);
  }

  // optional bool install_quantum = 45 [default = false];
  if (has_install_quantum()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(45, this->install_quantum(), target);
  }

  // optional bool install_glance = 50 [default = false];
  if (has_install_glance()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(50, this->install_glance(), target);
  }

  // optional bool install_cinder = 55 [default = false];
  if (has_install_cinder()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(55, this->install_cinder(), target);
  }

  // optional bool install_nova = 60 [default = false];
  if (has_install_nova()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(60, this->install_nova(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int Installer::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional bool install_management = 20 [default = false];
    if (has_install_management()) {
      total_size += 2 + 1;
    }

    // optional bool install_mariadb = 25 [default = false];
    if (has_install_mariadb()) {
      total_size += 2 + 1;
    }

    // optional bool install_rabbitmq = 30 [default = false];
    if (has_install_rabbitmq()) {
      total_size += 2 + 1;
    }

    // optional bool install_keystone = 40 [default = false];
    if (has_install_keystone()) {
      total_size += 2 + 1;
    }

    // optional bool install_quantum = 45 [default = false];
    if (has_install_quantum()) {
      total_size += 2 + 1;
    }

    // optional bool install_glance = 50 [default = false];
    if (has_install_glance()) {
      total_size += 2 + 1;
    }

    // optional bool install_cinder = 55 [default = false];
    if (has_install_cinder()) {
      total_size += 2 + 1;
    }

    // optional bool install_nova = 60 [default = false];
    if (has_install_nova()) {
      total_size += 2 + 1;
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

void Installer::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const Installer* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const Installer*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void Installer::MergeFrom(const Installer& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_install_management()) {
      set_install_management(from.install_management());
    }
    if (from.has_install_mariadb()) {
      set_install_mariadb(from.install_mariadb());
    }
    if (from.has_install_rabbitmq()) {
      set_install_rabbitmq(from.install_rabbitmq());
    }
    if (from.has_install_keystone()) {
      set_install_keystone(from.install_keystone());
    }
    if (from.has_install_quantum()) {
      set_install_quantum(from.install_quantum());
    }
    if (from.has_install_glance()) {
      set_install_glance(from.install_glance());
    }
    if (from.has_install_cinder()) {
      set_install_cinder(from.install_cinder());
    }
    if (from.has_install_nova()) {
      set_install_nova(from.install_nova());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void Installer::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Installer::CopyFrom(const Installer& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Installer::IsInitialized() const {

  return true;
}

void Installer::Swap(Installer* other) {
  if (other != this) {
    std::swap(install_management_, other->install_management_);
    std::swap(install_mariadb_, other->install_mariadb_);
    std::swap(install_rabbitmq_, other->install_rabbitmq_);
    std::swap(install_keystone_, other->install_keystone_);
    std::swap(install_quantum_, other->install_quantum_);
    std::swap(install_glance_, other->install_glance_);
    std::swap(install_cinder_, other->install_cinder_);
    std::swap(install_nova_, other->install_nova_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata Installer::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = Installer_descriptor_;
  metadata.reflection = Installer_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace os
}  // namespace config
}  // namespace cloudos

// @@protoc_insertion_point(global_scope)
