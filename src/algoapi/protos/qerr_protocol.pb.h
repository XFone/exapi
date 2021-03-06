// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protos/qerr_protocol.proto

#ifndef PROTOBUF_INCLUDED_protos_2fqerr_5fprotocol_2eproto
#define PROTOBUF_INCLUDED_protos_2fqerr_5fprotocol_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_protos_2fqerr_5fprotocol_2eproto 

namespace protobuf_protos_2fqerr_5fprotocol_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_protos_2fqerr_5fprotocol_2eproto
namespace autotrade {
class ReqtErr;
class ReqtErrDefaultTypeInternal;
extern ReqtErrDefaultTypeInternal _ReqtErr_default_instance_;
class ReqtOK;
class ReqtOKDefaultTypeInternal;
extern ReqtOKDefaultTypeInternal _ReqtOK_default_instance_;
}  // namespace autotrade
namespace google {
namespace protobuf {
template<> ::autotrade::ReqtErr* Arena::CreateMaybeMessage<::autotrade::ReqtErr>(Arena*);
template<> ::autotrade::ReqtOK* Arena::CreateMaybeMessage<::autotrade::ReqtOK>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace autotrade {

// ===================================================================

class ReqtOK : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:autotrade.ReqtOK) */ {
 public:
  ReqtOK();
  virtual ~ReqtOK();

  ReqtOK(const ReqtOK& from);

