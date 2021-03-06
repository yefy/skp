// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: memory_file.proto

#ifndef PROTOBUF_memory_5ffile_2eproto__INCLUDED
#define PROTOBUF_memory_5ffile_2eproto__INCLUDED

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

namespace MemoryFile {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_memory_5ffile_2eproto();
void protobuf_AssignDesc_memory_5ffile_2eproto();
void protobuf_ShutdownFile_memory_5ffile_2eproto();

class MemoryFileData;
class MemoryFileDataList;
class MemoryFileMD5;
class MemoryFileToFriend;

enum FileDataStatus {
  FileDataStatusNull = 0,
  FileDataStatusPushing = 1,
  FileDataStatusFinish = 2,
  FileDataStatus_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  FileDataStatus_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool FileDataStatus_IsValid(int value);
const FileDataStatus FileDataStatus_MIN = FileDataStatusNull;
const FileDataStatus FileDataStatus_MAX = FileDataStatusFinish;
const int FileDataStatus_ARRAYSIZE = FileDataStatus_MAX + 1;

const ::google::protobuf::EnumDescriptor* FileDataStatus_descriptor();
inline const ::std::string& FileDataStatus_Name(FileDataStatus value) {
  return ::google::protobuf::internal::NameOfEnum(
    FileDataStatus_descriptor(), value);
}
inline bool FileDataStatus_Parse(
    const ::std::string& name, FileDataStatus* value) {
  return ::google::protobuf::internal::ParseNamedEnum<FileDataStatus>(
    FileDataStatus_descriptor(), name, value);
}
// ===================================================================

class MemoryFileMD5 : public ::google::protobuf::Message {
 public:
  MemoryFileMD5();
  virtual ~MemoryFileMD5();

  MemoryFileMD5(const MemoryFileMD5& from);

  inline MemoryFileMD5& operator=(const MemoryFileMD5& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const MemoryFileMD5& default_instance();

  void Swap(MemoryFileMD5* other);

  // implements Message ----------------------------------------------

  inline MemoryFileMD5* New() const { return New(NULL); }

  MemoryFileMD5* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MemoryFileMD5& from);
  void MergeFrom(const MemoryFileMD5& from);
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
  void InternalSwap(MemoryFileMD5* other);
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

  // optional bytes md5 = 1;
  void clear_md5();
  static const int kMd5FieldNumber = 1;
  const ::std::string& md5() const;
  void set_md5(const ::std::string& value);
  void set_md5(const char* value);
  void set_md5(const void* value, size_t size);
  ::std::string* mutable_md5();
  ::std::string* release_md5();
  void set_allocated_md5(::std::string* md5);

  // @@protoc_insertion_point(class_scope:MemoryFile.MemoryFileMD5)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr md5_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_memory_5ffile_2eproto();
  friend void protobuf_AssignDesc_memory_5ffile_2eproto();
  friend void protobuf_ShutdownFile_memory_5ffile_2eproto();

  void InitAsDefaultInstance();
  static MemoryFileMD5* default_instance_;
};
// -------------------------------------------------------------------

class MemoryFileData : public ::google::protobuf::Message {
 public:
  MemoryFileData();
  virtual ~MemoryFileData();

  MemoryFileData(const MemoryFileData& from);

  inline MemoryFileData& operator=(const MemoryFileData& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const MemoryFileData& default_instance();

  void Swap(MemoryFileData* other);

  // implements Message ----------------------------------------------

  inline MemoryFileData* New() const { return New(NULL); }

  MemoryFileData* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MemoryFileData& from);
  void MergeFrom(const MemoryFileData& from);
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
  void InternalSwap(MemoryFileData* other);
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

  // optional bytes md5 = 1;
  void clear_md5();
  static const int kMd5FieldNumber = 1;
  const ::std::string& md5() const;
  void set_md5(const ::std::string& value);
  void set_md5(const char* value);
  void set_md5(const void* value, size_t size);
  ::std::string* mutable_md5();
  ::std::string* release_md5();
  void set_allocated_md5(::std::string* md5);

