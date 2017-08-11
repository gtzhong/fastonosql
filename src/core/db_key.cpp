/*  Copyright (C) 2014-2017 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    FastoNoSQL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FastoNoSQL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FastoNoSQL.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "core/db_key.h"

#include <memory>  // for __shared_ptr
#include <string>  // for string
#include <vector>  // for vector

#include <common/convert2string.h>
#include <common/string_util.h>  // for JoinString, Tokenize

#include "core/global.h"

namespace fastonosql {
namespace core {

bool IsBinaryKey(const command_buffer_t& key) {
  for (size_t i = 0; i < key.size(); ++i) {
    char c = key[i];
    if (isprint(c) == 0) {  // should be hexed symbol
      return true;
    }
  }
  return false;
}

KeyString::KeyString() : key_() {}

KeyString::KeyString(const string_key_t& key_data) : key_() {
  SetKey(key_data);
}

string_key_t KeyString::GetKey() const {
  return key_;
}

void KeyString::SetKey(const string_key_t& key_data) {
  if (IsBinaryKey(key_data)) {
    command_buffer_writer_t wr;
    string_key_t hexed = common::utils::hex::encode(key_data, false);
    for (size_t i = 0; i < hexed.size(); i += 2) {
      wr << MAKE_COMMAND_BUFFER("\\x");
      wr << hexed[i];
      wr << hexed[i + 1];
    }
    key_ = wr.str();
  } else {
    key_ = key_data;
  }
}

const string_key_t::value_type* KeyString::GetKeyData() const {
  return key_.data();
}

string_key_t::size_type KeyString::GetKeySize() const {
  return key_.size();
}

bool KeyString::Equals(const KeyString& other) const {
  return key_ == other.key_;
}

KeyInfo::KeyInfo(const splited_namespaces_t& splited_namespaces_and_key, std::string ns_separator)
    : splited_namespaces_and_key_(splited_namespaces_and_key), ns_separator_(ns_separator) {}

std::string KeyInfo::GetKey() const {
  return common::JoinString(splited_namespaces_and_key_, ns_separator_);
}

bool KeyInfo::HasNamespace() const {
  size_t ns_size = GetNspaceSize();
  return ns_size > 0;
}

std::string KeyInfo::GetNspace() const {
  return JoinNamespace(splited_namespaces_and_key_.size() - 1);
}

size_t KeyInfo::GetNspaceSize() const {
  if (splited_namespaces_and_key_.empty()) {
    return 0;
  }

  return splited_namespaces_and_key_.size() - 1;
}

std::string KeyInfo::JoinNamespace(size_t pos) const {
  size_t ns_size = GetNspaceSize();
  if (ns_size > pos) {
    std::vector<std::string> copy;
    for (size_t i = 0; i <= pos; ++i) {
      copy.push_back(splited_namespaces_and_key_[i]);
    }
    return common::JoinString(copy, ns_separator_);
  }

  return std::string();
}

NKey::NKey() : key_(), ttl_(NO_TTL) {}

NKey::NKey(key_t key, ttl_t ttl_sec) : key_(key), ttl_(ttl_sec) {}

KeyInfo NKey::GetInfo(const std::string& ns_separator) const {
  KeyInfo::splited_namespaces_t tokens;
  common::Tokenize(common::ConvertToString(key_.GetKey()), ns_separator, &tokens);
  return KeyInfo(tokens, ns_separator);
}

key_t NKey::GetKey() const {
  return key_;
}

void NKey::SetKey(key_t key) {
  key_ = key;
}

ttl_t NKey::GetTTL() const {
  return ttl_;
}

void NKey::SetTTL(ttl_t ttl) {
  ttl_ = ttl;
}

bool NKey::Equals(const NKey& other) const {
  if (key_ != other.key_) {
    return false;
  }

  return ttl_ == other.ttl_;
}

NDbKValue::NDbKValue() : key_(), value_() {}

NDbKValue::NDbKValue(const NKey& key, NValue value) : key_(key), value_(value) {}

NKey NDbKValue::GetKey() const {
  return key_;
}

NValue NDbKValue::GetValue() const {
  return value_;
}

common::Value::Type NDbKValue::GetType() const {
  if (!value_) {
    return common::Value::TYPE_NULL;
  }

  return value_->GetType();
}

void NDbKValue::SetKey(const NKey& key) {
  key_ = key;
}

void NDbKValue::SetValue(NValue value) {
  value_ = value;
}

std::string NDbKValue::ValueString() const {
  return common::ConvertToString(value_.get(), " ");
}

bool NDbKValue::Equals(const NDbKValue& other) const {
  if (!key_.Equals(other.key_)) {
    return false;
  }

  if (!value_) {
    return !other.value_;
  }

  return value_->Equals(other.value_.get());
}

}  // namespace core
}  // namespace fastonosql
