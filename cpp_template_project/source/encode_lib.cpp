#include "encode_lib.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

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
  return bytes[index] == bool_true_tag;
}

EncodeData Serializing::serialize_bytes_to_object() {
  int start_index = first_six_bytes;
  int data_size = counting_size_from_bytes(start_index - 1);
  object.data.assign(bytes.begin() + start_index,
                     bytes.begin() + start_index + data_size);
  object.key = making_string(start_index, data_size);
  object.vec = making_string(start_index, data_size);
  object.algorithm_name = making_string(start_index, data_size);
  object.encode = translate_bool_tag(start_index + data_size);
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

void Serializing::writing_string_tag(int data_size) {
  unsigned char string_type_tag = seventh_bit + sixth_bit;
  if (data_size >= fifth_bit) {
    string_type_tag += fifth_bit + third_bit;
    bytes.push_back(string_type_tag);
    counting_size_to_bytes(data_size);
  } else {
    string_type_tag += data_size;
    bytes.push_back(string_type_tag);
  }
}

void Serializing::writing_string(std::string name) {
  writing_string_tag(name.length());
  auto adding_char_from_string_to_bytes = [this](const char& ch) {
    unsigned char elem = static_cast<unsigned char>(ch);
    bytes.push_back(elem);
  };
  std::for_each(name.begin(), name.end(), adding_char_from_string_to_bytes);
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