  // optional bytes name = 2;
  void clear_name();
  static const int kNameFieldNumber = 2;
  const ::std::string& name() const;
  void set_name(const ::std::string& value);
  void set_name(const char* value);
  void set_name(const void* value, size_t size);
  ::std::string* mutable_name();
  ::std::string* release_name();
  void set_allocated_name(::std::string* name);

  // optional uint64 lenth = 3;
  void clear_lenth();
  static const int kLenthFieldNumber = 3;
  ::google::protobuf::uint64 lenth() const;
  void set_lenth(::google::protobuf::uint64 value);

  // optional uint64 offset = 4;
  void clear_offset();
  static const int kOffsetFieldNumber = 4;
  ::google::protobuf::uint64 offset() const;
  void set_offset(::google::protobuf::uint64 value);

  // optional uint32 status = 5;
  void clear_status();
  static const int kStatusFieldNumber = 5;
  ::google::protobuf::uint32 status() const;
  void set_status(::google::protobuf::uint32 value);

  // optional bytes buffer = 6;
  void clear_buffer();
  static const int kBufferFieldNumber = 6;
  const ::std::string& buffer() const;
  void set_buffer(const ::std::string& value);
  void set_buffer(const char* value);
  void set_buffer(const void* value, size_t size);
  ::std::string* mutable_buffer();
  ::std::string* release_buffer();
  void set_allocated_buffer(::std::string* buffer);

  // optional uint64 friend = 7;
  void clear_friend_();
  static const int kFriendFieldNumber = 7;
  ::google::protobuf::uint64 friend_() const;
  void set_friend_(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:MemoryFile.MemoryFileData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr md5_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  ::google::protobuf::uint64 lenth_;
  ::google::protobuf::uint64 offset_;
  ::google::protobuf::internal::ArenaStringPtr buffer_;
  ::google::protobuf::uint64 friend__;
  ::google::protobuf::uint32 status_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_memory_5ffile_2eproto();
  friend void protobuf_AssignDesc_memory_5ffile_2eproto();
  friend void protobuf_ShutdownFile_memory_5ffile_2eproto();

  void InitAsDefaultInstance();
  static MemoryFileData* default_instance_;
};
// -------------------------------------------------------------------

class MemoryFileDataList : public ::google::protobuf::Message {
 public:
  MemoryFileDataList();
  virtual ~MemoryFileDataList();

  MemoryFileDataList(const MemoryFileDataList& from);

  inline MemoryFileDataList& operator=(const MemoryFileDataList& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const MemoryFileDataList& default_instance();

  void Swap(MemoryFileDataList* other);

  // implements Message ----------------------------------------------

  inline MemoryFileDataList* New() const { return New(NULL); }

  MemoryFileDataList* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MemoryFileDataList& from);
  void MergeFrom(const MemoryFileDataList& from);
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
  void InternalSwap(MemoryFileDataList* other);
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

  // repeated .MemoryFile.MemoryFileData file_data = 1;
  int file_data_size() const;
  void clear_file_data();
  static const int kFileDataFieldNumber = 1;
  const ::MemoryFile::MemoryFileData& file_data(int index) const;
  ::MemoryFile::MemoryFileData* mutable_file_data(int index);
  ::MemoryFile::MemoryFileData* add_file_data();
  ::google::protobuf::RepeatedPtrField< ::MemoryFile::MemoryFileData >*
      mutable_file_data();
  const ::google::protobuf::RepeatedPtrField< ::MemoryFile::MemoryFileData >&
      file_data() const;

  // @@protoc_insertion_point(class_scope:MemoryFile.MemoryFileDataList)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::RepeatedPtrField< ::MemoryFile::MemoryFileData > file_data_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_memory_5ffile_2eproto();
  friend void protobuf_AssignDesc_memory_5ffile_2eproto();
  friend void protobuf_ShutdownFile_memory_5ffile_2eproto();

