// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: api/common/v1/club_enum.proto

#ifndef PROTOBUF_api_2fcommon_2fv1_2fclub_5fenum_2eproto__INCLUDED
#define PROTOBUF_api_2fcommon_2fv1_2fclub_5fenum_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include "Define.h" // for TC_PROTO_API
// @@protoc_insertion_point(includes)

namespace bgs {
namespace protocol {
namespace club {
namespace v1 {

// Internal implementation detail -- do not call these.
void TC_PROTO_API protobuf_AddDesc_api_2fcommon_2fv1_2fclub_5fenum_2eproto();
void protobuf_AssignDesc_api_2fcommon_2fv1_2fclub_5fenum_2eproto();
void protobuf_ShutdownFile_api_2fcommon_2fv1_2fclub_5fenum_2eproto();

enum PrivacyLevel {
  PRIVACY_LEVEL_CLOSED = 0,
  PRIVACY_LEVEL_OPEN_INVITATION = 1,
  PRIVACY_LEVEL_OPEN_TICKET = 2,
  PRIVACY_LEVEL_OPEN = 3
};
TC_PROTO_API bool PrivacyLevel_IsValid(int value);
const PrivacyLevel PrivacyLevel_MIN = PRIVACY_LEVEL_CLOSED;
const PrivacyLevel PrivacyLevel_MAX = PRIVACY_LEVEL_OPEN;
const int PrivacyLevel_ARRAYSIZE = PrivacyLevel_MAX + 1;

TC_PROTO_API const ::google::protobuf::EnumDescriptor* PrivacyLevel_descriptor();
inline const ::std::string& PrivacyLevel_Name(PrivacyLevel value) {
  return ::google::protobuf::internal::NameOfEnum(
    PrivacyLevel_descriptor(), value);
}
inline bool PrivacyLevel_Parse(
    const ::std::string& name, PrivacyLevel* value) {
  return ::google::protobuf::internal::ParseNamedEnum<PrivacyLevel>(
    PrivacyLevel_descriptor(), name, value);
}
enum VisibilityLevel {
  VISIBILITY_LEVEL_PRIVATE = 0,
  VISIBILITY_LEVEL_PUBLIC = 1
};
TC_PROTO_API bool VisibilityLevel_IsValid(int value);
const VisibilityLevel VisibilityLevel_MIN = VISIBILITY_LEVEL_PRIVATE;
const VisibilityLevel VisibilityLevel_MAX = VISIBILITY_LEVEL_PUBLIC;
const int VisibilityLevel_ARRAYSIZE = VisibilityLevel_MAX + 1;

TC_PROTO_API const ::google::protobuf::EnumDescriptor* VisibilityLevel_descriptor();
inline const ::std::string& VisibilityLevel_Name(VisibilityLevel value) {
  return ::google::protobuf::internal::NameOfEnum(
    VisibilityLevel_descriptor(), value);
}
inline bool VisibilityLevel_Parse(
    const ::std::string& name, VisibilityLevel* value) {
  return ::google::protobuf::internal::ParseNamedEnum<VisibilityLevel>(
    VisibilityLevel_descriptor(), name, value);
}
enum JoinClubSource {
  JOIN_CLUB_SOURCE_NONE = 0,
  JOIN_CLUB_SOURCE_CLUB_FINDER = 1,
  JOIN_CLUB_SOURCE_CLUB_RECOMMENDATION = 2
};
TC_PROTO_API bool JoinClubSource_IsValid(int value);
const JoinClubSource JoinClubSource_MIN = JOIN_CLUB_SOURCE_NONE;
const JoinClubSource JoinClubSource_MAX = JOIN_CLUB_SOURCE_CLUB_RECOMMENDATION;
const int JoinClubSource_ARRAYSIZE = JoinClubSource_MAX + 1;

TC_PROTO_API const ::google::protobuf::EnumDescriptor* JoinClubSource_descriptor();
inline const ::std::string& JoinClubSource_Name(JoinClubSource value) {
  return ::google::protobuf::internal::NameOfEnum(
    JoinClubSource_descriptor(), value);
}
inline bool JoinClubSource_Parse(
    const ::std::string& name, JoinClubSource* value) {
  return ::google::protobuf::internal::ParseNamedEnum<JoinClubSource>(
    JoinClubSource_descriptor(), name, value);
}
// ===================================================================

// ===================================================================

// ===================================================================

// ===================================================================
// @@protoc_insertion_point(namespace_scope)

}  // namespace v1
}  // namespace club
}  // namespace protocol
}  // namespace bgs

#ifndef SWIG
namespace google {
namespace protobuf {
template <> struct is_proto_enum< ::bgs::protocol::club::v1::PrivacyLevel> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::bgs::protocol::club::v1::PrivacyLevel>() {
  return ::bgs::protocol::club::v1::PrivacyLevel_descriptor();
}
template <> struct is_proto_enum< ::bgs::protocol::club::v1::VisibilityLevel> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::bgs::protocol::club::v1::VisibilityLevel>() {
  return ::bgs::protocol::club::v1::VisibilityLevel_descriptor();
}
template <> struct is_proto_enum< ::bgs::protocol::club::v1::JoinClubSource> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::bgs::protocol::club::v1::JoinClubSource>() {
  return ::bgs::protocol::club::v1::JoinClubSource_descriptor();
}
}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_api_2fcommon_2fv1_2fclub_5fenum_2eproto__INCLUDED