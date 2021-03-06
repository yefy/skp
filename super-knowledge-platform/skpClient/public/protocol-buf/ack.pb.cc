// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ack.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ack.pb.h"

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

namespace Ack {

namespace {

const ::google::protobuf::Descriptor* MessageAck_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MessageAck_reflection_ = NULL;
const ::google::protobuf::Descriptor* MessageAckList_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MessageAckList_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_ack_2eproto() {
  protobuf_AddDesc_ack_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ack.proto");
  GOOGLE_CHECK(file != NULL);
  MessageAck_descriptor_ = file->message_type(0);
  static const int MessageAck_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageAck, class__),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageAck, friend__),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageAck, passage_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageAck, from_),
  };
  MessageAck_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      MessageAck_descriptor_,
      MessageAck::default_instance_,
      MessageAck_offsets_,
      -1,
      -1,
      -1,
      sizeof(MessageAck),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageAck, _internal_metadata_),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageAck, _is_default_instance_));
  MessageAckList_descriptor_ = file->message_type(1);
  static const int MessageAckList_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageAckList, ack_),
  };
  MessageAckList_reflection_ =
    ::google::protobuf::internal::GeneratedMessageReflection::NewGeneratedMessageReflection(
      MessageAckList_descriptor_,
      MessageAckList::default_instance_,
      MessageAckList_offsets_,
      -1,
      -1,
      -1,
      sizeof(MessageAckList),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageAckList, _internal_metadata_),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessageAckList, _is_default_instance_));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ack_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      MessageAck_descriptor_, &MessageAck::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
      MessageAckList_descriptor_, &MessageAckList::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ack_2eproto() {
  delete MessageAck::default_instance_;
  delete MessageAck_reflection_;
  delete MessageAckList::default_instance_;
  delete MessageAckList_reflection_;
}

void protobuf_AddDesc_ack_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\tack.proto\022\003Ack\"J\n\nMessageAck\022\r\n\005class\030"
    "\001 \001(\r\022\016\n\006friend\030\002 \001(\004\022\017\n\007passage\030\003 \001(\r\022\014"
    "\n\004from\030\004 \001(\004\".\n\016MessageAckList\022\034\n\003ack\030\001 "
    "\003(\0132\017.Ack.MessageAckb\006proto3", 148);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ack.proto", &protobuf_RegisterTypes);
  MessageAck::default_instance_ = new MessageAck();
  MessageAckList::default_instance_ = new MessageAckList();
  MessageAck::default_instance_->InitAsDefaultInstance();
  MessageAckList::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ack_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ack_2eproto {
  StaticDescriptorInitializer_ack_2eproto() {
    protobuf_AddDesc_ack_2eproto();
  }
} static_descriptor_initializer_ack_2eproto_;

namespace {

static void MergeFromFail(int line) GOOGLE_ATTRIBUTE_COLD;
static void MergeFromFail(int line) {
  GOOGLE_CHECK(false) << __FILE__ << ":" << line;
}

}  // namespace


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int MessageAck::kClassFieldNumber;
const int MessageAck::kFriendFieldNumber;
const int MessageAck::kPassageFieldNumber;
const int MessageAck::kFromFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

MessageAck::MessageAck()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:Ack.MessageAck)
}

void MessageAck::InitAsDefaultInstance() {
  _is_default_instance_ = true;
}

MessageAck::MessageAck(const MessageAck& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:Ack.MessageAck)
}

void MessageAck::SharedCtor() {
    _is_default_instance_ = false;
  _cached_size_ = 0;
  class__ = 0u;
  friend__ = GOOGLE_ULONGLONG(0);
  passage_ = 0u;
  from_ = GOOGLE_ULONGLONG(0);
}

MessageAck::~MessageAck() {
  // @@protoc_insertion_point(destructor:Ack.MessageAck)
  SharedDtor();
}

void MessageAck::SharedDtor() {
  if (this != default_instance_) {
  }
}

void MessageAck::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MessageAck::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MessageAck_descriptor_;
}

const MessageAck& MessageAck::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ack_2eproto();
  return *default_instance_;
}

MessageAck* MessageAck::default_instance_ = NULL;

MessageAck* MessageAck::New(::google::protobuf::Arena* arena) const {
  MessageAck* n = new MessageAck;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void MessageAck::Clear() {
#define ZR_HELPER_(f) reinterpret_cast<char*>(\
  &reinterpret_cast<MessageAck*>(16)->f)

#define ZR_(first, last) do {\
  ::memset(&first, 0,\
           ZR_HELPER_(last) - ZR_HELPER_(first) + sizeof(last));\
} while (0)

  ZR_(friend__, from_);

#undef ZR_HELPER_
#undef ZR_

}

