// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: login.proto

#ifndef PROTOBUF_login_2eproto__INCLUDED
#define PROTOBUF_login_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace Login {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_login_2eproto();
void protobuf_AssignDesc_login_2eproto();
void protobuf_ShutdownFile_login_2eproto();

class Login;

enum LoginType {
  LoginTypeCode = 0,
  LoginTypePhone = 1,
  LoginType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  LoginType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool LoginType_IsValid(int value);
const LoginType LoginType_MIN = LoginTypeCode;
const LoginType LoginType_MAX = LoginTypePhone;
const int LoginType_ARRAYSIZE = LoginType_MAX + 1;

const ::google::protobuf::EnumDescriptor* LoginType_descriptor();
inline const ::std::string& LoginType_Name(LoginType value) {
  return ::google::protobuf::internal::NameOfEnum(
    LoginType_descriptor(), value);
}
inline bool LoginType_Parse(
    const ::std::string& name, LoginType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<LoginType>(
    LoginType_descriptor(), name, value);
}
// ===================================================================

class Login : public ::google::protobuf::Message {
 public:
  Login();
  virtual ~Login();

  Login(const Login& from);

  inline Login& operator=(const Login& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Login& default_instance();

  void Swap(Login* other);

  // implements Message ----------------------------------------------

  inline Login* New() const { return New(NULL); }

  Login* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Login& from);
  void MergeFrom(const Login& from);
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
  void InternalSwap(Login* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional uint32 login_type = 1;
  void clear_login_type();
  static const int kLoginTypeFieldNumber = 1;
  ::google::protobuf::uint32 login_type() const;
  void set_login_type(::google::protobuf::uint32 value);

  // optional uint64 user = 2;
  void clear_user();
  static const int kUserFieldNumber = 2;
  ::google::protobuf::uint64 user() const;
  void set_user(::google::protobuf::uint64 value);

  // optional bytes password = 3;
  void clear_password();
  static const int kPasswordFieldNumber = 3;
  const ::std::string& password() const;
  void set_password(const ::std::string& value);
  void set_password(const char* value);
  void set_password(const void* value, size_t size);
  ::std::string* mutable_password();
  ::std::string* release_password();
  void set_allocated_password(::std::string* password);

  // @@protoc_insertion_point(class_scope:Login.Login)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::uint64 user_;
  ::google::protobuf::internal::ArenaStringPtr password_;
  ::google::protobuf::uint32 login_type_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_login_2eproto();
  friend void protobuf_AssignDesc_login_2eproto();
  friend void protobuf_ShutdownFile_login_2eproto();

  void InitAsDefaultInstance();
  static Login* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Login

// optional uint32 login_type = 1;
inline void Login::clear_login_type() {
  login_type_ = 0u;
}
inline ::google::protobuf::uint32 Login::login_type() const {
  // @@protoc_insertion_point(field_get:Login.Login.login_type)
  return login_type_;
}
inline void Login::set_login_type(::google::protobuf::uint32 value) {
  
  login_type_ = value;
  // @@protoc_insertion_point(field_set:Login.Login.login_type)
}

// optional uint64 user = 2;
inline void Login::clear_user() {
  user_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 Login::user() const {
  // @@protoc_insertion_point(field_get:Login.Login.user)
  return user_;
}
inline void Login::set_user(::google::protobuf::uint64 value) {
  
  user_ = value;
  // @@protoc_insertion_point(field_set:Login.Login.user)
}

// optional bytes password = 3;
inline void Login::clear_password() {
  password_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Login::password() const {
  // @@protoc_insertion_point(field_get:Login.Login.password)
  return password_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Login::set_password(const ::std::string& value) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Login.Login.password)
}
inline void Login::set_password(const char* value) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Login.Login.password)
}
inline void Login::set_password(const void* value, size_t size) {
  
  password_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Login.Login.password)
}
inline ::std::string* Login::mutable_password() {
  
  // @@protoc_insertion_point(field_mutable:Login.Login.password)
  return password_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Login::release_password() {
  
  return password_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Login::set_allocated_password(::std::string* password) {
  if (password != NULL) {
    
  } else {
    
  }
  password_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), password);
  // @@protoc_insertion_point(field_set_allocated:Login.Login.password)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace Login

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::Login::LoginType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Login::LoginType>() {
  return ::Login::LoginType_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_login_2eproto__INCLUDED