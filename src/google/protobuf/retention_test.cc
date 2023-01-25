// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "google/protobuf/descriptor.pb.h"
#include <gtest/gtest.h>
#include "google/protobuf/unittest_retention.pb.h"

namespace google {
namespace protobuf {
namespace internal {
namespace {

TEST(RetentionTest, DirectOptions) {
  const google::protobuf::FileOptions& file_options =
      OptionsMessage::descriptor()->file()->options();
  EXPECT_EQ(file_options.GetExtension(plain_option), 1);
  EXPECT_EQ(file_options.GetExtension(runtime_retention_option), 2);
  // RETENTION_SOURCE option should be stripped.
  EXPECT_FALSE(file_options.HasExtension(source_retention_option));
  EXPECT_EQ(file_options.GetExtension(source_retention_option), 0);
}

void CheckOptionsMessageIsStrippedCorrectly(const OptionsMessage& options) {
  EXPECT_EQ(options.plain_field(), 1);
  EXPECT_EQ(options.runtime_retention_field(), 2);
  // RETENTION_SOURCE field should be stripped.
  EXPECT_FALSE(options.has_source_retention_field());
  EXPECT_EQ(options.source_retention_field(), 0);
}

TEST(RetentionTest, FieldsNestedInRepeatedMessage) {
  const google::protobuf::FileOptions& file_options =
      OptionsMessage::descriptor()->file()->options();
  ASSERT_EQ(1, file_options.ExtensionSize(repeated_options));
  const OptionsMessage& options_message =
      file_options.GetRepeatedExtension(repeated_options)[0];
  CheckOptionsMessageIsStrippedCorrectly(options_message);
}

TEST(RetentionTest, File) {
  CheckOptionsMessageIsStrippedCorrectly(
      OptionsMessage::descriptor()->file()->options().GetExtension(
          file_option));
}

TEST(RetentionTest, TopLevelMessage) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelMessage::descriptor()->options().GetExtension(message_option));
}

TEST(RetentionTest, NestedMessage) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelMessage::NestedMessage::descriptor()->options().GetExtension(
          message_option));
}

TEST(RetentionTest, TopLevelEnum) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelEnum_descriptor()->options().GetExtension(enum_option));
}

TEST(RetentionTest, NestedEnum) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelMessage::NestedEnum_descriptor()->options().GetExtension(
          enum_option));
}

TEST(RetentionTest, EnumEntry) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelEnum_descriptor()->value(0)->options().GetExtension(
          enum_entry_option));
}

TEST(RetentionTest, TopLevelExtension) {
  CheckOptionsMessageIsStrippedCorrectly(TopLevelMessage::descriptor()
                                             ->file()
                                             ->FindExtensionByName("i")
                                             ->options()
                                             .GetExtension(field_option));
}

TEST(RetentionTest, NestedExtension) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelMessage::descriptor()->extension(0)->options().GetExtension(
          field_option));
}

TEST(RetentionTest, Field) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelMessage::descriptor()->field(0)->options().GetExtension(
          field_option));
}

TEST(RetentionTest, Oneof) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelMessage::descriptor()->oneof_decl(0)->options().GetExtension(
          oneof_option));
}

TEST(RetentionTest, ExtensionRange) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelMessage::descriptor()->extension_range(0)->options_->GetExtension(
          extension_range_option));
}

TEST(RetentionTest, Service) {
  CheckOptionsMessageIsStrippedCorrectly(
      TopLevelMessage::descriptor()->file()->service(0)->options().GetExtension(
          service_option));
}

TEST(RetentionTest, Method) {
  CheckOptionsMessageIsStrippedCorrectly(TopLevelMessage::descriptor()
                                             ->file()
                                             ->service(0)
                                             ->method(0)
                                             ->options()
                                             .GetExtension(method_option));
}

}  // namespace
}  // namespace internal
}  // namespace protobuf
}  // namespace google