bool MessageAck::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Ack.MessageAck)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional uint32 class = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &class__)));

        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_friend;
        break;
      }

      // optional uint64 friend = 2;
      case 2: {
        if (tag == 16) {
         parse_friend:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &friend__)));

        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_passage;
        break;
      }

      // optional uint32 passage = 3;
      case 3: {
        if (tag == 24) {
         parse_passage:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &passage_)));

        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(32)) goto parse_from;
        break;
      }

      // optional uint64 from = 4;
      case 4: {
        if (tag == 32) {
         parse_from:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &from_)));

        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Ack.MessageAck)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Ack.MessageAck)
  return false;
#undef DO_
}

void MessageAck::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Ack.MessageAck)
  // optional uint32 class = 1;
  if (this->class_() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->class_(), output);
  }

  // optional uint64 friend = 2;
  if (this->friend_() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(2, this->friend_(), output);
  }

  // optional uint32 passage = 3;
  if (this->passage() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->passage(), output);
  }

  // optional uint64 from = 4;
  if (this->from() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(4, this->from(), output);
  }

  // @@protoc_insertion_point(serialize_end:Ack.MessageAck)
}

::google::protobuf::uint8* MessageAck::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:Ack.MessageAck)
  // optional uint32 class = 1;
  if (this->class_() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->class_(), target);
  }

  // optional uint64 friend = 2;
  if (this->friend_() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(2, this->friend_(), target);
  }

  // optional uint32 passage = 3;
  if (this->passage() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->passage(), target);
  }

  // optional uint64 from = 4;
  if (this->from() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(4, this->from(), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:Ack.MessageAck)
  return target;
}

int MessageAck::ByteSize() const {
  int total_size = 0;

  // optional uint32 class = 1;
  if (this->class_() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->class_());
  }

  // optional uint64 friend = 2;
  if (this->friend_() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt64Size(
        this->friend_());
  }

  // optional uint32 passage = 3;
  if (this->passage() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->passage());
  }

  // optional uint64 from = 4;
  if (this->from() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt64Size(
        this->from());
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void MessageAck::MergeFrom(const ::google::protobuf::Message& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const MessageAck* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const MessageAck>(
          &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MessageAck::MergeFrom(const MessageAck& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  if (from.class_() != 0) {
    set_class_(from.class_());
  }
  if (from.friend_() != 0) {
    set_friend_(from.friend_());
  }
  if (from.passage() != 0) {
    set_passage(from.passage());
  }
  if (from.from() != 0) {
    set_from(from.from());
  }
}

void MessageAck::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MessageAck::CopyFrom(const MessageAck& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MessageAck::IsInitialized() const {

  return true;
}

void MessageAck::Swap(MessageAck* other) {
  if (other == this) return;
  InternalSwap(other);
}
void MessageAck::InternalSwap(MessageAck* other) {
  std::swap(class__, other->class__);
  std::swap(friend__, other->friend__);
  std::swap(passage_, other->passage_);
  std::swap(from_, other->from_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata MessageAck::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MessageAck_descriptor_;
  metadata.reflection = MessageAck_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// MessageAck

// optional uint32 class = 1;
void MessageAck::clear_class_() {
  class__ = 0u;
}
 ::google::protobuf::uint32 MessageAck::class_() const {
  // @@protoc_insertion_point(field_get:Ack.MessageAck.class)
  return class__;
}
 void MessageAck::set_class_(::google::protobuf::uint32 value) {
  
  class__ = value;
  // @@protoc_insertion_point(field_set:Ack.MessageAck.class)
}

// optional uint64 friend = 2;
void MessageAck::clear_friend_() {
  friend__ = GOOGLE_ULONGLONG(0);
}
 ::google::protobuf::uint64 MessageAck::friend_() const {
  // @@protoc_insertion_point(field_get:Ack.MessageAck.friend)
  return friend__;
}
 void MessageAck::set_friend_(::google::protobuf::uint64 value) {
  
  friend__ = value;
  // @@protoc_insertion_point(field_set:Ack.MessageAck.friend)
}

// optional uint32 passage = 3;
void MessageAck::clear_passage() {
  passage_ = 0u;
}
 ::google::protobuf::uint32 MessageAck::passage() const {
  // @@protoc_insertion_point(field_get:Ack.MessageAck.passage)
  return passage_;
}
 void MessageAck::set_passage(::google::protobuf::uint32 value) {
  
  passage_ = value;
  // @@protoc_insertion_point(field_set:Ack.MessageAck.passage)
}

// optional uint64 from = 4;
void MessageAck::clear_from() {
  from_ = GOOGLE_ULONGLONG(0);
}
 ::google::protobuf::uint64 MessageAck::from() const {
  // @@protoc_insertion_point(field_get:Ack.MessageAck.from)
  return from_;
}
 void MessageAck::set_from(::google::protobuf::uint64 value) {
  
  from_ = value;
  // @@protoc_insertion_point(field_set:Ack.MessageAck.from)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int MessageAckList::kAckFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

MessageAckList::MessageAckList()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:Ack.MessageAckList)
}

void MessageAckList::InitAsDefaultInstance() {
  _is_default_instance_ = true;
}

MessageAckList::MessageAckList(const MessageAckList& from)
  : ::google::protobuf::Message(),
    _internal_metadata_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:Ack.MessageAckList)
}

void MessageAckList::SharedCtor() {
    _is_default_instance_ = false;
  _cached_size_ = 0;
}

MessageAckList::~MessageAckList() {
  // @@protoc_insertion_point(destructor:Ack.MessageAckList)
  SharedDtor();
}

void MessageAckList::SharedDtor() {
  if (this != default_instance_) {
  }
}

void MessageAckList::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MessageAckList::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MessageAckList_descriptor_;
}

const MessageAckList& MessageAckList::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ack_2eproto();
  return *default_instance_;
}

MessageAckList* MessageAckList::default_instance_ = NULL;

MessageAckList* MessageAckList::New(::google::protobuf::Arena* arena) const {
  MessageAckList* n = new MessageAckList;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void MessageAckList::Clear() {
  ack_.Clear();
}

bool MessageAckList::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:Ack.MessageAckList)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .Ack.MessageAck ack = 1;
      case 1: {
        if (tag == 10) {
          DO_(input->IncrementRecursionDepth());
         parse_loop_ack:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtualNoRecursionDepth(
                input, add_ack()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(10)) goto parse_loop_ack;
        input->UnsafeDecrementRecursionDepth();
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:Ack.MessageAckList)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:Ack.MessageAckList)
  return false;
#undef DO_
}