  void InitAsDefaultInstance();
  static MemoryFileDataList* default_instance_;
};
// -------------------------------------------------------------------

class MemoryFileToFriend : public ::google::protobuf::Message {
 public:
  MemoryFileToFriend();
  virtual ~MemoryFileToFriend();

  MemoryFileToFriend(const MemoryFileToFriend& from);

  inline MemoryFileToFriend& operator=(const MemoryFileToFriend& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const MemoryFileToFriend& default_instance();

  void Swap(MemoryFileToFriend* other);

  // implements Message ----------------------------------------------

  inline MemoryFileToFriend* New() const { return New(NULL); }

  MemoryFileToFriend* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MemoryFileToFriend& from);
  void MergeFrom(const MemoryFileToFriend& from);
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
  void InternalSwap(MemoryFileToFriend* other);
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

  // optional bytes md5 = 1;
  void clear_md5();
  static const int kMd5FieldNumber = 1;
  const ::std::string& md5() const;
  void set_md5(const ::std::string& value);
  void set_md5(const char* value);
  void set_md5(const void* value, size_t size);
  ::std::string* mutable_md5();
  ::std::string* release_md5();
  void set_allocated_md5(::std::string* md5);

  // optional bytes name = 2;
  void clear_name();
  static const int kNameFieldNumber = 2;
  const ::std::string& name() const;
  void set_name(const ::std::string& value);
  void set_name(const char* value);
  void set_name(const void* value, size_t size);
  ::std::string* mutable_name();
  ::std::string* release_name();
  void set_allocated_name(::std::string* name);

  // optional uint64 lenth = 3;
  void clear_lenth();
  static const int kLenthFieldNumber = 3;
  ::google::protobuf::uint64 lenth() const;
  void set_lenth(::google::protobuf::uint64 value);

  // optional uint64 friend = 4;
  void clear_friend_();
  static const int kFriendFieldNumber = 4;
  ::google::protobuf::uint64 friend_() const;
  void set_friend_(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:MemoryFile.MemoryFileToFriend)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr md5_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  ::google::protobuf::uint64 lenth_;
  ::google::protobuf::uint64 friend__;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_memory_5ffile_2eproto();
  friend void protobuf_AssignDesc_memory_5ffile_2eproto();
  friend void protobuf_ShutdownFile_memory_5ffile_2eproto();

  void InitAsDefaultInstance();
  static MemoryFileToFriend* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// MemoryFileMD5

// optional bytes md5 = 1;
inline void MemoryFileMD5::clear_md5() {
  md5_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& MemoryFileMD5::md5() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileMD5.md5)
  return md5_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileMD5::set_md5(const ::std::string& value) {
  
  md5_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileMD5.md5)
}
inline void MemoryFileMD5::set_md5(const char* value) {
  
  md5_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MemoryFile.MemoryFileMD5.md5)
}
inline void MemoryFileMD5::set_md5(const void* value, size_t size) {
  
  md5_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MemoryFile.MemoryFileMD5.md5)
}
inline ::std::string* MemoryFileMD5::mutable_md5() {
  
  // @@protoc_insertion_point(field_mutable:MemoryFile.MemoryFileMD5.md5)
  return md5_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* MemoryFileMD5::release_md5() {
  
  return md5_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileMD5::set_allocated_md5(::std::string* md5) {
  if (md5 != NULL) {
    
  } else {
    
  }
  md5_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), md5);
  // @@protoc_insertion_point(field_set_allocated:MemoryFile.MemoryFileMD5.md5)
}

// -------------------------------------------------------------------

// MemoryFileData

// optional bytes md5 = 1;
inline void MemoryFileData::clear_md5() {
  md5_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& MemoryFileData::md5() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileData.md5)
  return md5_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileData::set_md5(const ::std::string& value) {
  
  md5_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileData.md5)
}
inline void MemoryFileData::set_md5(const char* value) {
  
  md5_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MemoryFile.MemoryFileData.md5)
}
inline void MemoryFileData::set_md5(const void* value, size_t size) {
  
  md5_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MemoryFile.MemoryFileData.md5)
}
inline ::std::string* MemoryFileData::mutable_md5() {
  
  // @@protoc_insertion_point(field_mutable:MemoryFile.MemoryFileData.md5)
  return md5_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* MemoryFileData::release_md5() {
  
  return md5_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileData::set_allocated_md5(::std::string* md5) {
  if (md5 != NULL) {
    
  } else {
    
  }
  md5_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), md5);
  // @@protoc_insertion_point(field_set_allocated:MemoryFile.MemoryFileData.md5)
}

