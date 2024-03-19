#include "encode_lib.h"

#include <cmath>
#include <string>
#include <vector>

constexpr int eighth_bit = 128;
constexpr int seventh_bit = 64;
constexpr int sixth_bit = 32;
constexpr int fifth_bit = 16;
constexpr int fourth_bit = 8;
constexpr unsigned char bool_true_tag = 32;
constexpr unsigned char bool_false_tag = 64;
constexpr unsigned char start_end_tag = 0;
constexpr int bits = 8;
constexpr int bits_for_size = 4;
constexpr int second_bit = 4;
constexpr unsigned char undefined_type_tag = 20;

int Serializing::counting_size_from_bytes(int index) {
  int data_size = 0;
  int size = index - bits_for_size;
  for (int i = 0; index > size; --index, ++i) {
    for (int k = eighth_bit; k > 0; k = k / 2) {
      if (bytes[index] & k) {
        data_size += k * pow(2, bits * i);
      }
    }
  }
  return data_size;
}

int Serializing::counting_size_from_tag(unsigned char size) {
  int data_size = 0;
  for (int k = fourth_bit; k > 0; k = k / 2) {
    if (size & k) {
      data_size += k;
    }
  }
  return data_size;
}

std::string Serializing::making_string(int& start_index, int& data_size) {
  std::string name;
  if (bytes[start_index + data_size] & fifth_bit) {
    start_index = start_index + data_size + 5;
    data_size = counting_size_from_bytes(start_index - 1);
  } else {
    start_index = start_index + data_size + 1;
    data_size = counting_size_from_tag(bytes[start_index - 1]);
  }
  for (int i = start_index; i < start_index + data_size; ++i) {
    std::string elem(1, static_cast<char>(bytes[i]));
    name += elem;
  }
  return name;
}

EncodeData Serializing::serialize_bytes_to_object() {
  int start_index = 6;
  int data_size = counting_size_from_bytes(start_index - 1);
  for (int i = start_index; i < start_index + data_size; ++i) {
    object.data.push_back(bytes[i]);
  }
  object.key = making_string(start_index, data_size);
  object.vec = making_string(start_index, data_size);
  object.algorithm_name = making_string(start_index, data_size);
  object.encode = (bytes[start_index + data_size] == 32);
  return object;
}

EncodeData unpack(std::vector<unsigned char> bytes) {
  Serializing helper;
  helper.set_bytes(bytes);
  return helper.serialize_bytes_to_object();
}

void Serializing::counting_size_to_bytes(int data_size) {
  for (int i = 3; i >= 0; --i) {
    unsigned char elem = 0;
    for (int k = pow(2, bits * (i + 1) - 1); k > 0; k = k / 2) {
      if (data_size & k) {
        elem += k / pow(2, bits * i);
      }
    }
    bytes.push_back(elem);
  }
}

void Serializing::string_tag(int data_size) {
  unsigned char string_type_tag = seventh_bit + sixth_bit;
  if (data_size >= fifth_bit) {
    string_type_tag += fifth_bit + second_bit;
    bytes.push_back(string_type_tag);
    counting_size_to_bytes(data_size);
  } else {
    string_type_tag += data_size;
    bytes.push_back(string_type_tag);
  }
}

void Serializing::writing_string(std::string name) {
  string_tag(name.length());
  for (int i = 0; i < name.length(); ++i) {
    unsigned char elem = static_cast<unsigned char>(name[i]);
    bytes.push_back(elem);
  }
}

encoded_bytes Serializing::serialize_object_to_bytes() {
  bytes.push_back(start_end_tag);
  bytes.push_back(undefined_type_tag);
  int data_size = object.data.size();
  counting_size_to_bytes(data_size);
  bytes.insert(bytes.end(), object.data.begin(), object.data.end());
  writing_string(object.key);
  writing_string(object.vec);
  writing_string(object.algorithm_name);
  if (object.encode) {
    bytes.push_back(bool_true_tag);
  } else {
    bytes.push_back(bool_false_tag);
  }
  bytes.push_back(start_end_tag);
  return bytes;
}

std::vector<unsigned char> pack(EncodeData object) {
  Serializing helper;
  helper.set_object(object);
  return helper.serialize_object_to_bytes();
}
