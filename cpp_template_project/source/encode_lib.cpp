#include "encode_lib.h"

#include <algorithm>
#include <cmath>
#include <span>
#include <string>
#include <vector>

#include "encode_constants.h"

int Serializing::counting_size_from_bytes(int index) {
  return (int)(bytes[index - 3] << (en_const::bits * 3) |
               bytes[index - 2] << (en_const::bits * 2) |
               bytes[index - 1] << (en_const::bits) | bytes[index]);
  ;
}

int Serializing::counting_size_from_tag(unsigned char size) {
  return (int)(size - en_const::seventh_bit - en_const::sixth_bit);
}

std::string Serializing::making_string(int& start_index, int& data_size) {
  std::string name;
  if (bytes[start_index + data_size] & en_const::fifth_bit) {
    start_index = start_index + data_size + en_const::bytes_for_size + 1;
    data_size = counting_size_from_bytes(start_index - 1);
  } else {
    start_index = start_index + data_size + 1;
    data_size = counting_size_from_tag(bytes[start_index - 1]);
  }
  auto adding_char_to_final_string = [&](const unsigned char& byte) {
    std::string elem(1, static_cast<char>(byte));
    name += elem;
  };
  std::for_each(bytes.begin() + start_index,
                bytes.begin() + start_index + data_size,
                adding_char_to_final_string);
  return name;
}

bool Serializing::translate_bool_tag(int index) {
  return bytes[index] == en_const::bool_true_tag;
}

EncodeData Serializing::serialize_bytes_to_object() {
  int start_index = en_const::first_six_bytes;
  int data_size = counting_size_from_bytes(start_index - 1);
  object.data.assign(bytes.begin() + start_index,
                     bytes.begin() + start_index + data_size);
  object.key = making_string(start_index, data_size);
  object.vec = making_string(start_index, data_size);
  object.algorithm_name = making_string(start_index, data_size);
  object.encode = translate_bool_tag(start_index + data_size);
  return object;
}

EncodeData unpack(std::vector<unsigned char>& bytes) {
  Serializing helper;
  helper.set_bytes(bytes);
  return helper.serialize_bytes_to_object();
}

void Serializing::counting_size_to_bytes(int data_size) {
  int buffer[1]{data_size};
  auto const data = std::as_writable_bytes(std::span{buffer});
  bytes.resize(bytes.size() + en_const::bytes_for_size);
  std::transform(std::rbegin(data), std::rend(data),
                 bytes.end() - en_const::bytes_for_size, [&](std::byte byte) {
                   return static_cast<unsigned char>(byte);
                 });
}

void Serializing::writing_string_tag(int data_size) {
  unsigned char string_type_tag = en_const::seventh_bit + en_const::sixth_bit;
  if (data_size >= en_const::fifth_bit) {
    string_type_tag += en_const::fifth_bit + en_const::third_bit;
    bytes.push_back(string_type_tag);
    counting_size_to_bytes(data_size);
  } else {
    string_type_tag += data_size;
    bytes.push_back(string_type_tag);
  }
}

void Serializing::writing_string(std::string& name) {
  writing_string_tag(name.length());
  auto adding_char_from_string_to_bytes = [this](const char& ch) {
    unsigned char elem = static_cast<unsigned char>(ch);
    bytes.push_back(elem);
  };
  std::for_each(name.begin(), name.end(), adding_char_from_string_to_bytes);
}

encoded_bytes Serializing::serialize_object_to_bytes() {
  bytes.push_back(en_const::start_end_tag);
  bytes.push_back(en_const::undefined_type_tag);
  int data_size = object.data.size();
  counting_size_to_bytes(data_size);
  bytes.insert(bytes.end(), object.data.begin(), object.data.end());
  writing_string(object.key);
  writing_string(object.vec);
  writing_string(object.algorithm_name);
  if (object.encode) {
    bytes.push_back(en_const::bool_true_tag);
  } else {
    bytes.push_back(en_const::bool_false_tag);
  }
  bytes.push_back(en_const::start_end_tag);
  return bytes;
}

std::vector<unsigned char> pack(EncodeData& object) {
  Serializing helper;
  helper.set_object(object);
  return helper.serialize_object_to_bytes();
}