// optional bytes name = 2;
inline void MemoryFileData::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& MemoryFileData::name() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileData.name)
  return name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileData::set_name(const ::std::string& value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileData.name)
}
inline void MemoryFileData::set_name(const char* value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MemoryFile.MemoryFileData.name)
}
inline void MemoryFileData::set_name(const void* value, size_t size) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MemoryFile.MemoryFileData.name)
}
inline ::std::string* MemoryFileData::mutable_name() {
  
  // @@protoc_insertion_point(field_mutable:MemoryFile.MemoryFileData.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* MemoryFileData::release_name() {
  
  return name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileData::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:MemoryFile.MemoryFileData.name)
}

// optional uint64 lenth = 3;
inline void MemoryFileData::clear_lenth() {
  lenth_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 MemoryFileData::lenth() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileData.lenth)
  return lenth_;
}
inline void MemoryFileData::set_lenth(::google::protobuf::uint64 value) {
  
  lenth_ = value;
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileData.lenth)
}

// optional uint64 offset = 4;
inline void MemoryFileData::clear_offset() {
  offset_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 MemoryFileData::offset() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileData.offset)
  return offset_;
}
inline void MemoryFileData::set_offset(::google::protobuf::uint64 value) {
  
  offset_ = value;
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileData.offset)
}

// optional uint32 status = 5;
inline void MemoryFileData::clear_status() {
  status_ = 0u;
}
inline ::google::protobuf::uint32 MemoryFileData::status() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileData.status)
  return status_;
}
inline void MemoryFileData::set_status(::google::protobuf::uint32 value) {
  
  status_ = value;
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileData.status)
}

// optional bytes buffer = 6;
inline void MemoryFileData::clear_buffer() {
  buffer_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& MemoryFileData::buffer() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileData.buffer)
  return buffer_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileData::set_buffer(const ::std::string& value) {
  
  buffer_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileData.buffer)
}
inline void MemoryFileData::set_buffer(const char* value) {
  
  buffer_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MemoryFile.MemoryFileData.buffer)
}
inline void MemoryFileData::set_buffer(const void* value, size_t size) {
  
  buffer_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MemoryFile.MemoryFileData.buffer)
}
inline ::std::string* MemoryFileData::mutable_buffer() {
  
  // @@protoc_insertion_point(field_mutable:MemoryFile.MemoryFileData.buffer)
  return buffer_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* MemoryFileData::release_buffer() {
  
  return buffer_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileData::set_allocated_buffer(::std::string* buffer) {
  if (buffer != NULL) {
    
  } else {
    
  }
  buffer_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), buffer);
  // @@protoc_insertion_point(field_set_allocated:MemoryFile.MemoryFileData.buffer)
}

// optional uint64 friend = 7;
inline void MemoryFileData::clear_friend_() {
  friend__ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 MemoryFileData::friend_() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileData.friend)
  return friend__;
}
inline void MemoryFileData::set_friend_(::google::protobuf::uint64 value) {
  
  friend__ = value;
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileData.friend)
}

// -------------------------------------------------------------------

// MemoryFileDataList