  inline ReqtOK& operator=(const ReqtOK& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ReqtOK(ReqtOK&& from) noexcept
    : ReqtOK() {
    *this = ::std::move(from);
  }

  inline ReqtOK& operator=(ReqtOK&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ReqtOK& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ReqtOK* internal_default_instance() {
    return reinterpret_cast<const ReqtOK*>(
               &_ReqtOK_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(ReqtOK* other);
  friend void swap(ReqtOK& a, ReqtOK& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ReqtOK* New() const final {
    return CreateMaybeMessage<ReqtOK>(NULL);
  }

  ReqtOK* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<ReqtOK>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const ReqtOK& from);
  void MergeFrom(const ReqtOK& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ReqtOK* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string ret_message = 3;
  bool has_ret_message() const;
  void clear_ret_message();
  static const int kRetMessageFieldNumber = 3;
  const ::std::string& ret_message() const;
  void set_ret_message(const ::std::string& value);
  #if LANG_CXX11
  void set_ret_message(::std::string&& value);
  #endif
  void set_ret_message(const char* value);
  void set_ret_message(const char* value, size_t size);
  ::std::string* mutable_ret_message();
  ::std::string* release_ret_message();
  void set_allocated_ret_message(::std::string* ret_message);

  // optional string request_id = 4;
  bool has_request_id() const;
  void clear_request_id();
  static const int kRequestIdFieldNumber = 4;
  const ::std::string& request_id() const;
  void set_request_id(const ::std::string& value);
  #if LANG_CXX11
  void set_request_id(::std::string&& value);
  #endif
  void set_request_id(const char* value);
  void set_request_id(const char* value, size_t size);
  ::std::string* mutable_request_id();
  ::std::string* release_request_id();
  void set_allocated_request_id(::std::string* request_id);

  // required int32 ret_code = 2;
  bool has_ret_code() const;
  void clear_ret_code();
  static const int kRetCodeFieldNumber = 2;
  ::google::protobuf::int32 ret_code() const;
  void set_ret_code(::google::protobuf::int32 value);

  // optional int32 funcid = 1 [default = 1];
  bool has_funcid() const;
  void clear_funcid();
  static const int kFuncidFieldNumber = 1;
  ::google::protobuf::int32 funcid() const;
  void set_funcid(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:autotrade.ReqtOK)
 private:
  void set_has_funcid();
  void clear_has_funcid();
  void set_has_ret_code();
  void clear_has_ret_code();
  void set_has_ret_message();
  void clear_has_ret_message();
  void set_has_request_id();
  void clear_has_request_id();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr ret_message_;
  ::google::protobuf::internal::ArenaStringPtr request_id_;
  ::google::protobuf::int32 ret_code_;
  ::google::protobuf::int32 funcid_;
  friend struct ::protobuf_protos_2fqerr_5fprotocol_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class ReqtErr : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:autotrade.ReqtErr) */ {
 public:
  ReqtErr();
  virtual ~ReqtErr();

  ReqtErr(const ReqtErr& from);

  inline ReqtErr& operator=(const ReqtErr& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ReqtErr(ReqtErr&& from) noexcept
    : ReqtErr() {
    *this = ::std::move(from);
  }

  inline ReqtErr& operator=(ReqtErr&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ReqtErr& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ReqtErr* internal_default_instance() {
    return reinterpret_cast<const ReqtErr*>(
               &_ReqtErr_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(ReqtErr* other);
  friend void swap(ReqtErr& a, ReqtErr& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ReqtErr* New() const final {
    return CreateMaybeMessage<ReqtErr>(NULL);
  }

  ReqtErr* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<ReqtErr>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const ReqtErr& from);
  void MergeFrom(const ReqtErr& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ReqtErr* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string ret_message = 3;
  bool has_ret_message() const;
  void clear_ret_message();
  static const int kRetMessageFieldNumber = 3;
  const ::std::string& ret_message() const;
  void set_ret_message(const ::std::string& value);
  #if LANG_CXX11
  void set_ret_message(::std::string&& value);
  #endif
  void set_ret_message(const char* value);
  void set_ret_message(const char* value, size_t size);
  ::std::string* mutable_ret_message();
  ::std::string* release_ret_message();
  void set_allocated_ret_message(::std::string* ret_message);

  // optional string request_id = 4;
  bool has_request_id() const;
  void clear_request_id();
  static const int kRequestIdFieldNumber = 4;
  const ::std::string& request_id() const;
  void set_request_id(const ::std::string& value);
  #if LANG_CXX11
  void set_request_id(::std::string&& value);
  #endif
  void set_request_id(const char* value);
  void set_request_id(const char* value, size_t size);
  ::std::string* mutable_request_id();
  ::std::string* release_request_id();
  void set_allocated_request_id(::std::string* request_id);

  // required int32 ret_code = 2;
  bool has_ret_code() const;
  void clear_ret_code();
  static const int kRetCodeFieldNumber = 2;
  ::google::protobuf::int32 ret_code() const;
  void set_ret_code(::google::protobuf::int32 value);

  // optional int32 funcid = 1 [default = -1];
  bool has_funcid() const;
  void clear_funcid();
  static const int kFuncidFieldNumber = 1;
  ::google::protobuf::int32 funcid() const;
  void set_funcid(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:autotrade.ReqtErr)
 private:
  void set_has_funcid();
  void clear_has_funcid();
  void set_has_ret_code();
  void clear_has_ret_code();
  void set_has_ret_message();
  void clear_has_ret_message();
  void set_has_request_id();
  void clear_has_request_id();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr ret_message_;
  ::google::protobuf::internal::ArenaStringPtr request_id_;
  ::google::protobuf::int32 ret_code_;
  ::google::protobuf::int32 funcid_;
  friend struct ::protobuf_protos_2fqerr_5fprotocol_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ReqtOK

// optional int32 funcid = 1 [default = 1];
inline bool ReqtOK::has_funcid() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ReqtOK::set_has_funcid() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ReqtOK::clear_has_funcid() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ReqtOK::clear_funcid() {
  funcid_ = 1;
  clear_has_funcid();
}
inline ::google::protobuf::int32 ReqtOK::funcid() const {
  // @@protoc_insertion_point(field_get:autotrade.ReqtOK.funcid)
  return funcid_;
}
inline void ReqtOK::set_funcid(::google::protobuf::int32 value) {
  set_has_funcid();
  funcid_ = value;
  // @@protoc_insertion_point(field_set:autotrade.ReqtOK.funcid)
}

// required int32 ret_code = 2;
inline bool ReqtOK::has_ret_code() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ReqtOK::set_has_ret_code() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ReqtOK::clear_has_ret_code() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ReqtOK::clear_ret_code() {
  ret_code_ = 0;
  clear_has_ret_code();
}
inline ::google::protobuf::int32 ReqtOK::ret_code() const {
  // @@protoc_insertion_point(field_get:autotrade.ReqtOK.ret_code)
  return ret_code_;
}
inline void ReqtOK::set_ret_code(::google::protobuf::int32 value) {
  set_has_ret_code();
  ret_code_ = value;
  // @@protoc_insertion_point(field_set:autotrade.ReqtOK.ret_code)
}

// optional string ret_message = 3;
inline bool ReqtOK::has_ret_message() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqtOK::set_has_ret_message() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqtOK::clear_has_ret_message() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqtOK::clear_ret_message() {
  ret_message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_ret_message();
}
inline const ::std::string& ReqtOK::ret_message() const {
  // @@protoc_insertion_point(field_get:autotrade.ReqtOK.ret_message)
  return ret_message_.GetNoArena();
}
inline void ReqtOK::set_ret_message(const ::std::string& value) {
  set_has_ret_message();
  ret_message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:autotrade.ReqtOK.ret_message)
}
#if LANG_CXX11
inline void ReqtOK::set_ret_message(::std::string&& value) {
  set_has_ret_message();
  ret_message_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:autotrade.ReqtOK.ret_message)
}
#endif
inline void ReqtOK::set_ret_message(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_ret_message();
  ret_message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:autotrade.ReqtOK.ret_message)
}
inline void ReqtOK::set_ret_message(const char* value, size_t size) {
  set_has_ret_message();
  ret_message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:autotrade.ReqtOK.ret_message)
}
inline ::std::string* ReqtOK::mutable_ret_message() {
  set_has_ret_message();
  // @@protoc_insertion_point(field_mutable:autotrade.ReqtOK.ret_message)
  return ret_message_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ReqtOK::release_ret_message() {
  // @@protoc_insertion_point(field_release:autotrade.ReqtOK.ret_message)
  if (!has_ret_message()) {
    return NULL;
  }
  clear_has_ret_message();
  return ret_message_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ReqtOK::set_allocated_ret_message(::std::string* ret_message) {
  if (ret_message != NULL) {
    set_has_ret_message();
  } else {
    clear_has_ret_message();
  }
  ret_message_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ret_message);
  // @@protoc_insertion_point(field_set_allocated:autotrade.ReqtOK.ret_message)
}

// optional string request_id = 4;
inline bool ReqtOK::has_request_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReqtOK::set_has_request_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReqtOK::clear_has_request_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReqtOK::clear_request_id() {
  request_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_request_id();
}
inline const ::std::string& ReqtOK::request_id() const {
  // @@protoc_insertion_point(field_get:autotrade.ReqtOK.request_id)
  return request_id_.GetNoArena();
}
inline void ReqtOK::set_request_id(const ::std::string& value) {
  set_has_request_id();
  request_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:autotrade.ReqtOK.request_id)
}
#if LANG_CXX11
inline void ReqtOK::set_request_id(::std::string&& value) {
  set_has_request_id();
  request_id_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:autotrade.ReqtOK.request_id)
}
#endif
inline void ReqtOK::set_request_id(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_request_id();
  request_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:autotrade.ReqtOK.request_id)
}
inline void ReqtOK::set_request_id(const char* value, size_t size) {
  set_has_request_id();
  request_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:autotrade.ReqtOK.request_id)
}
inline ::std::string* ReqtOK::mutable_request_id() {
  set_has_request_id();
  // @@protoc_insertion_point(field_mutable:autotrade.ReqtOK.request_id)
  return request_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ReqtOK::release_request_id() {
  // @@protoc_insertion_point(field_release:autotrade.ReqtOK.request_id)
  if (!has_request_id()) {
    return NULL;
  }
  clear_has_request_id();
  return request_id_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ReqtOK::set_allocated_request_id(::std::string* request_id) {
  if (request_id != NULL) {
    set_has_request_id();
  } else {
    clear_has_request_id();
  }
  request_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), request_id);
  // @@protoc_insertion_point(field_set_allocated:autotrade.ReqtOK.request_id)
}

// -------------------------------------------------------------------

// ReqtErr

// optional int32 funcid = 1 [default = -1];
inline bool ReqtErr::has_funcid() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ReqtErr::set_has_funcid() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ReqtErr::clear_has_funcid() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ReqtErr::clear_funcid() {
  funcid_ = -1;
  clear_has_funcid();
}
inline ::google::protobuf::int32 ReqtErr::funcid() const {
  // @@protoc_insertion_point(field_get:autotrade.ReqtErr.funcid)
  return funcid_;
}
inline void ReqtErr::set_funcid(::google::protobuf::int32 value) {
  set_has_funcid();
  funcid_ = value;
  // @@protoc_insertion_point(field_set:autotrade.ReqtErr.funcid)
}

// required int32 ret_code = 2;
inline bool ReqtErr::has_ret_code() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ReqtErr::set_has_ret_code() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ReqtErr::clear_has_ret_code() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ReqtErr::clear_ret_code() {
  ret_code_ = 0;
  clear_has_ret_code();
}
inline ::google::protobuf::int32 ReqtErr::ret_code() const {
  // @@protoc_insertion_point(field_get:autotrade.ReqtErr.ret_code)
  return ret_code_;
}
inline void ReqtErr::set_ret_code(::google::protobuf::int32 value) {
  set_has_ret_code();
  ret_code_ = value;
  // @@protoc_insertion_point(field_set:autotrade.ReqtErr.ret_code)
}

// optional string ret_message = 3;
inline bool ReqtErr::has_ret_message() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqtErr::set_has_ret_message() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqtErr::clear_has_ret_message() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqtErr::clear_ret_message() {
  ret_message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_ret_message();
}
inline const ::std::string& ReqtErr::ret_message() const {
  // @@protoc_insertion_point(field_get:autotrade.ReqtErr.ret_message)
  return ret_message_.GetNoArena();
}
inline void ReqtErr::set_ret_message(const ::std::string& value) {
  set_has_ret_message();
  ret_message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:autotrade.ReqtErr.ret_message)
}
#if LANG_CXX11
inline void ReqtErr::set_ret_message(::std::string&& value) {
  set_has_ret_message();
  ret_message_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:autotrade.ReqtErr.ret_message)
}
#endif
inline void ReqtErr::set_ret_message(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_ret_message();
  ret_message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:autotrade.ReqtErr.ret_message)
}
inline void ReqtErr::set_ret_message(const char* value, size_t size) {
  set_has_ret_message();
  ret_message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:autotrade.ReqtErr.ret_message)
}
inline ::std::string* ReqtErr::mutable_ret_message() {
  set_has_ret_message();
  // @@protoc_insertion_point(field_mutable:autotrade.ReqtErr.ret_message)
  return ret_message_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ReqtErr::release_ret_message() {
  // @@protoc_insertion_point(field_release:autotrade.ReqtErr.ret_message)
  if (!has_ret_message()) {
    return NULL;
  }
  clear_has_ret_message();
  return ret_message_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ReqtErr::set_allocated_ret_message(::std::string* ret_message) {
  if (ret_message != NULL) {
    set_has_ret_message();
  } else {
    clear_has_ret_message();
  }
  ret_message_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ret_message);
  // @@protoc_insertion_point(field_set_allocated:autotrade.ReqtErr.ret_message)
}

// optional string request_id = 4;
inline bool ReqtErr::has_request_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReqtErr::set_has_request_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReqtErr::clear_has_request_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReqtErr::clear_request_id() {
  request_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_request_id();
}
inline const ::std::string& ReqtErr::request_id() const {
  // @@protoc_insertion_point(field_get:autotrade.ReqtErr.request_id)
  return request_id_.GetNoArena();
}
inline void ReqtErr::set_request_id(const ::std::string& value) {
  set_has_request_id();
  request_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:autotrade.ReqtErr.request_id)
}
#if LANG_CXX11
inline void ReqtErr::set_request_id(::std::string&& value) {
  set_has_request_id();
  request_id_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:autotrade.ReqtErr.request_id)
}
#endif
inline void ReqtErr::set_request_id(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_request_id();
  request_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:autotrade.ReqtErr.request_id)
}
inline void ReqtErr::set_request_id(const char* value, size_t size) {
  set_has_request_id();
  request_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:autotrade.ReqtErr.request_id)
}
inline ::std::string* ReqtErr::mutable_request_id() {
  set_has_request_id();
  // @@protoc_insertion_point(field_mutable:autotrade.ReqtErr.request_id)
  return request_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ReqtErr::release_request_id() {
  // @@protoc_insertion_point(field_release:autotrade.ReqtErr.request_id)
  if (!has_request_id()) {
    return NULL;
  }
  clear_has_request_id();
  return request_id_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ReqtErr::set_allocated_request_id(::std::string* request_id) {
  if (request_id != NULL) {
    set_has_request_id();
  } else {
    clear_has_request_id();
  }
  request_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), request_id);
  // @@protoc_insertion_point(field_set_allocated:autotrade.ReqtErr.request_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace autotrade

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_protos_2fqerr_5fprotocol_2eproto