void MessageAckList::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:Ack.MessageAckList)
  // repeated .Ack.MessageAck ack = 1;
  for (unsigned int i = 0, n = this->ack_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->ack(i), output);
  }

  // @@protoc_insertion_point(serialize_end:Ack.MessageAckList)
}

::google::protobuf::uint8* MessageAckList::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:Ack.MessageAckList)
  // repeated .Ack.MessageAck ack = 1;
  for (unsigned int i = 0, n = this->ack_size(); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->ack(i), target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:Ack.MessageAckList)
  return target;
}

int MessageAckList::ByteSize() const {
  int total_size = 0;

  // repeated .Ack.MessageAck ack = 1;
  total_size += 1 * this->ack_size();
  for (int i = 0; i < this->ack_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->ack(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void MessageAckList::MergeFrom(const ::google::protobuf::Message& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  const MessageAckList* source = 
      ::google::protobuf::internal::DynamicCastToGenerated<const MessageAckList>(
          &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MessageAckList::MergeFrom(const MessageAckList& from) {
  if (GOOGLE_PREDICT_FALSE(&from == this)) MergeFromFail(__LINE__);
  ack_.MergeFrom(from.ack_);
}

void MessageAckList::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MessageAckList::CopyFrom(const MessageAckList& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MessageAckList::IsInitialized() const {

  return true;
}

void MessageAckList::Swap(MessageAckList* other) {
  if (other == this) return;
  InternalSwap(other);
}
void MessageAckList::InternalSwap(MessageAckList* other) {
  ack_.UnsafeArenaSwap(&other->ack_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata MessageAckList::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MessageAckList_descriptor_;
  metadata.reflection = MessageAckList_reflection_;
  return metadata;
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// MessageAckList

// repeated .Ack.MessageAck ack = 1;
int MessageAckList::ack_size() const {
  return ack_.size();
}
void MessageAckList::clear_ack() {
  ack_.Clear();
}
const ::Ack::MessageAck& MessageAckList::ack(int index) const {
  // @@protoc_insertion_point(field_get:Ack.MessageAckList.ack)
  return ack_.Get(index);
}
::Ack::MessageAck* MessageAckList::mutable_ack(int index) {
  // @@protoc_insertion_point(field_mutable:Ack.MessageAckList.ack)
  return ack_.Mutable(index);
}
::Ack::MessageAck* MessageAckList::add_ack() {
  // @@protoc_insertion_point(field_add:Ack.MessageAckList.ack)
  return ack_.Add();
}
::google::protobuf::RepeatedPtrField< ::Ack::MessageAck >*
MessageAckList::mutable_ack() {
  // @@protoc_insertion_point(field_mutable_list:Ack.MessageAckList.ack)
  return &ack_;
}
const ::google::protobuf::RepeatedPtrField< ::Ack::MessageAck >&
MessageAckList::ack() const {
  // @@protoc_insertion_point(field_list:Ack.MessageAckList.ack)
  return ack_;
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace Ack

// @@protoc_insertion_point(global_scope)