// repeated .MemoryFile.MemoryFileData file_data = 1;
inline int MemoryFileDataList::file_data_size() const {
  return file_data_.size();
}
inline void MemoryFileDataList::clear_file_data() {
  file_data_.Clear();
}
inline const ::MemoryFile::MemoryFileData& MemoryFileDataList::file_data(int index) const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileDataList.file_data)
  return file_data_.Get(index);
}
inline ::MemoryFile::MemoryFileData* MemoryFileDataList::mutable_file_data(int index) {
  // @@protoc_insertion_point(field_mutable:MemoryFile.MemoryFileDataList.file_data)
  return file_data_.Mutable(index);
}
inline ::MemoryFile::MemoryFileData* MemoryFileDataList::add_file_data() {
  // @@protoc_insertion_point(field_add:MemoryFile.MemoryFileDataList.file_data)
  return file_data_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::MemoryFile::MemoryFileData >*
MemoryFileDataList::mutable_file_data() {
  // @@protoc_insertion_point(field_mutable_list:MemoryFile.MemoryFileDataList.file_data)
  return &file_data_;
}
inline const ::google::protobuf::RepeatedPtrField< ::MemoryFile::MemoryFileData >&
MemoryFileDataList::file_data() const {
  // @@protoc_insertion_point(field_list:MemoryFile.MemoryFileDataList.file_data)
  return file_data_;
}

// -------------------------------------------------------------------

// MemoryFileToFriend

// optional bytes md5 = 1;
inline void MemoryFileToFriend::clear_md5() {
  md5_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& MemoryFileToFriend::md5() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileToFriend.md5)
  return md5_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileToFriend::set_md5(const ::std::string& value) {
  
  md5_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileToFriend.md5)
}
inline void MemoryFileToFriend::set_md5(const char* value) {
  
  md5_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MemoryFile.MemoryFileToFriend.md5)
}
inline void MemoryFileToFriend::set_md5(const void* value, size_t size) {
  
  md5_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MemoryFile.MemoryFileToFriend.md5)
}
inline ::std::string* MemoryFileToFriend::mutable_md5() {
  
  // @@protoc_insertion_point(field_mutable:MemoryFile.MemoryFileToFriend.md5)
  return md5_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* MemoryFileToFriend::release_md5() {
  
  return md5_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileToFriend::set_allocated_md5(::std::string* md5) {
  if (md5 != NULL) {
    
  } else {
    
  }
  md5_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), md5);
  // @@protoc_insertion_point(field_set_allocated:MemoryFile.MemoryFileToFriend.md5)
}

// optional bytes name = 2;
inline void MemoryFileToFriend::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& MemoryFileToFriend::name() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileToFriend.name)
  return name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileToFriend::set_name(const ::std::string& value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileToFriend.name)
}
inline void MemoryFileToFriend::set_name(const char* value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MemoryFile.MemoryFileToFriend.name)
}
inline void MemoryFileToFriend::set_name(const void* value, size_t size) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MemoryFile.MemoryFileToFriend.name)
}
inline ::std::string* MemoryFileToFriend::mutable_name() {
  
  // @@protoc_insertion_point(field_mutable:MemoryFile.MemoryFileToFriend.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* MemoryFileToFriend::release_name() {
  
  return name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MemoryFileToFriend::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:MemoryFile.MemoryFileToFriend.name)
}

// optional uint64 lenth = 3;
inline void MemoryFileToFriend::clear_lenth() {
  lenth_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 MemoryFileToFriend::lenth() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileToFriend.lenth)
  return lenth_;
}
inline void MemoryFileToFriend::set_lenth(::google::protobuf::uint64 value) {
  
  lenth_ = value;
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileToFriend.lenth)
}

// optional uint64 friend = 4;
inline void MemoryFileToFriend::clear_friend_() {
  friend__ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 MemoryFileToFriend::friend_() const {
  // @@protoc_insertion_point(field_get:MemoryFile.MemoryFileToFriend.friend)
  return friend__;
}
inline void MemoryFileToFriend::set_friend_(::google::protobuf::uint64 value) {
  
  friend__ = value;
  // @@protoc_insertion_point(field_set:MemoryFile.MemoryFileToFriend.friend)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace MemoryFile

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::MemoryFile::FileDataStatus> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::MemoryFile::FileDataStatus>() {
  return ::MemoryFile::FileDataStatus_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_memory_5ffile_2eproto__INCLUDED
